#include <HTTPClient.h>
#include <string>
#include <string_view>

int http_get(std::string_view url, std::string& outPayload) {
  const std::string url_nt(url);

  HTTPClient http;
  http.begin(url_nt.c_str());
  int code = http.GET();

  if (code > 0) {
    outPayload = std::string(http.getString().c_str());
  }

  http.end();
  return code;
}
