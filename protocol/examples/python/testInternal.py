from __future__ import print_function
from aurum_pb2 import *
import aurum_pb2_grpc
import logging
import grpc
import time
from tkinter import *
from PIL import ImageTk,Image

def touchTest(stub):
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=0))
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=1))
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=2))

    res = stub.touchDown(ReqTouchDown(coordination=Point(x=160,y=330)))
    print(res)
    seq = res.seqId
    print(seq)
    for yy in range(330, 30, -10):
        stub.touchMove(ReqTouchMove(coordination=Point(x=160,y=yy), seqId=seq))
    stub.touchUp(ReqTouchUp(coordination=Point(x=160,y=30), seqId=seq))

    return True

global img

def traverse(node, canvas, depth):
    print('traverse', depth)
    print('size:',node.geometry)

    #//canvas.pack()
    rect = canvas.create_rectangle(node.geometry.x, node.geometry.y, node.geometry.x+node.geometry.width, node.geometry.y+node.geometry.height,  outline='red')
    for child in node.child:
        traverse(child, canvas, depth+1)

def dumpTest(stub, tkroot):
    response = stub.findElement(ReqFindElement(maxDepth=1, minDepth=1, isShowing=True))
    print(response)
    for i in response.elements:
       response = stub.dumpObjectTree(ReqDumpObjectTree(elementId=i.elementId))
    print(response.roots)

    responses = stub.takeScreenshot(ReqTakeScreenshot())
    image = open("screenshot.png", "wb")
    for res in responses:
        image.write(res.image)
    image.close()

    canvas = Canvas(tkroot, width=360, height=360)
    canvas.pack()

    img = ImageTk.PhotoImage(Image.open("./screenshot.png"))
    print(img)
    canvas.create_image(0, 0, anchor=NW, image=img)
    canvas.img = img
    traverse(response.roots[0], canvas, 0)

def test(arg=None):
    print(test, arg)

def run():
    with grpc.insecure_channel('127.0.0.1:50051') as channel:
        stub = aurum_pb2_grpc.BootstrapStub(channel)
        root = Tk()
        root.geometry('360x360')

        dumpTest(stub, root)

        root.mainloop()

#        touchTest(stub)

#        print(stub.getLocation(ReqGetLocation()).status)
#        print(stub.sync(ReqEmpty()))
#        print(stub.getDeviceTime(ReqGetDeviceTime(type='WALLCLOCK')))
#        print(stub.sendKey(ReqKey(type='WHEELUP')))
#        time.sleep(0.1)
#        print(stub.sendKey(ReqKey(type='WHEELDOWN')))
#        time.sleep(0.1)
#        print(stub.sendKey(ReqKey(type='HOME')))
#        print(stub.sendKey(ReqKey(type='POWER')))
#        stub.killServer(aurum_pb2.ReqEmpty())

if __name__ == '__main__':
    logging.basicConfig()
    run()
