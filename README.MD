# AppDynamics Node Native Wrapper

This is a Node.JS Native module using the AppDynamics CPP SDK for Windows.

This is for apps running Node 0.10, on Windows 32 bits, where the Node.JS Agent is not compatible.

To compile this, you will need:

1. node 0.10 32 bits (`nvm install 0.10.46 32`)
2. node-gyp (`npm install -g node-gyp`)
3. Microsoft C++ Redist 2.0, This comes with Visual Studio 2013

To compile:

1. `node-gyp rebuild`
