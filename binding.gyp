{
    "targets": [
        {
            "target_name": "hello",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": ["appd-wrapper-agent.cc"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "./appdynamics-cpp-sdk/include"
            ],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "link_settings": {
                "libraries": [
                    "C:/project/node/appdynamics-node-native-wrapper/appdynamics-cpp-sdk/lib/appdynamics.lib",
                ]
            }
        },
         {
         "target_name": "copy_binary",
         "type":"none",
         "dependencies" : [ "hello" ],
         "copies":
         [
            {
               'destination': '<(module_root_dir)/build/Release/',
               'files': ['<(module_root_dir)/appdynamics-cpp-sdk/lib/appdynamics.dll']
            }
         ]
      }
    ]
}
