cc_library(
    name = "log",
    srcs = [
        "logger.cpp",
        "logger_helper.h",
    ],
    hdrs = [
        "logger.h",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//lib/error",
        "//third_party/glog",
    ],
)

cc_binary(
    name = "test_logger",
    srcs = [
        "test_logger.cpp",
    ],
    deps = [
        "//lib:core",
    ],
)
