#include "../../../../include/prettyurl/core/utility/string_utils.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace prettyurl;

using namespace std::literals;

SCENARIO("string transform to lower") {
  std::string s = "ABCDEfGh"; 
  core::utility::to_lower(s);

  REQUIRE(s == "abcdefgh");
}

SCENARIO("string transfowm to upper") {
  std::string s = "abcdEFGh";
  core::utility::to_upper(s);

  REQUIRE(s == "ABCDEFGH");
}

SCENARIO("convert to string") {
  const auto res1 = core::utility::to_string(1);
  const auto res2 = core::utility::to_string(9.123);
  const auto res3 = core::utility::to_string('A');
  const auto res4 = core::utility::to_string("str");
  const auto res5 = core::utility::to_string("str"s);
  const auto res6 = core::utility::to_string("str"sv);

  REQUIRE(res1 == "1");
  REQUIRE(res2 == "9.123000");
  REQUIRE(res3 == "A");
  REQUIRE(res4 == "str");
  REQUIRE(res5 == "str");
  REQUIRE(res6 == "str");
}

SCENARIO("concating values to string") {
  const auto res1 = core::utility::concat("Hello", "World", 1234, '!');
  const auto res2 = core::utility::concat(1, 2, 3, 4, 5);
  const auto res3 = core::utility::concat(1.213, 1, "str");
  const auto res4 = core::utility::concat('1', '2', '3', '4', '5');

  REQUIRE(res1 == "Hello World 1234 !");
  REQUIRE(res2 == "1 2 3 4 5");
  REQUIRE(res3 == "1.213000 1 str");
  REQUIRE(res4 == "1 2 3 4 5");
}

SCENARIO("convert string to a value") {
  const auto res1 = core::utility::string_to<float>("1.2345");
  const auto res2 = core::utility::string_to<int>("11223344");
  const auto res3 = core::utility::string_to<char>("A");
  const auto res4 = core::utility::string_to<bool>("true");
  const auto res5 = core::utility::string_to<std::string>("string");

  REQUIRE(res1 == 1.2345f);
  REQUIRE(res2 == 11223344);
  REQUIRE(res3 == 'A');
  REQUIRE(res4 == true);
  REQUIRE(res5 == "string");
}