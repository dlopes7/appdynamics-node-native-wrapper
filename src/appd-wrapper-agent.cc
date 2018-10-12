#include <node.h>
#include <time.h>
#include <appdynamics.h>
#include <windows.h>
#include <nan.h>

using namespace v8;

using Nan::GetFunction;
using Nan::New;
using Nan::Set;


const char *NanToCString(v8::Local<v8::Value> &value){
  Nan::Utf8String val(value);
  return *val ? *val : "<string conversion failed>";
}

// AppDProfile will receive parameters:
// controllerHostName, controllerPort, controllerSslEnabled, accountName, accountAccessKey, applicationName, tierName, nodeName
NAN_METHOD(AppDProfile)
{
  srand(time(NULL));
  
  const char *controllerHostName = NanToCString(info[0]);
  int controllerPort = info[1]->NumberValue();
  bool controllerSslEnabled = info[2]->BooleanValue();
  const char *accountName = NanToCString(info[3]);;
  const char *accountAccessKey = NanToCString(info[4]);
  const char *applicationName = NanToCString(info[5]);
  const char *tierName =  NanToCString(info[6]);
  const char *nodeName =  NanToCString(info[7]);
  int logLevel = info[8]->NumberValue();
  const char *logDirectory = NanToCString(info[9]);

  appd_config *cfg = appd_config_init();

  appd_config_set_app_name(cfg, applicationName);
  appd_config_set_tier_name(cfg, tierName);
  appd_config_set_node_name(cfg, nodeName);
  appd_config_set_controller_host(cfg, controllerHostName);
  appd_config_set_controller_port(cfg, controllerPort);
  appd_config_set_controller_account(cfg, accountName);
  appd_config_set_controller_access_key(cfg, accountAccessKey);
  appd_config_set_controller_use_ssl(cfg, controllerSslEnabled);
  appd_config_set_logging_min_level(cfg, appd_config_log_level(logLevel));
  appd_config_set_logging_log_dir(cfg, logDirectory);

  // This calls initializes the agent
  printf("APPDYNAMICS - Initializing the agent asynchronously...\n");
  int initRC = appd_sdk_init(cfg);
}

// Terminates the AppDynamics agent
NAN_METHOD(AppDTerminate)
{
  printf("APPDYNAMICS - Terminating the agent...\n");
  appd_sdk_term();
  info.GetReturnValue().Set(0);
}

NAN_METHOD(AppDStartBT)
{

  const char *BTName = NanToCString(info[0]);
  const char *CorrelationHeader =NanToCString(info[1]);

  appd_bt_handle btHandle = appd_bt_begin(BTName, CorrelationHeader);

  // This is an unique identifier for the BT
  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_bt_store(btHandle, guid);

  info.GetReturnValue().Set(String::New(guid));
}

NAN_METHOD(AppDEndBT)
{

  const char *BTID = NanToCString(info[0]);
  appd_bt_handle btHandle = appd_bt_get(BTID);
  appd_bt_end(btHandle);

  info.GetReturnValue().Set(String::New(""));
}

NAN_METHOD(AppDBackendDeclare)
{

  const char *backendType = NanToCString(info[0]);
  const char *backendName = NanToCString(info[1]);

  appd_backend_declare(backendType, backendName);

  info.GetReturnValue().Set(Integer::New(0));
}

NAN_METHOD(AppDBackendSetIdentifyingProperty)
{

  const char *backend = NanToCString(info[0]);
  const char *property = NanToCString(info[1]);
  const char *value = NanToCString(info[2]);

  printf("%s, %s, %s\n", backend, property, value);

  int rc = appd_backend_set_identifying_property(backend, property, value);

  info.GetReturnValue().Set(Integer::New(rc));
}

NAN_METHOD(AppDBackendAdd)
{

  const char *backend = NanToCString(info[0]);
  int rc = appd_backend_add(backend);

  info.GetReturnValue().Set(Integer::New(rc));
}

NAN_METHOD(AppDExitCallBegin)
{

  const char *BTID = NanToCString(info[0]);
  const char *backend = NanToCString(info[1]);

  appd_bt_handle bt = appd_bt_get(BTID);
  appd_exitcall_handle exitCall = appd_exitcall_begin(bt, backend);

  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_exitcall_store(exitCall, guid);

  info.GetReturnValue().Set(String::New(guid));
}

NAN_METHOD(AppDExitCallEnd)
{

  const char *exitCallID = NanToCString(info[0]);

  appd_exitcall_handle exitCall = appd_exitcall_get(exitCallID);
  appd_exitcall_end(exitCall);

  info.GetReturnValue().Set(Integer::New(0));
}

// appd_exitcall_get_correlation_header
NAN_METHOD(AppDExitCallGetCorrelationHeader)
{

  const char *exitCallID = NanToCString(info[0]);

  appd_exitcall_handle exitCall = appd_exitcall_get(exitCallID);
  const char *correlationHeader = appd_exitcall_get_correlation_header(exitCall);

  info.GetReturnValue().Set(String::New(correlationHeader));
}

NAN_METHOD(AppDFrameBegin)
{

  const char *btID = NanToCString(info[0]);
  const char *frameType = NanToCString(info[1]);
  const char *className = NanToCString(info[2]);
  const char *methodName = NanToCString(info[3]);
  const char *file = NanToCString(info[4]);

  int lineNumber = info[5]->NumberValue();

  appd_bt_handle btHandle = appd_bt_get(btID);

  appd_frame_handle frameHandle = appd_frame_begin(btHandle, APPD_FRAME_TYPE_CPP, className, methodName, file, lineNumber);

  info.GetReturnValue().Set(String::New(""));
}

// Register Methods
NAN_MODULE_INIT(initAll){
  Set(target, New<String>("profile").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDProfile)).ToLocalChecked());

  Set(target, New<String>("terminate").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDTerminate)).ToLocalChecked());

  Set(target, New<String>("start_bt").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDStartBT)).ToLocalChecked());
  
  Set(target, New<String>("end_bt").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDEndBT)).ToLocalChecked());

  Set(target, New<String>("appd_backend_declare").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDBackendDeclare)).ToLocalChecked());

  Set(target, New<String>("appd_backend_set_identifying_property").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDBackendSetIdentifyingProperty)).ToLocalChecked());

  Set(target, New<String>("appd_backend_add").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDBackendAdd)).ToLocalChecked());

  Set(target, New<String>("appd_exitcall_begin").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDExitCallBegin)).ToLocalChecked());

  Set(target, New<String>("appd_exitcall_end").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDExitCallEnd)).ToLocalChecked());  

  Set(target, New<String>("appd_exitcall_get_correlation_header").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(AppDExitCallGetCorrelationHeader)).ToLocalChecked());                      
}

// NODE_MODULE(appd_wrapper_agent, Initialize)
NODE_MODULE(appd_wrapper_agent, initAll)
