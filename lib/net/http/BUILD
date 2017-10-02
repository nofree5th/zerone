cc_library(
    name = "http",
    srcs = [
        "header.cpp",
        "request.cpp",
        "response.cpp",
    ],
    hdrs = [
        "header.h",
        "request.h",
        "response.h",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "//lib/log",
        "//lib/net/url",
        "//third_party/curl",
    ],
)
