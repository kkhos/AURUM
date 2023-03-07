---
layout: default
title: Send Key
parent: Device Control
grand_parent: Commands
nav_order: 18
---
# Send Key
Generate key event
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
response = stub.sendKey(ReqKey(type = 'XF86', actionType = 'STROKE', XF86keyCode = 'Left'))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.sendKey({ type: 'XF86', actionType: 'STROKE', XF86keyCode: 'Return'}, (err, response) => {
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
var response = client.sendKey(new ReqKey{ Type = ReqKey.Types.KeyType.Xf86, ActionType = ReqKey.Types.KeyActionType.Stroke, XF86KeyCode = "Left" });
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
            flick
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqKey {
   enum KeyType {
      BACK = 0;
      MENU = 1;
      HOME = 2;
      VOLUP = 3;
      VOLDOWN = 4;
      POWER = 5;
      XF86 = 7;
      WHEELUP = 8;
      WHEELDOWN = 9;
   }
   enum KeyActionType {
      STROKE = 0;
      LONG_STROKE = 1;
      PRESS = 2;
      RELEASE = 3;
      REPEAT = 4;
   }
   KeyType type = 1;
   KeyActionType actionType = 2;
   string XF86keyCode = 3;
   int32 durationMs = 4;
   int32 intervalMs = 5;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspKey {
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