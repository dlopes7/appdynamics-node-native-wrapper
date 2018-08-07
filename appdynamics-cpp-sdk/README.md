AppDynamics C/C++ SDK
=====================

Welcome to the AppDynamics C/C++ SDK. Please visit [the AppDynamics documentation site](https://docs.appdynamics.com) for up-to-date documentation on the SDK.

This distribution contains:

* LICENSE -- licensing information for this software
* VERSION -- information on the version of this software
* README.md -- this file
* include/ -- contains the SDK headers
    * appdynamics.h -- the public API for using the C/C++ SDK
    * appdynamics_advanced.h -- the public API for using advanced C/C++ SDK functionality
* lib/ -- directory containing shared object files for the SDK

## Required linking arguments

The pthread, dynamic linking, and realtime libraries are required in addition to AppDynamics:

    -lappdynamics -lpthread -ldl -lrt
