from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Find TextField and input "Movie" text
def SearchTestWithText(stub):
    response = stub.findElement(ReqFindElement(widgetType='TextField'))
    if len(response.elements) <= 0: return False
    targetObj = response.elements[0].elementId
    testString = 'Movie'
    stub.setValue(ReqSetValue(elementId=targetObj, stringValue=testString))

    # Wait until result upload
    time.sleep(2)

    return True

# Find Foused item and move focus to right then compare focused item with previous one
def SearchFocusedObject(stub):
    response = stub.findElement(ReqFindElement(isFocused=True))
    if len(response.elements) <= 0: return False

    prevObj = response.elements[0].elementId
    stub.sendKey(ReqKey(type='XF86', actionType='LONG_STROKE', XF86keyCode='Right'))
    time.sleep(1)

    response = stub.findElement(ReqFindElement(isFocused=True))
    if len(response.elements) <= 0: return False

    if prevObj != response.elements[0].elementId:
        return True

    return False


# Launch application. it returns application running state
def launchAppTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='XF86Search'))
    # Wait until app launch
    time.sleep(5)
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.searchall')).isRunning

# Close application. it returns application running state
def closeAppTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='XF86Exit'))
    time.sleep(2)
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.searchall')).isRunning != True

def runTest(stub, testFunc):
    print("Testing started :", testFunc)

    result = testFunc(stub)

    print("Testing result :", result)
    return True

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        runTest(stub, launchAppTest)
        runTest(stub, SearchTestWithText)
        runTest(stub, SearchFocusedObject)
        runTest(stub, closeAppTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
