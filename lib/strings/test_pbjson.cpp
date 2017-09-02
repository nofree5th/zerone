#include "gtest/gtest.h"
#include "lib/strings/strings.h"
#include "lib/strings/test_pbjson.pb.h"

using namespace test_pbjson;

TEST(PbJson, ToString)
{
    // pb2json
    {
        Item item;
        item.set_u32(1234567890);
        item.set_i32(-1234567890);
        item.set_str("hellos str");
        item.set_d(3.14);

        Wrap wrap;
        wrap.set_id(123);

        for (auto i = 0; i < 10; ++i) wrap.add_id_list(100 + i);

        for (auto i = 0; i < 3; ++i)
        {
            wrap.add_item_list()->CopyFrom(item);
        }

        std::string result;
        pbjson::pb2json(&item, result);
        printf("Item Result: \n%s\n", result.c_str());

        result.clear();
        pbjson::pb2json(&wrap, result);
        printf("Wrap Result: \n%s\n", result.c_str());
    }
}

TEST(PbJson, FromString)
{
    // normal json2pb
    {
        std::string data(
            R"__({"id":123,"id_list":[100,101,102,103,104,105,106,107,108,109],
            "item_list":[
            {"u32":1234567890,"i32":-1234567890,"str":"hellos str","d":3.14,"u64":123},
            {"u32":1234567890,"i32":-1234567890,"str":"hellos str","d":3},
            {"u32":1234,"i32":-12345,"str":"hellos str","d":3.14,"i64":-1}]
            })__");
        Wrap wrap;
        std::string err;
        const int ret = pbjson::json2pb(data, &wrap, err);
        ASSERT_EQ(0, ret);
        printf("Parsed result: %d, err: %s\n", ret, err.c_str());
        printf("%s\n", wrap.Utf8DebugString().c_str());
    }

    // bad
    {
        std::string data(
            R"__({"id":-123,"id_list":[100,101,102,103,104,105,106,107,108,109],
            "item_list":[
            {"u32":1234567890,"i32":-1234567890,"str":"hellos str","d":3.14},
            {"u32":1234567890,"i32":-1234567890,"str":"hellos str","d":3.14},
            {"u32":1234567890,"i32":-1234567890,"str":"hellos str","d":3.14}]
            })__");
        Wrap wrap;
        std::string err;
        const int ret = pbjson::json2pb(data, &wrap, err);
        ASSERT_NE(0, ret);
        printf("Parsed result: %d, err: %s\n", ret, err.c_str());
        printf("%s\n", wrap.Utf8DebugString().c_str());
    }
}
