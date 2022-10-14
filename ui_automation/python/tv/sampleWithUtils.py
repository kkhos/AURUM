from utils import *

def run():
    #Do below command if you needed.
    """
    checkSdb()
    getDeviceInfo()
    displayStop()
    isAurumReady()
    """
    
    stub = getStub()
    stub.enableScreenAnalyzer(ReqEnableScreenAnalyzer(enable=True))
    response = stub.findElements(ReqFindElements())
    print(response)


if __name__ == '__main__':
    run()
