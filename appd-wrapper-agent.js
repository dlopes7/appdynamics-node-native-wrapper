var appd = require("bindings")("hello");

console.log(appd.hello()); // 'world'
console.log(
  appd.profile(
    "controllerandeum-davidlopeseumlabdr-qcfxfvsr.srv.ravcloud.com",
    8090,
    false,
    "customer1",
    "2b7cbcf6-2a45-4f9b-acb3-1e23c6836767",
    "NodeC++Wrapper",
    "wrapper",
    "david-mac"
  )
);

console.log("Agente Inicializado");

appd.appd_backend_declare("HTTP", "Google");
console.log(
  appd.appd_backend_set_identifying_property("Google", "HOST", "www.google.com")
);
console.log(appd.appd_backend_add("Google"));

for (var i = 0; i < 100; i++) {
  var btID = appd.start_bt("test_bt1", null);
  console.log("Started BT: " + btID);

  var waitTill = new Date(new Date().getTime() + 3000);
  while (waitTill > new Date()) {}

  appd.end_bt(btID);
  console.log("End BT");

  var btID = appd.start_bt("test_bt2", null);
  console.log("Started BT: " + btID);
  var waitTill = new Date(new Date().getTime() + 2000);
  while (waitTill > new Date()) {}

  var exitCall = appd.appd_exitcall_begin(btID, "Google");
  var waitTill = new Date(new Date().getTime() + 500);
  while (waitTill > new Date()) {}
  appd.appd_exitcall_end(exitCall);

  while (waitTill > new Date()) {}

  appd.end_bt(btID);
  console.log("End BT");

  appd.end_bt(btID);
}

appd.terminate();

process.exit();
