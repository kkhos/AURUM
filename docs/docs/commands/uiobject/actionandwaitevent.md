---
layout: default
title: Action And Wait Event
parent: UI Object Control
grand_parent: Commands
nav_order: 25
---
# Action And Wait Event
Do action and wait event
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
response = stub.actionAndWaitEvent(ReqActionAndWaitEvent(type = 'KEY',
                                                       XF86keyCode = 'Down',
                                                       eventType = 'EVENT_STATE_CHANGED_FOCUSED',
                                                       timeoutMs=500))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.actionAndWaitEvent({ type: 'KEY', XF86keyCode: 'Down',
                                       eventType: 'EVENT_STATE_CHANGED_FOCUSED',
                                       timeoutMs: 500 }, (err, response) => {
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
var response = client.actionAndWaitEvent(new ReqActionAndWaitEvent{ Type = ReqActionAndWaitEvent.Types.ActionType.Key, XF86KeyCode = "Down",
                    EventType = ReqActionAndWaitEvent.Types.EventType.EventStateChangedFocused, TimeoutMs = 500 });
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
            actionAndWaitEvent
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqActionAndWaitEvent {
   enum ActionType{
      CLICK = 0;
      KEY = 1;
      FOCUS = 2;
   }
   enum EventType {
      EVENT_WINDOW_ACTIVATE = 0;
      EVENT_WINDOW_DEACTIVATE = 1;
      EVENT_STATE_CHANGED_FOCUSED = 2;
   }
   ActionType type = 1;
   oneof params {
      string elementId = 2;
      string XF86keyCode = 3;
   }
   EventType eventType = 4;
   int32 timeoutMs = 5;
   string packageName = 6;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspActionAndWaitEvent {
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