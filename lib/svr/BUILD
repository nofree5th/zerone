cc_library(
    name = "svr",
    srcs = [
        "server.cpp",
        "server_options.h",
        "server_options.cpp",

        # process unit
        "server_thread.cpp",
        "server_thread.h",

        # acceptor
        "acceptor.h",
        "acceptor.cpp",

        # process all connections
        "worker.h",
        "worker.cpp",

        # one connection handler
        "connection_handler.cpp",
        "connection_handler.h",
    ],
    hdrs = [
        "server.h",
    ],
    visibility = [
        "//services:__subpackages__",
    ],
    deps = [
        "//lib:core",
        "//lib/net",
        "//third_party/boost:core",
        "//third_party/libco",
    ],
)

cc_binary(
    name = "test_svr",
    srcs = [
        "test_svr.cpp",
    ],
    deps = [
        ":svr",
    ],
)
