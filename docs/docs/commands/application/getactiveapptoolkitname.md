---
layout: default
title: Get Active App Toolkit Name
parent: Application Control
grand_parent: Commands
nav_order: 7
---
# Get Active App Tookit Name
Get the toolkit name of active application
## Example Usage
<ul class="nav nav-tabs">
    <li class="nav-item remove active">
    <a class="nav-link active" data-toggle="tab" id="python-tab" data-target="#python-tab-pane">Python</a>
    </li>
    <li class="nav-item remove">
    <a class="nav-link" data-toggle="tab" id="js-tab" data-target="#js-tab-pane">Javascript</a>
    </li>
    <li class="nav-item remove">
    <a class="nav-link" data-toggle="tab" id="java-tab" data-target="#java-tab-pane">Java</a>
    </li>
    <li class="nav-item remove">
    <a class="nav-link" data-toggle="tab" id="grpc-tab" data-target="#grpc-tab-pane">CsharpGrpc</a>
    </li>
</ul>
<div class="tab-content">
    <div class="tab-pane fade active" id="python-tab-pane">
{% capture code %}
{% highlight python linenos %}
response = stub.getActiveAppToolkitName(ReqGetActiveAppToolkitName())
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.getActiveAppToolkitName({}, (err, response) => {
    if (err) console.log(err);
    else console.log(response);
});
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="java-tab-pane">
    
    </div>
    <div class="tab-pane fade" id="grpc-tab-pane">
{% capture code %}
{% highlight csharp linenos %}
var response = client.getActiveAppToolkitName(new ReqGetActiveAppToolkitName{});
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
</div>

## Proto Definition
<table>
    <thead>
        <tr>
            <th style="text-align: left">
            Name
            </th>
            <th style="text-align: left">
            Requirement
            </th>
            <th style="text-align: left">
            Response
            </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td style="text-align: left">
            getActiveToolkitName
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqGetActiveAppToolkitName {
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspGetActiveAppToolkitName {
   RspStatus status = 1;   
   string toolkitName = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
        </tr>
    </tbody>
</table>