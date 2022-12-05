---
layout: default
title: Touch Up
parent: Device Control
grand_parent: Commands
nav_order: 23
---
# Touch Up
Generate touch up event
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
response = stub.touchDown(ReqTouchDown(coordination = Point(x = 100, y = 100)))
response2 = stub.touchUp(ReqTouchUp(seqId=response.seqId, coordination = Point(x = 100,y = 100)))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.touchDown({ coordination: { x: 100, y: 100 } }, (err, response) => {
    if (err) console.log(err);
    else
    {
        console.log(response);
        stub.touchUp({ seqId: response.seqId, coordination: { x: 100, y: 100 } }, (err2, response2) => {
            if (err2) console.log(err2);
            else console.log(response2);
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
            touchUp
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqTouchUp {
   int32 seqId = 1;
   Point coordination = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspTouchUp {
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