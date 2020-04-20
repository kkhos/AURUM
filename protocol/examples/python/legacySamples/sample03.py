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
    time.sleep(1.5)

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

def sync(stub):
   rsp = stub.sync(aurum_pb2.ReqEmpty())
   print(rsp)


def new_memo(stub):
    touchdown(stub, 630,1140)
    time.sleep(0.1)
    touchup(stub, 630,1140)
    time.sleep(0.3)

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
    els = []
    for el in rsp_find.elements:
        els.append(el.elementId)
    return els

def clickById(stub, id):
    stub.click(aurum_pb2.ReqClick(
                type='ELEMENTID',
                elementId=id
            )
        )

def run_memo(stub):
    foundId = findElementByText(stub, 'All apps')
    time.sleep(1.5)
    if foundId != None:
        clickById(stub, foundId)
        time.sleep(1.5)

        foundId = findElementByText(stub, 'Memo')
        time.sleep(1.5)
        if foundId != None:
            clickById(stub, foundId)
            time.sleep(2.5)

def set_text(stub, text):
    foundIds = findElementsByText(stub, "Memo")
    foundIds += (findElementsByText(stub, "Title"))
    print(foundIds)
    for el in foundIds:
        print(el)
        stub.setValue(aurum_pb2.ReqSetValue(
                   elementId=el,
                   stringValue=text))

    #foundId = findElementByText(stub, 'DONE')
    #if foundId != None:
    #  clickById(stub, foundId)


def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)
        set_text(stub, 'hello')

if __name__ == '__main__':
    logging.basicConfig()
    run()
