from __future__ import print_function
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub
import logging
import grpc
import time

def findElementTest(stub):
    response = stub.findElement(ReqFindElement(isClickable=True))
    for el in response.elements:
        return True
    return False

def getValueTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    print("els", response)
    for el in response.elements:
        response = stub.getValue(ReqGetValue(elementId=el.elementId))
        return response.stringValue == 'Widgets'
    return False

def setValueTest(stub):
    response = stub.findElement(ReqFindElement(textField='Widgets'))
    for el in response.elements:
        print(el)
        stub.click(ReqClick(type='ELEMENTID', elementId=el.elemenetId))
        break

    response = stub.findElement(ReqFindElement(textField='Entry/Editfield'))
    for el in response.elements:
        print(el)
        stub.click(ReqClick(type='ELEMENTID', elementId=el.elemenetId))
        break

    return False

def getSizeTest(stub):
    return False

def clearTest(stub):
    return False

def getAttributeTest(stub):
    return False

def clickTest(stub):
    return False

def longClickTest(stub):
    return False

def flickTest(stub):
    return False

def touchDownTest(stub):
    return False

def touchMoveTest(stub):
    return False

def touchUpTest(stub):
    return False

def installAppTest(stub):
    return False

def removeAppTest(stub):
    return False

def getAppInfoTest(stub):
    return False

def launchAppTest(stub):
    return False

def closeAppTest(stub):
    return False

def sendKeyTest(stub):
    return False

def scrollToTest(stub):
    return False

def getDeviceTimeTest(stub):
    return False

def getLocationTest(stub):
    return False


def runTest(stub, testFunc):
    print("Testing started :", testFunc)
    assert True == testFunc(stub)

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = BootstrapStub(channel)
        stub.launchApp(ReqLaunchApp(packageName='com.samsung.ui-widget-sample'))
        #runTest(stub, findElementTest)
        runTest(stub, getValueTest)
        runTest(stub, setValueTest)
        runTest(stub, getSizeTest)
        runTest(stub, clearTest)
        runTest(stub, getAttributeTest)
        runTest(stub, clickTest)
        runTest(stub, longClickTest)
        runTest(stub, flickTest)
        runTest(stub, touchDownTest)
        runTest(stub, touchMoveTest)
        runTest(stub, touchUpTest)
        runTest(stub, installAppTest)
        runTest(stub, removeAppTest)
        runTest(stub, getAppInfoTest)
        runTest(stub, launchAppTest)
        runTest(stub, closeAppTest)
        runTest(stub, sendKeyTest)
        runTest(stub, scrollToTest)
        runTest(stub, getDeviceTimeTest)
        runTest(stub, getLocationTest)

if __name__ == '__main__':
    logging.basicConfig()
    run()
