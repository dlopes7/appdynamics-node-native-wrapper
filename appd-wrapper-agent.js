var appd = require("bindings")("hello");

export var BACKEND_TYPES = {
  APPD_BACKEND_HTTP: "HTTP",
  APPD_BACKEND_DB: "DB",
  APPD_BACKEND_CACHE: "CACHE",
  APPD_BACKEND_RABBITMQ: "RABBITMQ",
  APPD_BACKEND_WEBSERVICE: "WEBSERVICE",
  APPD_BACKEND_JMS: "JMS",
  APPD_BACKEND_WEBSPHEREMQ: "WEBSPHERE_MQ"
};

export function profile(
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
export function backendDeclare(backendType, backendName) {
  appd.appd_backend_declare(backendType, backendName);
}

export function backendSetIdentifyingProperty(backendName, property, value) {
  return appd.appd_backend_set_identifying_property(
    backendName,
    property,
    value
  );
}

export function backendAdd(backendName) {
  return appd.appd_backend_add(backendName);
}

export function startBT(name, correlationHeader) {
  return appd.start_bt(name, correlationHeader);
}

export function endBT(btID) {
  appd.end_bt(btID);
}

export function exitCallBegin(btID, backendName) {
  return appd.appd_exitcall_begin(btID, backendName);
}

export function exitCallEnd(exitCallID) {
  appd.appd_exitcall_end(exitCallID);
}

export function terminate() {
  appd.terminate();
}
