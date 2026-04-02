#include <unity.h>

#include "test_retry_until_success.h"
#include "test_parse_weather_json.h"
#include "test_serialize_weather.h"

int main() {
  UNITY_BEGIN();
  test_retry_until_success();
  test_parse_weather_json();
  test_serialize_weather();
  return UNITY_END();
}
