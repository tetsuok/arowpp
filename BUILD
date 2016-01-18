COPTS = [
    "-Wcast-align",
    "-Wcast-qual",
    "-Wconversion",
    "-Wextra",
    "-Wfloat-equal",
    "-Woverloaded-virtual",
    "-Wparentheses",
    "-Wpointer-arith",
    "-Wswitch-enum",
    "-Wwrite-strings",
]

cc_library(
    name = "arowpp",
    srcs = [
        "src/classifier.cc",
        "src/feature.cc",
        "src/libarowpp.cc",
        "src/model_reader.cc",
        "src/model_writer.cc",
        "src/options.cc",
        "src/result.cc",
        "src/tokenizer.cc",
    ],
    hdrs = [
        "src/arowpp.h",
        "src/common.h",
        "src/error_handler.h",
        "src/feature.h",
        "src/logging.h",
        "src/model_reader_interface.h",
        "src/model_writer_interface.h",
        "src/options.h",
        "src/param.h",
        "src/random.h",
        "src/timer.h",
        "src/tokenizer.h",
    ],
    copts = COPTS,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "arow_learn",
    srcs = [
        "src/arow_learn.cc",
    ],
    deps = [
        ":arowpp",
    ],
)

cc_binary(
    name = "arow_test",
    srcs = [
        "src/arow_test.cc",
    ],
    deps = [
        ":arowpp",
    ],
)

cc_test(
    name = "arow_unittest",
    srcs = [
        "src/classifier_test.cc",
        "src/feature_test.cc",
        "src/model_reader_test.cc",
        "src/result_test.cc",
        "src/tokenizer_test.cc",
    ],
    copts = COPTS,
    linkopts = ["-lpthread"],
    deps = [
        ":arowpp",
        "//external:gtest_main",
    ]
)
