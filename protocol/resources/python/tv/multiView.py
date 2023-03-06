from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Please refer key codes below page
# https://code.sec.samsung.net/confluence/display/GFX/VD+Key+Code+Table
def MultiViewSelectContentTest(stub):
    response = stub.findElement(ReqFindElements(textField='VSComponent2'))
    if response.element is None: return False

    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    # Wait until render finished
    time.sleep(3)

    responseAfter = stub.findElement(ReqFindElement(textField='VSComponent2'))
    if responseAfter.element is None: return False

    if response.element.geometry.height < responseAfter.element.geometry.height:
        return True

    return False

def MultiViewChangeContentTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='LONG_STROKE', XF86keyCode='XF86Back'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Left'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Up'))
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    # Wait until render finished
    time.sleep(5)

    # It fails if there is a View
    response= stub.findElement(ReqFindElements(textField='VSComponent2'))
    if response.element is None: return False

    return True

# Launch application. it returns application running state
def launchAppTest(stub):
    stub.launchApp(ReqLaunchApp(packageName='com.samsung.tv.multiscreen'))
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
        # Select Internet View
        runTest(stub, MultiViewSelectContentTest)
        # Change Internet View to Youtube View
        runTest(stub, MultiViewChangeContentTest)
        # Change Youtube View to Internet View
        runTest(stub, MultiViewChangeContentTest)
        runTest(stub, closeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
