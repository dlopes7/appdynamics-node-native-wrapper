var appd = require("bindings")("appd_wrapper_agent");

var BACKEND_TYPES = {
  APPD_BACKEND_HTTP: "HTTP",
  APPD_BACKEND_DB: "DB",
  APPD_BACKEND_CACHE: "CACHE",
  APPD_BACKEND_RABBITMQ: "RABBITMQ",
  APPD_BACKEND_WEBSERVICE: "WEBSERVICE",
  APPD_BACKEND_JMS: "JMS",
  APPD_BACKEND_WEBSPHEREMQ: "WEBSPHERE_MQ"
};

var LOG_LEVELS = {
  APPD_LOG_LEVEL_TRACE: 0,
  APPD_LOG_LEVEL_DEBUG: 1,
  APPD_LOG_LEVEL_INFO: 2,
  APPD_LOG_LEVEL_WARN: 3,
  APPD_LOG_LEVEL_ERROR: 4,
  APPD_LOG_LEVEL_FATAL: 5
};

var ERROR_LEVELS = {
  APPD_LEVEL_NOTICE: 0,
  APPD_LEVEL_WARNING: 1,
  APPD_LEVEL_ERROR: 2
};

function appDMiddleware(req, res, next) {
  var singularityHeader = req.headers.singularityheader;
  var btID = startBT(
    req.path
      .split("/")
      .splice(0, 3)
      .join("/"),
    singularityHeader
  );
  req.headers.appd_btID = btID;

  res.on("finish", function() {
    endBT(btID);
  });

  next();
}

function profile(
  controllerHost,
  controllerPort,
  useSSL,
  accountName,
  accessKey,
  applicationName,
  tierName,
  nodeName,
  logLevel,
  logDirectory
) {
  return appd.profile(
    controllerHost,
    controllerPort,
    useSSL,
    accountName,
    accessKey,
    applicationName,
    tierName,
    nodeName,
    logLevel,
    logDirectory
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

function exitCallGetCorrelationHeader(exitCallID) {
  return appd.appd_exitcall_get_correlation_header(exitCallID);
}

function exitCallSetDetails(exitCallID, details) {
  return appd.appd_exitcall_set_details(exitCallID, details);
}

function btAddError(btID, errorLevel, message, markBTAsError) {
  var isError = markBTAsError ? 1 : 0;
  return appd.appd_bt_add_error(btID, errorLevel, message, isError);
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
module.exports.exitCallGetCorrelationHeader = exitCallGetCorrelationHeader;
module.exports.appDMiddleware = appDMiddleware;
module.exports.LOG_LEVELS = LOG_LEVELS;
module.exports.ERROR_LEVELS = ERROR_LEVELS;
module.exports.exitCallSetDetails = exitCallSetDetails;
module.exports.btAddError = btAddError;

function noOp() {}

cleanup = function(callback) {
  // attach user callback to the process event emitter
  // if no callback, it will still exit gracefully on Ctrl-C
  callback = callback || noOp;
  process.on("cleanup", callback);

  // do app specific cleaning before exiting
  process.on("exit", function() {
    process.emit("cleanup");
  });

  // catch ctrl+c event and exit normally
  process.on("SIGINT", function() {
    console.log("Ctrl-C...");
    process.exit(2);
  });

  //catch uncaught exceptions, trace, then exit normally
  process.on("uncaughtException", function(e) {
    console.log("Uncaught Exception...");
    console.log(e.stack);
    process.exit(99);
  });
};

//do something when app is closing
function myCleanup() {
  appd.terminate();
}

cleanup(myCleanup);
