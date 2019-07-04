
var messages = require('./aurum_pb');
var services = require('./aurum_grpc_pb');

var grpc = require('grpc');

function main() {
  var client = new services.BootstrapClient('localhost:50051',
                                          grpc.credentials.createInsecure());

  var request = new messages.ReqFindElement();

  client.sync(new messages.ReqEmpty(), function(err, res){console.log(res)});

  request.setStrategy(proto.aurum.ReqFindElement.RequestType.TEXT);
  request.setTextfield("DONE");

  client.findElement(request, function(err, response) {
    if (err) {
      console.log(err);
      return;
    }
    var returnList = response.getElementsList();
    console.log(returnList);
    if (returnList.length > 0)
    {
      var request2 = new messages.ReqClick();
      request2.setType(proto.aurum.ReqClick.RequestType.ELEMENTID)
      request2.setElementid(returnList[0].getElementid());
      client.click(request2, function(err, response2) {
        if (err) {
          console.log(err);
          return;
        }
        console.log(response2);
      });
    }
  });
}

main();
