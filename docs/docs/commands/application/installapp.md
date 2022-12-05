---
layout: default
title: Install App
parent: Application Control
grand_parent: Commands
nav_order: 2
---
# Install App
Install application
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
def get_file_chunks(filename):                                             
   CHUNK_SIZE = 1024 * 1024                                                
   with open(filename, 'rb') as f:                                         
       while True:                                                         
           piece = f.read(CHUNK_SIZE)                                      
           if len(piece) == 0:                                             
               return                                                      
        yield ReqInstallApp(package = piece)                              
                                                                           
tpkPath = './app.tpk'              
tpkFile = get_file_chunks(tpkPath)                                 
response = stub.installApp(tpkFile)
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
    </div>
    <div class="tab-pane fade" id="js-tab-pane">
{% capture code %}
{% highlight js linenos %}
let call = stub.installApp((err, response) => {
    if (err) console.log(err);
    else console.log(response);
});

let tpk = fs.createReadStream('app.tpk');
tpk.on('data', (chunk) => {		
    call.write({ package: chunk });
}).on('end', () => {
    call.end();
})
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
            installApp
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message ReqInstallApp{
    bytes package = 1;
}
{% endhighlight %}
{% endcapture %}
{% include fix_linenos.html code=code %}
{% assign code = nil %}
            </td>
            <td style="text-align: left">
{% capture code %}
{% highlight ruby %}
message RspInstallApp{
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

