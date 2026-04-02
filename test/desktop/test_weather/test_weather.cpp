#include <unity.h>

#include "test_retry_until_success.h"
#include "test_parse_weather_json.h"
#include "test_serialize_weather.h"
#include "test_build_url.h"

int main() {
  UNITY_BEGIN();
  test_retry_until_success();
  test_parse_weather_json();
  test_serialize_weather();
  test_build_url();
  return UNITY_END();
}
