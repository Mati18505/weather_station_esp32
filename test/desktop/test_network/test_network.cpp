#include <unity.h>

#include "test_retry_until_success.h"

int main() {
  UNITY_BEGIN();
  test_retry_until_success();
  return UNITY_END();
}
