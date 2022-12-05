---
layout: default
title: Get Size
parent: UI Object Control
grand_parent: Commands
nav_order: 15
---
# Get Size
Get geometry of the object
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
</ul>
<div class="tab-content">
    <div class="tab-pane fade active" id="python-tab-pane">
{% capture code %}
{% highlight python linenos %}
element = stub.findElement(ReqFindElement(maxDepth = 1, isShowing = True))
responses = stub.getSize(ReqGetSize(type = 'SCREEN', elementId = element.element.elementId))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.findElement({ maxDepth: 1, isShowing: true }, (err, element) => {
    if (err) console.log(err);
    else
    {
        console.log(element);
        stub.getSize({ type: 'SCREEN', elementId: element.element.elementId }, (err2, response) => {
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
            getSize
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqGetSize {
   enum CoordType {
   SCREEN = 0;
   WINDOW = 1;
   }
   CoordType type = 1;
   string elementId = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspGetSize {
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