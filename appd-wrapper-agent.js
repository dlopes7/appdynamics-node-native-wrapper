var addon = require("bindings")("hello");

console.log(addon.hello()); // 'world'
console.log(
  addon.profile(
    "192.168.0.38",
    8090,
    false,
    "customer1",
    "664f9a73-9f7c-400e-b2e8-ad8b9d019041",
    "NodeC++Wrapper",
    "wrapper",
    "david-mac"
  )
);

console.log(" Inicializado");
addon.terminate();

process.exit();
