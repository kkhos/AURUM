from __future__ import print_function
import os
import sys
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

# Launch application. it returns application running state
def CheckNormalExecutionTest(stub):
    stub.launchApp(ReqLaunchApp(packageName='com.samsung.tv.multiscreen'))
    time.sleep(5)
    return stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.multiscreen')).isRunning

# MVScreenFocusView is focus area. check the focus area has moved well or not to follow generated key event
def CheckKeyNavigationTest(stub):
    # Wait till help popup disappear
    time.sleep(5)

    response = stub.findElement(ReqFindElement(textField='MVScreenFocusView'))
    if len(response.elements) <= 0: return False

    prevFocusX = response.elements[0].geometry.x

    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Left'))
    time.sleep(0.1)

    response = stub.findElement(ReqFindElement(textField='MVScreenFocusView'))
    if len(response.elements) <= 0: return False

    if response.elements[0].geometry.x == prevFocusX:
        print("Focus not changed")
        return False

    return True

# Check the Guide TextBox has appeared or not when select one view
def CheckVisualDefect(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='Return'))
    time.sleep(1)
    
    response = stub.findElement(ReqFindElement(textField='Guide TextBox'))
    if len(response.elements) <= 0: 
        print("Fail to find guide text box")
        return False

    return True

# Check the Exit Key event generate well or not
def CheckReturnExitKeyTest(stub):
    stub.sendKey(ReqKey(type='XF86', actionType='STROKE', XF86keyCode='XF86Exit'))
    time.sleep(3)

    if stub.getAppInfo(ReqGetAppInfo(packageName='com.samsung.tv.multiview')).isRunning == True:
        print("Exit is not generated")
        return False

    return True

def runTest(stub, testFunc):
    print("Testing started :", testFunc)

    result = testFunc(stub)

    print("Testing result :", result)
    return True

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        runTest(stub, CheckNormalExecutionTest)
        runTest(stub, CheckKeyNavigationTest)
        runTest(stub, CheckVisualDefect)
        runTest(stub, CheckReturnExitKeyTest)
        #runTest(stub, CheckCrashOccurs)

if __name__ == '__main__':
    logging.basicConfig()
    run()
