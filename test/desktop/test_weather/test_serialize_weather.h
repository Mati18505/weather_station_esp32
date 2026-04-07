#include <unity.h>
#include <string>
#include <weather.h>

bool str_has_substr(std::string_view str, std::string_view substr) {
   return str.find(substr) != std::string_view::npos;
}

void test_serialize_weather_basic() {
  Weather w{21.5f, 60, "clear sky"};

  std::string json = weather::serialize_weather(w);

  TEST_ASSERT_FALSE(json.empty());

  TEST_ASSERT_TRUE(str_has_substr(json, "\"temperature\":21.5"));
  TEST_ASSERT_TRUE(str_has_substr(json, "\"humidity\":60"));
  TEST_ASSERT_TRUE(str_has_substr(json, "\"desc\":\"clear sky\""));
}

void test_serialize_weather_empty_desc() {
    Weather w{10.0f, 50, ""};

    std::string json = weather::serialize_weather(w);

    TEST_ASSERT_FALSE(json.empty());
    TEST_ASSERT_TRUE(str_has_substr(json, "\"desc\":\"\""));
}

void test_serialize_weather_negative_values() {
    Weather w{-5.0f, -1, "cold"};

    std::string json = weather::serialize_weather(w);

    TEST_ASSERT_FALSE(json.empty());
    TEST_ASSERT_NOT_EQUAL(nullptr, strstr(json.c_str(), "\"temperature\":-5"));
    TEST_ASSERT_NOT_EQUAL(nullptr, strstr(json.c_str(), "\"humidity\":-1"));
}

void test_serialize_weather() {
  RUN_TEST(test_serialize_weather_basic);
  RUN_TEST(test_serialize_weather_empty_desc);
  RUN_TEST(test_serialize_weather_negative_values);
}
