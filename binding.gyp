{
    "targets": [
        {
            "target_name": "appd_wrapper_agent",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": ["src/appd-wrapper-agent.cc"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "./appdynamics-cpp-sdk/include",
                "<!(node -e \"require('nan')\")"
            ],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "link_settings": {
                "libraries": [
                    "<(module_root_dir)/appdynamics-cpp-sdk/lib/appdynamics.lib",
                ]
            }
        },
         {
         "target_name": "copy_binary",
         "type":"none",
         "dependencies" : [ "appd_wrapper_agent" ],
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
