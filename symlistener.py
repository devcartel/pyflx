#!/usr/bin/python
import pyflx
import time

def onData(symbol, data={}):
    print("[onData] symbol:{}, data:{}".format(symbol, data))

sl = pyflx.Symlistener()
sl.onData(onData)

# Enter loop() and exit if connection error.
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
