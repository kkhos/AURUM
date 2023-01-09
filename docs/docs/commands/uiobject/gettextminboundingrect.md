---
layout: default
title: Get Text Min Bounding Rect
parent: UI Object Control
grand_parent: Commands
nav_order: 16
---
# Get Text Min Bounding Rect
Get the minium bounding rect size of text
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
element = stub.findElement(ReqFindElement(widgetType = 'TextLabel'))
response = stub.getTextMinBoundingRect(ReqGetTextMinBoundingRect(elementId = element.element.elementId))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.findElement({ widgetType: 'TextLabel' }, (err, element) => {
    if (err) console.log(err);
    else
    {
        console.log(element);
        stub.getTextMinBoundingRect({ elementId: element.element.elementId }, (err2, response) => {
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
var element = client.findElement(new ReqFindElement{ WidgetType = "TextLabel" });
var response = client.getTextMinBoundingRect(new ReqGetTextMinBoundingRect{ ElementId = element.Element.ElementId });
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
            getTextMinBoundingRect
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqGetTextMinBoundingRect {
   string elementId = 1;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspGetTextMinBoundingRect {
   RspStatus status = 1;
   Rect size = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
        </tr>
    </tbody>
</table>