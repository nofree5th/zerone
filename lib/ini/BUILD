cc_library(
    name = "ini",
    srcs = ["ini.cpp"],
    hdrs = ["ini.h"],
    deps = [
        "//lib/log",
        "//third_party/boost:algorithm",
        "//third_party/boost:smart_ptr",
    ],
)

cc_test(
    name = "test_ini",
    srcs = ["test_ini.cpp"],
    deps = [
        ":ini",
        "//third_party/gtest:gtest_main",
    ],
)
