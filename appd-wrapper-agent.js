var addon = require('bindings')('hello');

console.log(addon.hello()); // 'world'
console.log(addon.profile('localhost'));
