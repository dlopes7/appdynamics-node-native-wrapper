var appd = require("bindings")("hello");

var BACKEND_TYPES = {
  APPD_BACKEND_HTTP: "HTTP",
  APPD_BACKEND_DB: "DB",
  APPD_BACKEND_CACHE: "CACHE",
  APPD_BACKEND_RABBITMQ: "RABBITMQ",
  APPD_BACKEND_WEBSERVICE: "WEBSERVICE",
  APPD_BACKEND_JMS: "JMS",
  APPD_BACKEND_WEBSPHEREMQ: "WEBSPHERE_MQ"
};

function profile(
  controllerHost,
  controllerPort,
  useSSL,
  accountName,
  accessKey,
  applicationName,
  tierName,
  nodeName
) {
  appd.profile(
    controllerHost,
    controllerPort,
    useSSL,
    accountName,
    accessKey,
    applicationName,
    tierName,
    nodeName
  );
}
function backendDeclare(backendType, backendName) {
  appd.appd_backend_declare(backendType, backendName);
}

function backendSetIdentifyingProperty(backendName, property, value) {
  return appd.appd_backend_set_identifying_property(
    backendName,
    property,
    value
  );
}

function backendAdd(backendName) {
  return appd.appd_backend_add(backendName);
}

function startBT(name, correlationHeader) {
  return appd.start_bt(name, correlationHeader);
}

function endBT(btID) {
  appd.end_bt(btID);
}

function exitCallBegin(btID, backendName) {
  return appd.appd_exitcall_begin(btID, backendName);
}

function exitCallEnd(exitCallID) {
  appd.appd_exitcall_end(exitCallID);
}

function terminate() {
  appd.terminate();
}

module.exports.BACKEND_TYPES = BACKEND_TYPES;
module.exports.profile = profile;
module.exports.backendDeclare = backendDeclare;
module.exports.backendSetIdentifyingProperty = backendSetIdentifyingProperty;
module.exports.backendAdd = backendAdd;
module.exports.startBT = startBT;
module.exports.endBT = endBT;
module.exports.exitCallBegin = exitCallBegin;
module.exports.exitCallEnd = exitCallEnd;
module.exports.terminate = terminate;
