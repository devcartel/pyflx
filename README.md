# PyFLX
[![version](https://img.shields.io/pypi/v/pyflx.svg)](https://pypi.org/project/pyflx)
[![pyversion](https://img.shields.io/pypi/pyversions/pyflx.svg)](#)
[![platform](https://img.shields.io/badge/platform-linux|%20win-lightgray.svg)](#platform-availability)
[![license](https://img.shields.io/pypi/l/pyflx.svg)](https://github.com/devcartel/pyflx/blob/master/LICENSE.txt)
[![downloads](https://img.shields.io/pypi/dm/pyflx.svg)](https://pypi.org/project/pyflx)

Provides Python market data API access to FlexTrade SYM. PyFLX has two market data Python classes - **Symlistener** (client) for consuming market data from FlexTrade market data server and **Symproxy** (publisher or feeder) for publising custom tick data to FlexTrade market data server.

<p align="center">
    <img src="https://user-images.githubusercontent.com/3415706/62541201-85692d80-b883-11e9-826a-05191f845c87.png" alt="application" width="800"/>
</p>

## Installation
Install from [PyPI](https://pypi.org/project/pyflx) using `pip`:

    pip install pyflx

## Example
```python
import pyflx
import time

def onData(symbol, data={}):
    print("[onData] symbol:{}, data:{}".format(symbol, data))

sl = pyflx.Symlistener()
sl.onData(onData)
while True:
    try:
        if sl.connect('192.168.1.123', 7600):
            sl.subscribe('XAU/USD')
            sl.loop()
        time.sleep(5)
    except KeyboardInterrupt:
        exit(0)
    except Exception as e:
        print(e)
```

See `symlistener.py` and `symproxy.py` for example codes.

## API
### Symlistener Class
__pyflx.Symlistener()__  
Create an instance of Symlistener client.

    >> sl = pyflx.Symlistener()

__Symlistener.connect(_ip_address_, _port_)__  
_ip_address: str_   
_port: int_  
_➥return: bool_  
Connect to a SYM server on a specified port number. Return `true` if successful or `false` if not able to connect to the server.

    >> sl.connect('192.168.1.123', 7600)

__Symlistener.subscribe(_symbol_)__  
_symbol: str_   
Subscribe to market data of the symbol.

    >> sl.subscribe('XAU/USD')

__Symlistener.subscribeAll()__    
Subscribe to market data of all symbols.

    >> sl.subscribeAll()

__Symlistener.unsubscribe(_symbol_)__  
_symbol: str_   
Unsubscribe to market data of the symbol.

    >> sl.unsubscribe('XAU/USD')

__Symlistener.subscribeAll()__  
Unsubscribe to market data of all symbols.

    >> sl.unsubscribeAll()

__Symlistener.loop()__  
Enter an event loop and listens for callbacks .

    >> sl.loop()

__Symlistener.onData(_symbol_, _data_)__  
_symbol: str_  
_data: dict_  
A callback `onData`, if assigned, is called upon receiving messages from SYM server.

    >> def onData(symbol, data):
    >>     print("[onData] symbol:{}, data:{}".format(symbol, data))
    >>
    >> sl = pyflx.Symlistener()
    >> sl.onData(onData)

### Symproxy Class
__pyflx.Symproxy()__  
Create an instance of Symproxy server.

    >> sp = pyflx.Symproxy()

__Symproxy.initialize(_port_)__  
_port: int_   
Listens to an incoming connection request on a port.

    >> sp.initialize(7600)

__Symproxy.getFd()__  
_➥return: int_  
Returns either client or a binding socket.

    >> sp.getFd()

__Symproxy.select()__  
_➥return: int_  
Looks for events on a socket.

    >> sp.select()

__Symproxy.checkSocket()__  
Process checks requests and callbacks will be triggered.

    >> sp.checkSocket()

__Symproxy.publish(_symbol_, _data_)__  
_symbol: str_  
_data: dict_  
Publishes market data tick update to a SYM server.

    >> sp.publish('XAU/USD', {0:1420, 1:1421.5, 512:1000, 513:2000, 1032:'USD', 1541:'U'})

__Symproxy.onLogon(_username_, _password_)__  
_username: str_  
_password: str_  
A callback `onLogon`, if assigned, is called upon receiving a logon message from a client.

    >> def onLogon(username, password):
    >>     print("[onLogon] username:{}, password:{}".format(username, password))
    >>
    >> sp = pyflx.Symproxy()
    >> sp.initialize(7600)
    >> sp.onLogon(onLogon)
    >> sp.onSubscribe(onSubscribe)
    >> sp.onUnsubscribe(onUnsubscribe)

__Symproxy.onSubscribe(_symbol_)__  
_symbol: str_  
A callback `onSubscribe`, if assigned, is called upon receiving a symbol subscription message from a client.

__Symproxy.onUnsubscribe(_username_, _password_)__  
_symbol: str_  
A callback `onUnsubscribe`, if assigned, is called upon receiving a symbol unsubscription message from a client.

## Support
* Report an issue in [issue tracker](https://github.com/devcartel/pyflx/issues)

## Changelog
1.2.0
* 7 August 2019
* Support for Python 2.7
* Symlistener class
* Symproxy class
