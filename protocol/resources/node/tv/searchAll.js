import * as grpc from "@grpc/grpc-js";
import * as protoLoader from "@grpc/proto-loader";
import assert, { doesNotMatch } from "assert";

let PROTO_PATH = "./aurum.proto";
let packageDefinition = protoLoader.loadSync(PROTO_PATH, {
  keepCase: true,
  longs: String,
  enums: String,
  defaults: true,
  oneofs: true
});

let aurum = grpc.loadPackageDefinition(packageDefinition).aurum;

describe("Aurum Nodejs Test", function () {
  let client;
  let searchAll;
  let textField;

  before(() => {    
    client = new aurum.Bootstrap(
      "localhost:50051",
      grpc.credentials.createInsecure()
    );
  });
  
  describe("SearchAll Test", () => {

    it("Launch App", function (done) {
      this.timeout(15000);
      client.launchApp({ packageName: "com.samsung.tv.searchall" }, (err, response) => {
        if (err) {
          console.log(err);
          done(err);
          return;
        }
        else {
          assert.strictEqual(response.status, "OK");
          done();
          return;
        }
      });
    });

    /*it("Find SearchAll Window", function (done) {
      this.timeout(15000);
      client.findElement({ maxDepth: 1, packageName: "com.samsung.tv.searchall", isActive: true, isShowing: true }, (err, response) => {
        if (err) {
          console.log(err);
	        done(err);
        } else {
          let returnList = response.elements;
          assert.ok(returnList.length > 0);
          console.log(returnList[0]);
          searchAll = returnList[0];
          assert.strictEqual(searchAll.package, "com.samsung.tv.searchall");
          done();
        }
      });
  });*/

    it("Find TextField", function (done) {
      this.timeout(15000);
      // assert.notEqual(searchAll, undefined);

      client.findElement({ widgetType: "TextField", packageName: "com.samsung.tv.searchall" }, (err, response) => {
        if (err) {
          console.log(err);
	  done(err);
        } else {
          let textField = response.element;
          assert.null(textField);

          assert.strictEqual(textField.widget_type, "TextField");
          done();
        }
      });
    });

    it("Set Text", function (done) {
      this.timeout(15000);
      assert.notEqual(textField, undefined);
      client.setValue({ elementId: textField.elementId, type: 0, stringValue: "Movie" }, (err, response) => {
        if (err) {
          console.log(err);
          done(err);
        } else {
          assert.strictEqual(response.status, "OK");
          done();
        }
      });      
    })
  });
});
