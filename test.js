var appd = require("./appd-wrapper-agent");

const http = require("http");

appd.profile(
  "platform-davidlopessmallcon-gokodoek.srv.ravcloud.com",
  8090,
  false,
  "customer1",
  "a4abbf5e-caf5-49e0-9685-796a7714a29b",
  "NodeC++Wrapper",
  "wrapper",
  "david-mac"
);

appd.backendDeclare(appd.BACKEND_TYPES.APPD_BACKEND_HTTP, "Controller");
appd.backendSetIdentifyingProperty(
  "Controller",
  "HOST",
  "http://www.controller.com"
);
appd.backendAdd("Controller");

appd.backendDeclare(appd.BACKEND_TYPES.APPD_BACKEND_HTTP, "Hystrix");
appd.backendSetIdentifyingProperty(
  "Hystrix",
  "HOST",
  "http://platform-davidlopessmallcon-gokodoek.srv.ravcloud.com"
);
appd.backendAdd("Hystrix");

for (var i = 0; i < 1; i++) {
  var btID = appd.startBT("test_bt1", null);
  console.log("Started BT: " + btID);
  var exit = appd.exitCallBegin(btID, "Hystrix");
  var correlationHeader = appd.exitCallGetCorrelationHeader(exit);

  options = {
    headers: { singularityheader: correlationHeader },
    hostname: "platform-davidlopessmallcon-gokodoek.srv.ravcloud.com",
    port: 8080,
    method: "GET",
    path: "/ok"
  };

  var req = http
    .request(options, function(resp) {
      var data = "";

      // A chunk of data has been recieved.
      resp.on("data", function(chunk) {
        data += chunk;
      });

      // The whole response has been received. Print out the result.
      resp.on("end", function() {
        console.log(data);
        appd.exitCallEnd(exit);

        appd.endBT(btID);
        console.log("End BT: ", btID);
      });
    })
    .on("error", function(err) {
      console.log("Error: " + err.message);
    });

  req.end();

  /*
  var waitTill = new Date(new Date().getTime() + 3000);
  while (waitTill > new Date()) {}

  appd.endBT(btID);
  console.log("End BT", btID);

  var btID = appd.startBT("test_bt2", null);
  console.log("Started BT: " + btID);
  var waitTill = new Date(new Date().getTime() + 2000);
  while (waitTill > new Date()) {}

  var exitCall = appd.exitCallBegin(btID, "Controller");
  var waitTill = new Date(new Date().getTime() + 500);
  while (waitTill > new Date()) {}
  appd.exitCallEnd(exitCall);

  while (waitTill > new Date()) {}

  appd.endBT(btID);
  console.log("End BT", btID);
  */
}
