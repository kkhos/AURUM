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
From the examples/node directory:
```bash
$ node helloworld.js
```
Congratulations! You’ve just run a UI automation test application with Aurum.

## Introduce an example code
{% capture code %}
{% highlight js linenos %}
import * as grpc from "@grpc/grpc-js";                                       
import * as protoLoader from "@grpc/proto-loader";                           
                                                                             
let PROTO_PATH = "./aurum.proto";                                            
let packageDefinition = protoLoader.loadSync(PROTO_PATH, {                   
  keepCase: true,                                                            
  longs: String,                                                             
  enums: String,                                                             
  defaults: true,                                                            
  oneofs: true                                                               
});                                                                          
                                                                             
let aurum = grpc.loadPackageDefinition(packageDefinition).aurum;             
                                                                             
function main () {                                                           
  let stub = new aurum.Bootstrap(                                            
    "localhost:50051",                                                       
    grpc.credentials.createInsecure()                                        
  );                                                                         
// If you have a proxy problem, use the below code instead of the above code.
//  grpc.credentials.createInsecure(),                                       
//  { 'grpc.enable_http_proxy': 0 },                                         
//);                                                                         
                                                                             
  stub.getDeviceTime({ type: 'SYSTEM' }, (err, response) => {                
    if (err) {                                                               
      console.log(err);                                                      
    } else {                                                                 
      console.log(response);                                                 
    }                                                                        
  });                                                                        
                                                                             
  stub.close()                                                               
}                                                                            
                                                                             
main();
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}

## What’s next
- Explore the [API reference](/tizen/aurum/docs/commands/).
- The Aurum [issue tracker](https://github.sec.samsung.net/tizen/aurum/issues) - let the Aurum maintainers know here if you think you've found a bug