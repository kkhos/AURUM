from __future__ import print_function
import aurum_pb2
import aurum_pb2_grpc
import logging
import grpc
import time

def back(stub):
    rsp_key = stub.sendKey(aurum_pb2.ReqKey(
                    type='BACK',
                    actionType='STROKE',
                )
            )

    rsp_find = stub.findElement(aurum_pb2.ReqFindElement(
                    strategy='TEXT',
                    textField='TestMemo'
                )
            )

    for item in rsp_find.elements:
        print(item)
        stub.click(aurum_pb2.ReqClick(
                type='ELEMENTID',
                elementId=item.elementId
            )
        )

def findNclick(stub, text):
    rsp_find = stub.findElement(aurum_pb2.ReqFindElement(
                    strategy='TEXT',
                    textField=text
                )
            )

    for item in rsp_find.elements:
        print(item)
        stub.click(aurum_pb2.ReqClick(
                type='ELEMENTID',
                elementId=item.elementId
            )
        )

def flick(stub):
    rsp_flick = stub.flick(aurum_pb2.ReqFlick(
        startPoint=aurum_pb2.Point(x=100, y=100),
        endPoint=aurum_pb2.Point(x=400, y=400),
        durationMs=1
    ))

def launchApp(stub):
    rsp_launch = stub.launchApp(aurum_pb2.ReqLaunchApp(
           packageName='org.example.uicomponents'
    ))

def closeApp(stub):
    rsp_launch = stub.closeApp(aurum_pb2.ReqCloseApp(
           packageName='org.example.uicomponents'
    ))

CHUNK_SIZE = 1024 * 1024
def get_file_chunks(filename):
   with open(filename, 'rb') as f:
       while True:
           piece = f.read(CHUNK_SIZE)
           if len(piece) == 0:
               return
           yield aurum_pb2.ReqInstallApp(package=piece)

def installApp(stub):
   in_file_name = './org.tizen.uicomponents.arm.tpk'
   chunks_generator = get_file_chunks(in_file_name)
   rsp_install = stub.installApp(chunks_generator)

def removeApp(stub):
   rsp_install = stub.removeApp(aurum_pb2.ReqRemoveApp(
                            packageName='org.example.uicomponents'
                        )
                    )

def getAppInfo(stub):
   rsp_info = stub.getAppInfo(aurum_pb2.ReqGetAppInfo(packageName='org.example.uicomponents'))
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

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)

        findNclick(stub, 'Testmemo')
        back(stub)
        flick(stub)
        installApp(stub)
        time.sleep(1)
        launchApp(stub)
        time.sleep(1)
        getAppInfo(stub)
        time.sleep(1)
        closeApp(stub)
        time.sleep(1)
#        removeApp(stub)
        flick(stub)
        touchdown(stub, 300, 300)
        touchmove(stub, 250, 250)
        touchmove(stub, 200, 200)
        touchmove(stub, 110, 110)
        touchup(stub, 100, 100)

if __name__ == '__main__':
    logging.basicConfig()
    run()
