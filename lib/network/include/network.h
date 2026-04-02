#pragma once

#include <cstdint>
#include <functional>

inline bool retry_until_success(std::function<bool()> condition, uint32_t max_attempts) {
  bool success = false;

  for (uint32_t attempt = 0; attempt < max_attempts; attempt++) {
    if (condition()) {
      success = true;
      break;
    }
  }

  return success;
}
