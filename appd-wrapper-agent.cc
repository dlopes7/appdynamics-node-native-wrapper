#include <node.h>
#include <time.h>
#include <appdynamics.h>
#include <windows.h>

using namespace v8;

const char *ToCString(const String::Utf8Value &value)
{
  return *value ? *value : "<string conversion failed>";
}

// Hello World Sample
Handle<Value> Method(const Arguments &args)
{
  HandleScope scope;
  return scope.Close(String::New("world"));
}

// AppDProfile will receive parameters:
// controllerHostName, controllerPort, controllerSslEnabled, accountName, accountAccessKey, applicationName, tierName, nodeName
Handle<Value> AppDProfile(const Arguments &args)
{
  HandleScope scope;
  srand(time(NULL));
  String::Utf8Value v8controllerHostName(args[0]);
  const char *controllerHostName = ToCString(v8controllerHostName);

  int controllerPort = args[1]->NumberValue();
  bool controllerSslEnabled = args[2]->BooleanValue();

  String::Utf8Value v8accountName(args[3]);
  const char *accountName = ToCString(v8accountName);

  String::Utf8Value v8accountAccessKey(args[4]);
  const char *accountAccessKey = ToCString(v8accountAccessKey);

  String::Utf8Value v8applicationName(args[5]);
  const char *applicationName = ToCString(v8applicationName);

  String::Utf8Value v8tierName(args[6]);
  const char *tierName = ToCString(v8tierName);

  String::Utf8Value v8nodeName(args[7]);
  const char *nodeName = ToCString(v8nodeName);

  printf("controllerHostName: %s\n", controllerHostName);
  printf("controllerPort: %d\n", controllerPort);
  printf("controllerSslEnabled: %s\n", controllerSslEnabled ? "true" : "false");
  printf("accountName: %s\n", accountName);
  printf("accountAccessKey: %s\n", accountAccessKey);
  printf("applicationName: %s\n", applicationName);
  printf("tierName: %s\n", tierName);
  printf("nodeName: %s\n", nodeName);

  appd_config *cfg = appd_config_init();

  appd_config_set_app_name(cfg, applicationName);
  appd_config_set_tier_name(cfg, tierName);
  appd_config_set_node_name(cfg, nodeName);
  appd_config_set_controller_host(cfg, controllerHostName);
  appd_config_set_controller_port(cfg, controllerPort);
  appd_config_set_controller_account(cfg, accountName);
  appd_config_set_controller_access_key(cfg, accountAccessKey);
  appd_config_set_controller_use_ssl(cfg, controllerSslEnabled);
  appd_config_set_logging_min_level(cfg, APPD_LOG_LEVEL_TRACE);
  appd_config_set_init_timeout_ms(cfg, 60000);

  // This calls initialized the agent
  // For some reason this seems to keep the process alive until we terminate it
  int initRC = appd_sdk_init(cfg);
  return scope.Close(Integer::New(initRC));
}

// Terminates the AppDynamics agent
Handle<Value> AppDTerminate(const Arguments &args)
{
  HandleScope scope;
  appd_sdk_term();

  return scope.Close(Integer::New(0));
}

Handle<Value> AppDStartBT(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8BTName(args[0]);
  const char *BTName = ToCString(v8BTName);

  String::Utf8Value v8CorrelationHeader(args[1]);
  const char *CorrelationHeader = ToCString(v8CorrelationHeader);

  appd_bt_handle btHandle = appd_bt_begin(BTName, CorrelationHeader);

  // This is an unique identifier for the BT
  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_bt_store(btHandle, guid);

  return scope.Close(String::New(guid));
}

Handle<Value> AppDEndBT(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8BTID(args[0]);
  const char *BTID = ToCString(v8BTID);

  appd_bt_handle btHandle = appd_bt_get(BTID);

  appd_bt_end(btHandle);

  return scope.Close(String::New(" "));
}

Handle<Value> AppDBackendDeclare(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8BackendType(args[0]);
  const char *backendType = ToCString(v8BackendType);

  String::Utf8Value v8BackendName(args[1]);
  const char *backendName = ToCString(v8BackendName);

  appd_backend_declare(backendType, backendName);

  return scope.Close(Integer::New(0));
}

Handle<Value> AppDBackendSetIdentifyingProperty(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8Backend(args[0]);
  const char *backend = ToCString(v8Backend);

  String::Utf8Value v8Property(args[1]);
  const char *property = ToCString(v8Property);

  String::Utf8Value v8Value(args[2]);
  const char *value = ToCString(v8Value);

  printf("%s, %s, %s\n", backend, property, value);

  int rc = appd_backend_set_identifying_property(backend, property, value);

  return scope.Close(Integer::New(rc));
}

Handle<Value> AppDBackendAdd(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8Backend(args[0]);
  const char *backend = ToCString(v8Backend);

  int rc = appd_backend_add(backend);

  return scope.Close(Integer::New(rc));
}

Handle<Value> AppDExitCallBegin(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8BTID(args[0]);
  const char *BTID = ToCString(v8BTID);

  String::Utf8Value v8Backend(args[1]);
  const char *backend = ToCString(v8Backend);

  appd_bt_handle bt = appd_bt_get(BTID);
  appd_exitcall_handle exitCall = appd_exitcall_begin(bt, backend);

  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_exitcall_store(exitCall, guid);

  return scope.Close(String::New(guid));
}

Handle<Value> AppDExitCallEnd(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8ExitCallID(args[0]);
  const char *exitCallID = ToCString(v8ExitCallID);

  appd_exitcall_handle exitCall = appd_exitcall_get(exitCallID);
  appd_exitcall_end(exitCall);

  return scope.Close(Integer::New(0));
}

// appd_exitcall_get_correlation_header

Handle<Value> AppDExitCallGetCorrelationHeader(const Arguments &args)
{
  HandleScope scope;

  String::Utf8Value v8ExitCallID(args[0]);
  const char *exitCallID = ToCString(v8ExitCallID);

  appd_exitcall_handle exitCall = appd_exitcall_get(exitCallID);
  const char *correlationHeader = appd_exitcall_get_correlation_header(exitCall);

  return scope.Close(String::New(correlationHeader));
}

Handle<Value> AppDFrameBegin(const Arguments &args)
{
  HandleScope scope;

  // APPD_API appd_frame_handle appd_frame_begin(appd_bt_handle bt, enum appd_frame_type frame_type,
  // const char* class_name, const char* method_name, const char* file, unsigned int line_number);

  String::Utf8Value v8btID(args[0]);
  const char *btID = ToCString(v8btID);

  String::Utf8Value v8FrameType(args[1]);
  const char *frameType = ToCString(v8FrameType);

  String::Utf8Value v8ClassName(args[2]);
  const char *className = ToCString(v8ClassName);

  String::Utf8Value v8MethodName(args[3]);
  const char *methodName = ToCString(v8MethodName);

  String::Utf8Value v8File(args[4]);
  const char *file = ToCString(v8File);

  int lineNumber = args[5]->NumberValue();

  appd_bt_handle btHandle = appd_bt_get(btID);

  appd_frame_handle frameHandle = appd_frame_begin(btHandle, APPD_FRAME_TYPE_CPP, className, methodName, file, lineNumber);

  return scope.Close(String::New(""));
}

// Register Methods
void Initialize(Local<Object> exports)
{

  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "profile", AppDProfile);
  NODE_SET_METHOD(exports, "terminate", AppDTerminate);
  NODE_SET_METHOD(exports, "start_bt", AppDStartBT);
  NODE_SET_METHOD(exports, "end_bt", AppDEndBT);
  NODE_SET_METHOD(exports, "appd_backend_declare", AppDBackendDeclare);
  NODE_SET_METHOD(exports, "appd_backend_set_identifying_property", AppDBackendSetIdentifyingProperty);
  NODE_SET_METHOD(exports, "appd_backend_add", AppDBackendAdd);
  NODE_SET_METHOD(exports, "appd_exitcall_begin", AppDExitCallBegin);
  NODE_SET_METHOD(exports, "appd_exitcall_end", AppDExitCallEnd);
  NODE_SET_METHOD(exports, "appd_exitcall_get_correlation_header", AppDExitCallGetCorrelationHeader);
}

NODE_MODULE(appd_wrapper_agent, Initialize)

/*
#include <napi.h>
#include <appdynamics.h>

Napi::String printHello(const Napi::CallbackInfo &args)
{
  Napi::Env env = args.Env();
  return Napi::String::New(env, "world!");
}

// Initializes the agent
// Must receive:
// controllerHostName, controllerPort, controllerSslEnabled, accountName, accountAccessKey, applicationName, tierName, nodeName
Napi::Number appd_profile(const Napi::CallbackInfo &args)
{
  Napi::Env env = args.Env();

  appd_config *cfg = appd_config_init();

  const char *bufferData = args[0].ToString().Utf8Value().c_str();

  printf("Param 0: %s\n", bufferData);

  return Napi::Number::New(env, 0);
}

// Init exports all methods
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, printHello));

  exports.Set(Napi::String::New(env, "appd_profile"),
              Napi::Function::New(env, appd_profile));

  return exports;
}

NODE_API_MODULE(appdagent, Init)
*/