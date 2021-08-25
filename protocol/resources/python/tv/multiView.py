from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Second view size change and check
# Please refer key codes below page
# https://code.sec.samsung.net/confluence/display/GFX/VD+Key+Code+Table
def MultiViewSizeTest(stub):
    response = stub.findElement(ReqFindElement(textField='VSComponent2'))
    if len(response.elements) <= 0: return False

    responseGuide = stub.findElement(ReqFindElement(textField='Guide TextBox'))
    if len(response.elements) <= 0:
        stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))

    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Right'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    # Wait until render finished
    time.sleep(1)

    responseAfter = stub.findElement(ReqFindElement(textField='VSComponent2'))
    if len(responseAfter.elements) <= 0: return False

    if response.elements[0].geometry.width < responseAfter.elements[0].geometry.width:
        return True

    return False

# Launch 3rd-party app and long press back key test
def MultiViewContentsTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Left'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    # Wait until render finished
    time.sleep(10)

    # It fails if there is a View
    response= stub.findElement(ReqFindElement(textField='VSComponent2'))
    if len(response.elements) > 0: return False

    stub.sendKey(ReqKey(type='XF86', actionType='LONG_STROKE', XF86keyCode='XF86Back'))

    return True

# Launch application. it returns application running state
def launchAppTest(stub):
    stub.launchApp(ReqLaunchApp(packageName='com.samsung.tv.multiscreen'))
    time.sleep(5)
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.multiscreen')).isRunning

# Close application. it returns application running state
def closeAppTest(stub):
    stub.closeApp(ReqCloseApp(packageName='com.samsung.tv.multiscreen'))
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.multiscreen')).isRunning != True

def runTest(stub, testFunc):
    print("Testing started :", testFunc)

    result = testFunc(stub)

    print("Testing result :", result)
    return True

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        runTest(stub, launchAppTest)
        runTest(stub, MultiViewSizeTest)
        runTest(stub, MultiViewContentsTest)
        runTest(stub, closeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
