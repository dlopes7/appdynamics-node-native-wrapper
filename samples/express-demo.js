var express = require("express");
var appd = require("../index");
var http = require("http");

appd.profile(
  "controller.com",
  8090,
  false,
  "customer1",
  "ravello",
  "NodeC++Wrapper",
  "nodejs0.10",
  "david-vm",
  appd.LOG_LEVELS.APPD_LOG_LEVEL_TRACE,
  "C:/nodelogs"
);

appd.backendDeclare(appd.BACKEND_TYPES.APPD_BACKEND_HTTP, "Hystrix");
appd.backendSetIdentifyingProperty(
  "Hystrix",
  "HOST",
  "http://platform-davidlopessmallcon-gokodoek.srv.ravcloud.com"
);
appd.backendAdd("Hystrix");

var app = express();

function registerBT(req, res, next) {
  var singularityHeader = req.headers.singularityheader;

  var btID = appd.startBT(req.url, singularityHeader);

  req.headers.btid = btID;
  console.log("Start BT", btID);

  res.on("finish", function() {
    var btID = req.headers.btid;
    console.log("End BT", btID);
    appd.endBT(btID);
  });

  next();
}

app.use(registerBT);

app.get("/", function(req, res) {
  var exitCall = appd.exitCallBegin(req.headers.btid, "Hystrix");
  var correlationHeader = appd.exitCallGetCorrelationHeader(exitCall);
  console.log(correlationHeader);

  options = {
    headers: { singularityheader: correlationHeader },
    hostname: "platform-davidlopessmallcon-gokodoek.srv.ravcloud.com",
    port: 8080,
    method: "GET",
    path: "/ok"
  };

  appd.exitCallSetDetails(exitCall, options.hostname + options.path);
  console.log("Details", options.hostname);

  var newreq = http
    .request(options, function(resp) {
      var data = "";

      // A chunk of data has been recieved.
      resp.on("data", function(chunk) {
        data += chunk;
      });

      // The whole response has been received. Print out the result.
      resp.on("end", function() {
        console.log(data);
        res.send(data);
        appd.exitCallEnd(exitCall);
      });
    })
    .on("error", function(err) {
      res.send(err.message);
      console.log("Error: " + err.message);
      appd.btAddError(
        req.headers.btid,
        appd.ERROR_LEVELS.APPD_LEVEL_ERROR,
        err.message,
        true
      );
      appd.exitCallEnd(exitCall);
    });

  newreq.end();
});

app.listen(3000);
