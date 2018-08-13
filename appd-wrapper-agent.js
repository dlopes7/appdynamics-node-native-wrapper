var addon = require("bindings")("hello");

console.log(addon.hello()); // 'world'
console.log(
  addon.profile(
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

var btID = addon.start_bt("test_bt1", null);
console.log("Started BT: " + btID);
setTimeout(function() {
  addon.end_bt(btID);
  console.log("End BT");
}, 3000);
//setTimeout(addon.end_bt(btID), 2000);

var btID = addon.start_bt("test_bt2", null);
console.log("Started BT: " + btID);
addon.end_bt(btID);
addon.terminate();

process.exit();
