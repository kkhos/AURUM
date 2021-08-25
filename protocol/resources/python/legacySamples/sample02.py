from __future__ import print_function
import aurum_pb2
import aurum_pb2_grpc
import logging
import grpc
import time

def sendKey(stub, key):
    rsp_key = stub.sendKey(aurum_pb2.ReqKey(
                    type=key,
                    actionType='STROKE',
                )
            )
    time.sleep(1)

def launchApp(stub, pkgname):
    rsp_launch = stub.launchApp(aurum_pb2.ReqLaunchApp(
           packageName=pkgname
    ))

def closeApp(stub, pkgname):
    rsp_launch = stub.closeApp(aurum_pb2.ReqCloseApp(
           packageName=pkgname
    ))

def getAppInfo(stub, pkgname):
   rsp_info = stub.getAppInfo(aurum_pb2.ReqGetAppInfo(packageName=pkgname))
   print(rsp_info)

def touchdown(stub, xx, yy):
   rsp = stub.touchDown(aurum_pb2.ReqTouchDown(coordination=aurum_pb2.Point(x=xx,y=yy)))
   print(rsp)

def touchmove(stub, xx, yy):
   rsp = stub.touchMove(aurum_pb2.ReqTouchMove(coordination=aurum_pb2.Point(x=xx,y=yy)))
   print(rsp)

def touchup(stub, xx, yy):
   rsp = stub.touchUp(aurum_pb2.ReqTouchUp(coordination=aurum_pb2.Point(x=xx,y=yy)))
   print(rsp)

def new_memo(stub):
    touchdown(stub, 630,1140)
    time.sleep(0.1)
    touchup(stub, 630,1140)
    time.sleep(0.5)

def findElementByText(stub, text):
    rsp_find = stub.findElement(aurum_pb2.ReqFindElement(
                strategy='TEXT',
                textField=text
            )
        )
    for item in rsp_find.elements:
        return item.elementId
    return None

def findElementsByText(stub, text):
    rsp_find = stub.findElement(aurum_pb2.ReqFindElement(
                strategy='TEXT',
                textField=text
            )
        )
    return rsp_find.elements

def clickById(stub, id):
    stub.click(aurum_pb2.ReqClick(
                type='ELEMENTID',
                elementId=id
            )
        )

def getAttrById(stub, id, attr):
   rsp = stub.getAttribute(aurum_pb2.ReqGetAttribute(attribute=attr, elementId=id))
   print(rsp)

def run_memo(stub):
    foundId = findElementByText(stub, 'All apps')
    time.sleep(1)
    if foundId != None:
        getAttrById(stub, foundId, 'VISIBLE')
        getAttrById(stub, foundId, 'CLICKABLE')
        getAttrById(stub, foundId, 'FOCUSED')
        getAttrById(stub, foundId, 'ENABLED')
        getAttrById(stub, foundId, 'CHECKED')
        clickById(stub, foundId)
        time.sleep(1)

        foundId = findElementByText(stub, 'Memo')
        time.sleep(1)
        if foundId != None:
            clickById(stub, foundId)
            time.sleep(2)

def set_text(stub, text):
    foundId = findElementByText(stub, 'Title')
    if foundId != None:
      clickById(stub, foundId)
      time.sleep(1.2)
      stub.setValue(aurum_pb2.ReqSetValue(
               elementId=foundId,
               stringValue=text))
    foundIds = findElementsByText(stub, "Memo")
    if len(foundIds) >= 2:
        stub.setValue(aurum_pb2.ReqSetValue(
                   elementId=foundIds[1].elementId,
                   stringValue=text))
    time.sleep(0.2)
    foundId = findElementByText(stub, 'DONE')
    if foundId != None:
      clickById(stub, foundId)

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)

        getAppInfo(stub, 'org.tizen.memo')
        time.sleep(1)
        sendKey(stub, 'HOME')
        time.sleep(1)
        closeApp(stub, 'org.tizen.memo')
        time.sleep(1)
        getAppInfo(stub, 'org.tizen.memo')
        time.sleep(1)
        run_memo(stub)
        time.sleep(1)
        new_memo(stub)
        time.sleep(1)
        set_text(stub, 'hello')

if __name__ == '__main__':
    logging.basicConfig()
    run()
