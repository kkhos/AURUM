---
layout: default
title: Click
parent: Device Control
grand_parent: Commands
nav_order: 19
---
# Click
Generate click event
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
response = stub.click(ReqClick(type = 'COORD', coordination = Point(x = 100, y = 100)))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.click({ type: 'COORD', coordination: { x: 100, y: 100 } }, (err, response) => {
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
var response = client.click(new ReqClick{ Type = ReqClick.Types.RequestType.Coord, Coordination = new Point{ X = 100, Y = 100 } });
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
            click
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqClick{
   enum RequestType {
      ELEMENTID = 0;
      COORD = 1;
      ATSPI = 2;
   }
   RequestType type  = 1;
   oneof params {
      string elementId = 2;
      Point coordination = 3;
   }
}
message Point {
   int32 x = 1;
   int32 y = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspClick{
   RspStatus status = 1;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
        </tr>
    </tbody>
</table>