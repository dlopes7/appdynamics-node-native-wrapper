var appd = require("./appd-wrapper-agent");

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

for (var i = 0; i < 100; i++) {
  var btID = appd.startBT("test_bt1", null);
  console.log("Started BT: " + btID);

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
}
