#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>

TEST(String, Trim)
{
    ASSERT_EQ(boost::trim_left_copy(std::string("  \t\r\n   hello")), "hello");
}
