/*
* (C) Copyright DevCartel. All rights reserved,
* Bangkok, Thailand
*/
#include "pyflx.h"

using namespace std;
using namespace boost::python;

struct connection_error : std::exception {
    char const* what() const throw() { return "CONNECTION_ERROR"; }
};

/***************
* Symproxy Class
***************/
Symproxy::Symproxy() :
    nCount(0),
    tmInit({0,0}),
    onLogonCallback(0),
    onSubscribeCallback(0),
    onUnsubscribeCallback(0)
{
}

Symproxy::~Symproxy() {}

bool Symproxy::initialize(int nBindPort) {
    return CSymProxy::Initialize(nBindPort, false);
}

int	Symproxy::getFd() {
    FD_ZERO(&readF);
    nFd = CSymProxy::GetFd();
    return nFd;
}

int	Symproxy::select() {
    tm = tmInit;
    FD_SET(nFd, &readF);
    int nReturn = ::select(nFd + 1, &readF, NULL, NULL, &tm);
    return nReturn;
}

bool Symproxy::checkSocket() {
    return CSymProxy::CheckSocket(readF);
}

bool Symproxy::dispatch() {
    return CSymProxy::Dispatch();
}

bool Symproxy::OnLogon(const char *szUserName, const char *szPassword) {
    if(onLogonCallback) {
        PyEval_InitThreads();
        PyGILState_STATE state = PyGILState_Ensure();
        boost::python::call<bool>(onLogonCallback, szUserName, szPassword);
        PyGILState_Release(state);
    }
    return true;
}

void Symproxy::OnSubscribe(const char* szTopic) {
    if(onSubscribeCallback) {
        PyEval_InitThreads();
        PyGILState_STATE state = PyGILState_Ensure();
        boost::python::call<void>(onSubscribeCallback, szTopic);
        PyGILState_Release(state);
    }
    return;
}

void Symproxy::OnUnsubscribe(const char* szTopic) {
    if(onUnsubscribeCallback) {
        PyEval_InitThreads();
        PyGILState_STATE state = PyGILState_Ensure();
        boost::python::call<void>(onUnsubscribeCallback, szTopic);
        PyGILState_Release(state);
    }
    return;
}

void Symproxy::onLogon(PyObject *callable) {
    onLogonCallback = callable;
}

void Symproxy::onSubscribe(PyObject *callable) {
    onSubscribeCallback = callable;
}

void Symproxy::onUnsubscribe(PyObject *callable) {
    onUnsubscribeCallback = callable;
}

void Symproxy::publish(const char* szTopic, boost::python::dict data) {
    HTICK pTick = CreateTick(szTopic);
    boost::python::list keys = (boost::python::list)data.keys();
    for (int j = len(keys) - 1 ; j >= 0; j--) {
        int key = extract<int>(keys[j]);
        extract<int> isInt(data[key]);
        if (isInt.check()) {
            int value = extract<int>(data[key]);
            if (SetTickField(pTick, key, value))
                continue;
        }
        extract<double> isDouble(data[key]);
        if (isDouble.check()) {
            double value = extract<double>(data[key]);
            if (SetTickField(pTick, key, value))
                continue;
        }
        extract<string> isString(str(data[key]));
        if (isString.check()) {
            string value = extract<string>(str(data[key]));
            if (SetTickField(pTick, key, value.c_str()))
                continue;
        }
        extract<char> isChar(str(data[key]));
        if (isChar.check()) {
            char value = extract<char>(str(data[key]));
            if (SetTickField(pTick, key, value))
                continue;
        }
    }
    Flush();
}

/******************
* Symlistener Class
******************/
Symlistener::Symlistener() :
    pConn(0),
    onDataCallback(0),
    m_nVal(0),
    m_dblVal(0),
    m_sVal(),
    m_cVal(0),
    m_Dict()
{
    pConn = new CSymConnection();
}

Symlistener::~Symlistener() {
    pConn->UnsubscribeAll();
    pConn->Disconnect();
    delete pConn;
}

bool Symlistener::connect(const char *host, int port, const char *username=0, const char *password=0) {
    bool bConnectSuccessful = pConn->Connect(host, port, username, password);
    if (bConnectSuccessful) {
        pConn->AddClient(this);
        pConn->DispatchEvents();
        return true;
    } else {
        return false;
    }
}

void Symlistener::dispatchEvents() {
    pConn->DispatchEvents();
}

bool Symlistener::subscribeAll() {
    return pConn->SubscribeAll();
}

bool Symlistener::unsubscribeAll() {
    return pConn->UnsubscribeAll();
}

bool Symlistener::subscribe(const char *szTopic) {
    return pConn->Subscribe(szTopic);
}

bool Symlistener::unsubscribe(const char *szTopic) {
    return pConn->Unsubscribe(szTopic);
}

void Symlistener::OnData(const char *szTopic, const CSymData *pData) {
    m_Dict.clear();
    SETFLIDS s = pData->m_setFlids;
    SETFLIDS::iterator it;
    for(it = s.begin(); it != s.end(); it++) {
        if(GetDouble(pData, *it, m_dblVal)) {
            m_Dict[*it] = m_dblVal;
        } else if(GetInt(pData, *it, m_nVal)) {
            m_Dict[*it] = m_nVal;
        } else if(GetString(pData, *it, m_sVal)) {
            m_Dict[*it] = m_sVal.c_str();
        } else if(GetChar(pData, *it, m_cVal)) {
            m_Dict[*it] = m_cVal;
        }
    }
    if(onDataCallback) {
        PyEval_InitThreads();
        PyGILState_STATE state = PyGILState_Ensure();
        boost::python::call<void>(onDataCallback, szTopic, m_Dict);
        PyGILState_Release(state);
    }
    return;
}

void Symlistener::onData(PyObject *callable) {
    onDataCallback = callable;
}

void Symlistener::loop() {
    pConn->Loop();
    throw connection_error();
}

void Symlistener::disconnect() {
    pConn->Disconnect();
}

/****************************************************************
* Exception translator for Python. See py_error class in pyflx.h.
****************************************************************/
static void translateException(const py_error &err) {
    PyErr_SetString(PyExc_Exception, err.what().c_str());
}

static void translate(const connection_error &err) {
    PyErr_SetString(PyExc_Exception, err.what());
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Symlistener_connect_overloads, Symlistener::connect, 2, 4)
BOOST_PYTHON_MODULE(pyflx)
{
    register_exception_translator<py_error>(&translateException);
    register_exception_translator<connection_error>(&translate);
    class_<Symproxy>("Symproxy")
        .def("initialize", &Symproxy::initialize)
        .def("onLogon", &Symproxy::onLogon)
        .def("onSubscribe", &Symproxy::onSubscribe)
        .def("onUnsubscribe", &Symproxy::onUnsubscribe)
        .def("getFd", &Symproxy::getFd)
        .def("select", &Symproxy::select)
        .def("checkSocket", &Symproxy::checkSocket)
        .def("dispatch", &Symproxy::dispatch)
        .def("publish", &Symproxy::publish)
        ;
    class_<Symlistener>("Symlistener")
        .def("connect", &Symlistener::connect, Symlistener_connect_overloads(args("username", "password")))
        .def("subscribeAll", &Symlistener::subscribeAll)
        .def("unsubscribeAll", &Symlistener::unsubscribeAll)
        .def("subscribe", &Symlistener::subscribe)
        .def("unsubscribe", &Symlistener::unsubscribe)
        .def("onData", &Symlistener::onData)
        .def("dispatchEvents", &Symlistener::dispatchEvents)
        .def("loop", &Symlistener::loop)
        .def("disconnect", &Symlistener::disconnect)
        ;
}
