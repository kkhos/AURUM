import * as grpc from "@grpc/grpc-js";
import * as protoLoader from "@grpc/proto-loader";

let PROTO_PATH = "./aurum.proto";
let packageDefinition = protoLoader.loadSync(PROTO_PATH, {
  keepCase: true,
  longs: String,
  enums: String,
  defaults: true,
  oneofs: true
});

let aurum = grpc.loadPackageDefinition(packageDefinition).aurum;

function main () {  
  let stub = new aurum.Bootstrap(
    "localhost:50051",
    grpc.credentials.createInsecure()
  );
// If you have a proxy problem, use the below code instead of the above code.
//  grpc.credentials.createInsecure(),
//  { 'grpc.enable_http_proxy': 0 },  
//);                                  

  stub.getDeviceTime({ type: 'SYSTEM' }, (err, response) => {
    if (err) {
      console.log(err);
    } else {
      console.log(response);
    }
  });
  
  stub.close()
}

main();
