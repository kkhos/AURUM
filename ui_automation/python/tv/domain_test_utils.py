import time
from aurum_pb2 import *

ACTION_DELAY_100MS = 0.1
ACTION_DELAY_500MS = 0.5
ACTION_DELAY_1000MS = 1
ACTION_DELAY_3000MS = 3
ACTION_DELAY_5000MS = 5
ACTION_DELAY_10000MS = 10

def SEND_KEY(stub, keyName, Delay):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode=keyName))
    time.sleep(Delay)

def RETURN_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(ACTION_DELAY_3000MS)

def HOME_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='XF86Home'))
    time.sleep(ACTION_DELAY_5000MS)

def DOWN_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Down'))
    time.sleep(ACTION_DELAY_500MS)

def UP_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    time.sleep(ACTION_DELAY_500MS)

def LEFT_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Left'))
    time.sleep(ACTION_DELAY_500MS)

def RIGHT_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Right'))
    time.sleep(ACTION_DELAY_500MS)

def LEFT_LONG_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='LONG_STROKE', XF86keyCode='Left'))
    time.sleep(ACTION_DELAY_500MS)

def RIGHT_LONG_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='LONG_STROKE', XF86keyCode='Right'))
    time.sleep(ACTION_DELAY_500MS)

def EXIT_KEY(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='XF86Exit'))
    time.sleep(ACTION_DELAY_3000MS)

def IS_APP_RUNNING(stub, appName):
    return stub.getAppInfo(ReqGetAppInfo(packageName=appName)).isRunning

def IS_APP_INSTALLED(stub, appName):
    return stub.getAppInfo(ReqGetAppInfo(packageName=appName)).isInstalled

def IS_APP_FOCUSED(stub, appName):
    return stub.getAppInfo(ReqGetAppInfo(packageName=appName)).isFocused

def APP_LAUNCH(stub, appName):
    stub.launchApp(ReqLaunchApp(packageName=appName))
    time.sleep(ACTION_DELAY_5000MS)

def FIND_FOCUSED_ELEMENT(stub):
    response = stub.findElement(ReqFindElement(isFocused=True))
    return response

def FIND_EDITABLE_ELEMENT(stub):
    response = stub.findElement(ReqFindElement(widgetType='TextField'))
    return response

def FIND_EFL_EDITABLE_ELEMENT(stub):
    response = stub.findElement(ReqFindElement(widgetType='Elm_Entry'))
    return response

def SET_VALUE(stub, obj, val):
    stub.setValue(ReqSetValue(elementId=obj, stringValue=val))

def GET_VALUE(stub, obj):
    return stub.getValue(ReqGetValue(elementId=obj))

def CHECK_ELEMENT(response):
    if len(response.elements) <= 0:
        print("Fail to find object")
        return False
    return True

def COMP_ID(id1, id2):
    if id1 == id2: return False
    return True

def CHECK_NOTI_POPUP(stub):
    if stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.notification-app')).isRunning:
        print("Noti popup disable")
        RETURN_KEY(stub)