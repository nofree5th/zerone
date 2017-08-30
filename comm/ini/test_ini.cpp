#include <gtest/gtest.h>
#include "comm/ini/ini.h"

TEST(IniParser, ReadFromFile)
{
    dry::ini::IniParser parser;
    ASSERT_NE(0, parser.InitFromFile("not_exist"));

    ASSERT_EQ(0, parser.InitFromFile("ini.conf"));
}
