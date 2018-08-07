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
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
            "link_settings": {
                "libraries": [
                    "/Users/dlopes/Documents/projects/node/appdynamics-node-native-agent/appdynamics-cpp-sdk/lib/libappdynamics.so",
                ]}
        }
    ]
}
