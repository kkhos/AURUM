---
layout: default
title: C#
parent: Getting Started
nav_order: 34
---

# Quick Start
{: .fs-8 }

This guide gets you started with Aurum in Node with a simple working example.
{: .fs-5 .fw-300 }

## Prerequisites

## Download the example
```bash
# Clone the repository to get the example code:
$ git clone -b tizen git://git.tizen.org/platform/core/uifw/aurum
# Navigate to the Node example:
$ cd aurum/examples/C#
# Install the example's dependencies
```

## Download the aurum.proto
From the examples/node directory:
```bash
$ curl -L -O https://github.sec.samsung.net/pages/tizen/aurum/protocol/aurum.proto  
```

## Connect to target
```bash
$ sdb shell app_launcher -s org.tizen.aurum-bootstrap
$ sdb forward tcp:50051 tcp:50051
```

## Run a Aurum example
From the examples/C# directory:
```bash
$ dotnet run
```
Congratulations! You’ve just run a UI automation test application with Aurum.

## Introduce an example code
{% capture code %}
{% highlight csharp linenos %}
using System;
using Tizen.NUI;
using Tizen.NUI.BaseComponents;

using Grpc.Core;
using Aurum;

namespace GrpcSample
{
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Tizen.NUI.Accessibility.Accessibility.BridgeDisableAutoInit();
            Environment.SetEnvironmentVariable("GRPC_CSHARP_EXT_OVERRIDE_LOCATION", "/usr/lib/libgrpc_csharp_ext.so.20", EnvironmentVariableTarget.Process);

            Scene1 Instance = new Scene1();
            Instance.Run(args);
        }
    }

    public partial class Scene1 : NUIApplication
    {
        override protected void OnCreate()
        {
            base.OnCreate();
            Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            var client = new Bootstrap.BootstrapClient(channel);
            var response = client.getDeviceTime(new ReqGetDeviceTime{ Type = ReqGetDeviceTime.Types.TimeType.Wallclock });
            Console.WriteLine(response.LocaleDatetime);
        }
    }
}

{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}

## What’s next
- Explore the [API reference](/tizen/aurum/docs/commands/).
- The Aurum [issue tracker](https://github.sec.samsung.net/tizen/aurum/issues) - let the Aurum maintainers know here if you think you've found a bug