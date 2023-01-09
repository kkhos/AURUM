---
layout: default
title: Find Elements
parent: UI Object Control
grand_parent: Commands
nav_order: 9
---
# Find Elements
If objects that satisfies a specific condition are searched on the screen and found, the objects are returned 
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
response = stub.findElements(ReqFindElements(isClickable=True))
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
stub.findElements({ isClickable: true }, (err, response) => { 
    if (err) console.log(err);
    else console.log(response);
})
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
var response = client.findElements(new ReqFindElements{ IsClickable = true, });
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
</div>

## Proto Definition

<details markdown="block">
<summary>Expand</summary>
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
            findElements
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqFindElements {
   oneof _elementid {
      string elementId = 1;
   }

   oneof _automationid {
      string automationId = 2;
   }

   oneof _textfield {
      string textField = 3;
   }

   oneof _widgettype {
      string widgetType = 4;
   }

   oneof _widgetstyle {
      string widgetStyle = 5;
   }

   oneof _ischecked {
      bool isChecked = 6;
   }

   oneof _ischeckable {
      bool isCheckable= 7;
   }

   oneof _isclickable {
      bool isClickable = 8;
   }

   oneof _isenabled {
      bool isEnabled = 9;
   }

   oneof _isfocused {
      bool isFocused = 10;
   }

   oneof _isfocusable {
      bool isFocusable = 11;
   }

   oneof _isscrollable {
      bool isScrollable = 12;
   }

   oneof _isselected {
      bool isSelected = 13;
   }

   oneof _isshowing {
      bool isShowing = 14;
   }

   oneof _isactive {
      bool isActive = 15;
   }

   oneof _mindepth {
      int32 minDepth = 16;
   }

   oneof _maxdepth {
      int32 maxDepth = 17;
   }

   oneof _packagename {
      string packageName = 18;
   }

   oneof _textpartialmatch {
      string textPartialMatch = 19;
   }

   oneof _xpath {
      string xpath = 20;
   }

   oneof _ocrtext {
      string ocrText = 21;
   }

   oneof _geometry {
      Rect geometry = 22;
   }

   oneof _isvisible {
      bool isVisible = 23;
   }

   oneof _isselectable {
      bool isSelectable = 24;
   }

   oneof _ishighlightable {
      bool isHighlightable = 25;
   }

   repeated ReqFindElements children = 26;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspFindElements {
   RspStatus status = 1;
   repeated Element elements = 2;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
        </tr>
    </tbody>
</table>
</details>