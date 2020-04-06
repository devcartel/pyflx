// pyflx.h
#if !defined(__PYFLX_H)
#define __PYFLX_H

#include "Python.h"

//Boost Header files
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call_method.hpp>

#include <iostream>
#include <locale.h>
#include <set>

#include "CSymProxy.h"
#include "CSymData.h"
#include "CCommand.h"
#include "SymConnection.h"
#include "SymListener.h"

class CSymProxy;
class CSymConnection;
class CSymListener;

class Symproxy : public CSymProxy {
    public:
        Symproxy();
        virtual ~Symproxy();
        bool initialize(int nBindPort);
        int getFd();
        int select();
        bool checkSocket();
        bool closeClient();
        bool dispatch();
        virtual bool OnLogon(const char *szUserName, const char *szPassword);
        virtual void OnSubscribe(const char* szTopic);
        virtual void OnUnsubscribe(const char* szTopic);
        void onLogon(PyObject *callable);
        void onSubscribe(PyObject *callable);
        void onUnsubscribe(PyObject *callable);
        void publish(const char* szTopic, boost::python::dict data);

    private:
        int nFd;
        int nCount;
        int nPort;
        fd_set readF;
        struct timeval tmInit;
        struct timeval tm;
        PyObject *onLogonCallback;
        PyObject *onSubscribeCallback;
        PyObject *onUnsubscribeCallback;
};

class Symlistener : public CSymListener {
    public:
        Symlistener();
        virtual ~Symlistener();
        virtual bool connect(const char *host, int port, const char *username, const char *password);
        void dispatchEvents();
        bool subscribeAll();
        bool unsubscribeAll();
        bool subscribe(const char *szTopic);
        bool unsubscribe(const char *szTopic);
        void loop();
        virtual void OnData(const char *szTopic, const CSymData *pData);
        void onData(PyObject *callable);
        void disconnect();

    private:
        CSymConnection *pConn;
        PyObject *onDataCallback;
        int m_nVal;
        double m_dblVal;
        GrowingStr m_sVal;
        char m_cVal;
        boost::python::dict m_Dict;
};

class py_error {
    public:
        py_error(const std::string str) {_str = str;}
        ~py_error() {}
        std::string what() const {return _str;}
    private:
        std::string _str;
};
#endif // __PYFLX_H
