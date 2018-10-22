#include <node.h>
#include <time.h>
#include <appdynamics.h>
#include <windows.h>
#include <nan.h>

using namespace v8;

using Nan::GetFunction;
using Nan::New;
using Nan::Set;

const char *NanToCString(v8::Local<v8::Value> &value)
{
  Nan::Utf8String val(value);
  return *val ? *val : "<string conversion failed>";
}

// AppDProfile will receive parameters:
// controllerHostName, controllerPort, controllerSslEnabled, accountName, accountAccessKey, applicationName, tierName, nodeName
NAN_METHOD(AppDProfile)
{
  srand(time(NULL));

  appd_config *cfg = appd_config_init();

  // Controller settings
  appd_config_set_controller_host(cfg, NanToCString(info[0]));
  appd_config_set_controller_port(cfg, info[1]->NumberValue());
  appd_config_set_controller_use_ssl(cfg, info[2]->BooleanValue());
  appd_config_set_controller_account(cfg, NanToCString(info[3]));
  appd_config_set_controller_access_key(cfg, NanToCString(info[4]));

  // App Model settings
  appd_config_set_app_name(cfg, NanToCString(info[5]));
  appd_config_set_tier_name(cfg, NanToCString(info[6]));
  appd_config_set_node_name(cfg, NanToCString(info[7]));

  // Log settings
  appd_config_set_logging_min_level(cfg, appd_config_log_level(int(info[8]->NumberValue())));
  appd_config_set_logging_log_dir(cfg, NanToCString(info[9]));

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

  appd_bt_handle btHandle = appd_bt_begin(NanToCString(info[0]), NanToCString(info[1]));

  // This is an unique identifier for the BT
  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_bt_store(btHandle, guid);

  info.GetReturnValue().Set(String::New(guid));
}

NAN_METHOD(AppDEndBT)
{

  appd_bt_handle btHandle = appd_bt_get(NanToCString(info[0]));
  appd_bt_end(btHandle);

  info.GetReturnValue().Set(String::New(""));
}

NAN_METHOD(AppDBackendDeclare)
{

  appd_backend_declare(NanToCString(info[0]), NanToCString(info[1]));

  info.GetReturnValue().Set(Integer::New(0));
}

NAN_METHOD(AppDBackendSetIdentifyingProperty)
{

  int rc = appd_backend_set_identifying_property(NanToCString(info[0]), NanToCString(info[1]), NanToCString(info[2]));

  info.GetReturnValue().Set(Integer::New(rc));
}

NAN_METHOD(AppDBackendAdd)
{

  int rc = appd_backend_add(NanToCString(info[0]));

  info.GetReturnValue().Set(Integer::New(rc));
}

NAN_METHOD(AppDExitCallBegin)
{

  appd_bt_handle bt = appd_bt_get(NanToCString(info[0]));
  appd_exitcall_handle exitCall = appd_exitcall_begin(bt, NanToCString(info[1]));

  int num = rand();
  char guid[20];
  sprintf(guid, "%d", num);

  appd_exitcall_store(exitCall, guid);

  info.GetReturnValue().Set(String::New(guid));
}

NAN_METHOD(AppDExitCallEnd)
{

  appd_exitcall_handle exitCall = appd_exitcall_get(NanToCString(info[0]));
  appd_exitcall_end(exitCall);

  info.GetReturnValue().Set(Integer::New(0));
}

// appd_exitcall_get_correlation_header
NAN_METHOD(AppDExitCallGetCorrelationHeader)
{

  appd_exitcall_handle exitCall = appd_exitcall_get(NanToCString(info[0]));
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

// Adds a Detail string to an Exit Call
NAN_METHOD(AppDExitCallSetDetails)
{

  appd_exitcall_handle exitCall = appd_exitcall_get(NanToCString(info[0]));
  int retCode = appd_exitcall_set_details(exitCall, NanToCString(info[1]));

  info.GetReturnValue().Set(Integer::New(retCode));
}

// Adds a Detail string to an Exit Call
NAN_METHOD(AppDBTAddError)
{

  appd_bt_handle bt = appd_bt_get(NanToCString(info[0]));

  // printf("Adding error %d %s %d\n", appd_error_level(int(info[1]->NumberValue())), NanToCString(info[2]), int(info[3]->NumberValue()));
  appd_bt_add_error(bt, appd_error_level(int(info[1]->NumberValue())), NanToCString(info[2]), int(info[3]->NumberValue()));

  info.GetReturnValue().Set(Integer::New(0));
}

// Register Methods
NAN_MODULE_INIT(initAll)
{
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

  Set(target, New<String>("appd_exitcall_set_details").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(AppDExitCallSetDetails)).ToLocalChecked());

  Set(target, New<String>("appd_bt_add_error").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(AppDBTAddError)).ToLocalChecked());
}

// NODE_MODULE(appd_wrapper_agent, Initialize)
NODE_MODULE(appd_wrapper_agent, initAll)
