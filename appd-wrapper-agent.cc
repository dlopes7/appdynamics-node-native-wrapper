#include <node.h>
#include <appdynamics.h>

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

  return scope.Close(String::New(""));
}

// Register Methods
void Init(Handle<Object> exports)
{
  exports->Set(String::NewSymbol("hello"),
               FunctionTemplate::New(Method)->GetFunction());

  exports->Set(String::NewSymbol("profile"),
               FunctionTemplate::New(AppDProfile)->GetFunction());
}

NODE_MODULE(hello, Init)

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