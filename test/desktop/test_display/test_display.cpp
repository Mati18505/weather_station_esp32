#include <unity.h>

#include "test_format_scrollable.h"
#include "test_format_first_line.h"
#include "test_wrap_increment.h"

int main() {
  UNITY_BEGIN();
  test_format_first_line();
  test_format_scrollable();
  test_wrap_increment();
  return UNITY_END();
}
