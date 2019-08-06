#!/usr/bin/python
import pyflx
import random
import time

def onLogon(username, password):
    print("[onLogon] username:{}, password:{}".format(username, password))

def onSubscribe(symbol):
    print("[onSubscribe] symbol:{}".format(symbol))

def onUnsubscribe(symbol):
    print("[onUnsubscribe] symbol:{}".format(symbol))

sp = pyflx.Symproxy()
sp.initialize(7600)
sp.onLogon(onLogon)
sp.onSubscribe(onSubscribe)
sp.onUnsubscribe(onUnsubscribe)

while True:
    try:
        nFd = sp.getFd()
        if nFd < 0: break
        nReturn = sp.select()
        if nReturn < 0: break
        sp.checkSocket()
        time.sleep(1)
        price = round(round(1023 + random.random(),2),2)
        sp.publish('XAU/USD', {0:price, 1:price+0.05, 512:1000, 513:2000, 1032:'USD', 1541:'U'})
    except KeyboardInterrupt:
        exit(0)
