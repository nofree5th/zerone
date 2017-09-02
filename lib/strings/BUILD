cc_library(
    name = "strings",
    srcs = ["strings.cpp"],
    hdrs = ["strings.h"],
    deps = [
        "//third_party/boost:algorithm",
        "//third_party/pbjson",
    ],
)

cc_test(
    name = "test_strings",
    srcs = ["test_strings.cpp"],
    deps = [
        "//lib/strings",
        "//third_party/gtest:gtest_main",
    ],
)

# test pbjson
cc_test(
    name = "test_pbjson",
    srcs = ["test_pbjson.cpp"],
    deps = [
        ":cc_test_pbjson_proto",
        "//lib/strings",
        "//third_party/gtest:gtest_main",
    ],
)

cc_proto_library(
    name = "cc_test_pbjson_proto",
    deps = [":test_pbjson_proto"],
)

proto_library(
    name = "test_pbjson_proto",
    srcs = ["test_pbjson.proto"],
)
