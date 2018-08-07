#include <node.h>
#include <appdynamics.h>

using namespace v8;

// Hello World Sample
Handle<Value> Method(const Arguments &args)
{
  HandleScope scope;
  return scope.Close(String::New("world"));
}

Handle<Value> AppDProfile(const Arguments &args)
{
  HandleScope scope;

  v8::String::Utf8Value controller_host(args[0]->ToString());
  std::string controller = std::string(*controller_host);

  printf("%s", controller.c_str());

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