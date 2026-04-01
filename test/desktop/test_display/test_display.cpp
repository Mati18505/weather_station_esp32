#include <unity.h>

#include "./test_format_scrollable.h"
#include "test_format_first_line.h"

int main() {
  UNITY_BEGIN();
  test_format_first_line();
  test_format_scrollable();
  return UNITY_END();
}
