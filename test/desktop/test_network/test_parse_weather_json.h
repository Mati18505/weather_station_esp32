#include <unity.h>
#include <string>
#include <network.h>

void test_parse_weather_minimal_valid_json() {
  std::string json = R"({
    "main": { "temp": 21.5, "humidity": 60 },
    "weather": [ { "description": "clear sky" } ]
    })";

  auto result = parse_weather_json(json);

  TEST_ASSERT_TRUE(result.has_value());
  TEST_ASSERT_EQUAL_FLOAT(21.5, result->temperature);
  TEST_ASSERT_EQUAL_INT(60, result->humidity);
  TEST_ASSERT_EQUAL_STRING("clear sky", result->desc.c_str());
}

void test_parse_weather_invalid_json() {
  std::string json = "invalid json";

  auto result = parse_weather_json(json);

  TEST_ASSERT_FALSE(result.has_value());
}

void test_parse_weather_empty_json() {
  std::string json = R"({ })";

  auto result = parse_weather_json(json);

  TEST_ASSERT_FALSE(result.has_value());
}

void test_parse_weather_missing_fields() {
  std::string json = R"({ "main": {}, "weather": [{}] })";

  auto result = parse_weather_json(json);

  TEST_ASSERT_FALSE(result.has_value());
}

void test_parse_weather_empty_desc() {
  std::string json = R"({
    "main": { "temp": 21.5, "humidity": 60 },
    "weather": [ { "description": "" } ]
    })";

  auto result = parse_weather_json(json);

  TEST_ASSERT_TRUE(result.has_value());
  TEST_ASSERT_EQUAL_FLOAT(21.5, result->temperature);
  TEST_ASSERT_EQUAL_INT(60, result->humidity);
  TEST_ASSERT_EQUAL_STRING("", result->desc.c_str());
}

void test_parse_weather_real_valid_json() {
  // Example response from openweather.
  std::string json = R"(
    {
      "main": {
        "temp": 25.3,
        "humidity": 65,
        "feels_like": 26.1
      },
      "name": "Tokyo",
      "wind": {
        "deg": 120,
        "speed": 3.6
      },
      "coord": {
        "lat": 35.6895,
        "lon": 139.6917
      },
      "weather": [
        {
          "id": 800,
          "main": "Clear",
          "description": "clear sky"
        }
      ],
      "visibility": 10000
    }
  )";

  auto result = parse_weather_json(json);

  TEST_ASSERT_TRUE(result.has_value());
  TEST_ASSERT_EQUAL_FLOAT(25.3, result->temperature);
  TEST_ASSERT_EQUAL_INT(65, result->humidity);
  TEST_ASSERT_EQUAL_STRING("clear sky", result->desc.c_str());
}

void test_parse_weather_json() {
  RUN_TEST(test_parse_weather_minimal_valid_json);
  RUN_TEST(test_parse_weather_empty_json);
  RUN_TEST(test_parse_weather_invalid_json);
  RUN_TEST(test_parse_weather_missing_fields);
  RUN_TEST(test_parse_weather_real_valid_json);
}
