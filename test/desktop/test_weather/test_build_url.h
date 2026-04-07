#include <unity.h>
#include <string>
#include <weather.h>
#include <string_view>

void test_build_weather_url_basic() {
    std::string url = weather::build_weather_url(
        "http://api.test/weather?q=",
        "Gdansk",
        "XYZ123"
    );

    TEST_ASSERT_EQUAL_STRING(
        "http://api.test/weather?q=Gdansk&appid=XYZ123&units=metric",
        url.c_str()
    );
}

void test_build_weather_url_empty_city() {
    std::string url = weather::build_weather_url(
        "http://api.test/weather?q=",
        "",
        "XYZ123"
    );

    TEST_ASSERT_EQUAL_STRING(
        "http://api.test/weather?q=&appid=XYZ123&units=metric",
        url.c_str()
    );
}

void test_build_weather_url_empty_api_key() {
    std::string url = weather::build_weather_url(
        "http://api.test/weather?q=",
        "Gdansk",
        ""
    );

    TEST_ASSERT_EQUAL_STRING(
        "http://api.test/weather?q=Gdansk&appid=&units=metric",
        url.c_str()
    );
}

void test_build_weather_url_long_city() {
    std::string url = weather::build_weather_url(
        "http://api.test/weather?q=",
        "VeryLongCityName1234567890",
        "XYZ123"
    );

    TEST_ASSERT_EQUAL_STRING(
        "http://api.test/weather?q=VeryLongCityName1234567890&appid=XYZ123&units=metric",
        url.c_str()
    );
}

void test_build_url() {
    RUN_TEST(test_build_weather_url_basic);
    RUN_TEST(test_build_weather_url_empty_city);
    RUN_TEST(test_build_weather_url_empty_api_key);
    RUN_TEST(test_build_weather_url_long_city);
}
