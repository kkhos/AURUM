---
layout: default
title: Python
parent: Getting Started
nav_order: 31
---

# Quick Start
{: .fs-8 }

This guide gets you started with Aurum in Python with a simple working example.
{: .fs-5 .fw-300 }

## Prerequisites
- Python 3.5 or higher
- pip version 9.0.1 or higher

If necessary, upgrade your version of pip:
```bash
$ python3 -m pip install --upgrade pip
```
If you cannot upgrade pip due to a system-owned installation, you can run the example in a venv:
```bash
$ python3 -m venv v
$ source v/bin/activate
```

## Install gRPC and gRPC tools
```bash
$ pip install grpcio grpcio-tools
```

## Download the example
```bash
# Clone the repository to get the example code:
$ git clone -b tizen git://git.tizen.org/platform/core/uifw/aurum
# Navigate to the Python example:
$ cd aurum/examples/python
```

## Generate gRPC code from aurum.proto
From the examples/python directory:
```bash
$ curl -L -O https://github.sec.samsung.net/pages/tizen/aurum/protocol/aurum.proto  
$ python3 -m grpc_tools.protoc --python_out=. --grpc_python_out=. --proto_path=. aurum.proto
```

## Connect to target
```bash
$ sdb shell app_launcher -s org.tizen.aurum-bootstrap
$ sdb forward tcp:50051 tcp:50051
```

## Run a Aurum example
From the examples/python directory:
```bash
$ python3 helloworld.py
```
Congratulations! You’ve just run a UI automation test application with Aurum.

## Introduce an example code
{% capture code %}
{% highlight python linenos %}
import grpc
from aurum_pb2 import *
from aurum_pb2_grpc import BootstrapStub

def run():                                                         
    with grpc.insecure_channel('localhost:50051') as channel:
#   If you have a proxy problem, use the below code instead of the above code.
#   with grpc.insecure_channel('localhost:50051', options=(('grpc.enable_http_proxy', 0),)) as channel: 
        stub = BootstrapStub(channel)
        getDeviceTime(stub)

def getDeviceTime(stub):
    response = stub.getDeviceTime(ReqGetDeviceTime(type='SYSTEM'))
    print(response)

if __name__ == '__main__':                                         
    run()
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}

## What’s next
- Explore the [API reference](/tizen/aurum/docs/commands/).
- The Aurum [issue tracker](https://github.sec.samsung.net/tizen/aurum/issues) - let the Aurum maintainers know here if you think you've found a bug