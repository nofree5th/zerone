cc_library(
    name = "url",
    srcs = [
        "url.cpp",
    ],
    hdrs = [
        "url.h",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//lib/log",
        "//third_party/http-parser",
    ],
)

cc_test(
    name = "test_url",
    srcs = [
        "test_url.cpp",
    ],
    deps = [
        ":url",
        "//third_party/gtest:gtest_main",
    ],
)
