#include <gtest/gtest.h>
#include "lib/net/url/url.h"

TEST(Url, TestHttpProto)
{
    dry::net::url::Url url;
    std::string schema("https");
    std::string host("www.google.com");
    std::string path("/path/to/page.html");
    std::string rawQuery("k1=v1&k2=v2&only_key");
    std::string frag("wechat_redirect");
    ASSERT_EQ(0, url.ParseFromString(schema + "://" + host + ":8080" + path + "?" + rawQuery + "#" + frag));
    ASSERT_EQ(schema, url.schema);
    ASSERT_EQ(host, url.host);
    ASSERT_EQ(8080, url.port);
    ASSERT_EQ(path, url.path);
    ASSERT_EQ(rawQuery, url.rawQuery);
    ASSERT_EQ(frag, url.fragment);
}
