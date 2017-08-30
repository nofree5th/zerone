#include <gtest/gtest.h>
#include "comm/ini/ini.h"

TEST(IniParser, ReadFromFile)
{
    dry::ini::IniParser parser;
    ASSERT_NE(0, parser.InitFromFile("not_exist"));

    {
        const auto content = R"__(

# unsectioned
unsection_key = value1

[root]
k1 = v1
k2 =   v 2   

# will overwrite before
k2 =   v 3   
# unclosed seciton ignored
[vad section

# bad key-value
bad_key
dblist = root;admin

[db]
ip = 127.0.0.1
  port = 3306
  empty_key = 

# invalid key
 = invalid value

# value include "=", key include " "
special key  = value include = ^_^

)__";
        ASSERT_EQ(0, parser.InitFromBuffer(content));

        // unsection
        ASSERT_EQ("value1", parser.GetValue("", "unsection_key"));

        ASSERT_EQ("v1", parser.GetValue("root", "k1"));

        // last priority
        ASSERT_EQ("v 3", parser.GetValue("root", "k2"));

        ASSERT_EQ("root;admin", parser.GetValue("root", "dblist"));

        // not exist
        ASSERT_EQ("", parser.GetValue("not exist section", "not exist key"));

        // value include =
        ASSERT_EQ("value include = ^_^", parser.GetValue("db", "special key"));
    }
}
