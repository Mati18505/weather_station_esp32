#include <unity.h>
#include <string>
#include <weather.h>

bool str_has_substr(std::string_view str, std::string_view substr) {
  return str.find(substr) != std::string_view::npos;
}

void test_serialize_weather_basic() {
  weather::Weather w{21.5f, 60, weather::WeatherCode::Clear_Sky};
  int expected_weather_code = static_cast<int>(weather::WeatherCode::Clear_Sky);

  std::string json = weather::serialize_weather(w);

  TEST_ASSERT_FALSE(json.empty());

  TEST_ASSERT_TRUE(str_has_substr(json, "\"temperature\":21.5"));
  TEST_ASSERT_TRUE(str_has_substr(json, "\"humidity\":60"));
  TEST_ASSERT_TRUE(str_has_substr(
    json, "\"weather_code\":" + std::to_string(expected_weather_code)));
}

void test_serialize_weather_empty_desc() {
  weather::Weather w{10.0f, 50, weather::WeatherCode::None};
  int expected_weather_code = static_cast<int>(weather::WeatherCode::None);

  std::string json = weather::serialize_weather(w);

  TEST_ASSERT_FALSE(json.empty());
  TEST_ASSERT_TRUE(str_has_substr(json, "\"weather_code\":0"));
  TEST_ASSERT_TRUE(str_has_substr(
    json, "\"weather_code\":" + std::to_string(expected_weather_code)));
}

void test_serialize_weather_negative_values() {
  weather::Weather w{-5.0f, -1, weather::WeatherCode::Drizzle_LightRain};
  int expected_weather_code =
    static_cast<int>(weather::WeatherCode::Drizzle_LightRain);

  std::string json = weather::serialize_weather(w);

  TEST_ASSERT_FALSE(json.empty());
  TEST_ASSERT_TRUE(str_has_substr(json, "\"temperature\":-5"));
  TEST_ASSERT_TRUE(str_has_substr(json, "\"humidity\":-1"));
  TEST_ASSERT_TRUE(str_has_substr(
    json, "\"weather_code\":" + std::to_string(expected_weather_code)));
}

void test_serialize_weather() {
  RUN_TEST(test_serialize_weather_basic);
  RUN_TEST(test_serialize_weather_empty_desc);
  RUN_TEST(test_serialize_weather_negative_values);
}
