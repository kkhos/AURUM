---
layout: default
title: Set Value
parent: UI Object Control
grand_parent: Commands
nav_order: 13
---
# Set Value
Set the value of the object
## Example Usage
<ul class="nav nav-tabs">
    <li class="nav-item remove active">
    <a class="nav-link active" data-toggle="tab" id="python-tab" data-target="#python-tab-pane">Python</a>
    </li>
   <li class="nav-item remove">
    <a class="nav-link" data-toggle="tab" id="js-tab" data-target="#js-tab-pane">Node</a>
    </li>
    <li class="nav-item remove" style="display:none">
    <a class="nav-link" data-toggle="tab" id="java-tab" data-target="#java-tab-pane">Java</a>
    </li>
    <li class="nav-item remove">
    <a class="nav-link" data-toggle="tab" id="grpc-tab" data-target="#grpc-tab-pane">C#</a>
    </li>
</ul>
<div class="tab-content">
    <div class="tab-pane fade active" id="python-tab-pane">
{% capture code %}
{% highlight python linenos %}
element = stub.findElement(ReqFindElement(widgetType = 'TextField'))
response = stub.setValue(ReqSetValue(elementId = element.element.elementId, stringValue = 'Test'))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.findElement({ widgetType: 'TextField' }, (err, element) => {
    if (err) console.log(err);
    else
    {
        console.log(element);
        stub.setValue({ elementId: element.element.elementId, stringValue: 'Test' }, (err2, response) => {
        if (err2) console.log(err2);
            else console.log(response);
        });
    }
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
var element = client.findElement(new ReqFindElement{ WidgetType = "TextField" });
var response = client.setValue(new ReqSetValue{ ElementId = element.Element.ElementId, StringValue = "Test" });
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
            setValue
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqSetValue {
   string elementId = 1;
   ParamType type  = 2;
   oneof params {
      string stringValue = 3;
      int32 intValue = 4;
      double doubleValue = 5;
      bool boolValue = 6;
   }
}

enum ParamType {
   STRING = 0;
   INT = 1;
   DOUBLE = 2;
   BOOL = 3;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspSetValue {
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