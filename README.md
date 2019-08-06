# PyFLX

Provides Python market data API access to FlexTrade SYM. PyFLX has two market data Python classes - **Symlistener** (client) for consuming market data from FlexTrade market data server and **Symproxy** (publisher or feeder) for publising custom tick data to FlexTrade market data server.

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
        if sl.connect('127.0.0.1', 7600):
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
*To be updated*

## Support
* Report an issue in [issue tracker](https://github.com/devcartel/pyflx/issues)

## Changelog
1.2.0
* 7 August 2019
* Support for Python 2.7
* Symlistener class
* Symproxy class
