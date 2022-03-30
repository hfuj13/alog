#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>

namespace hf {

template<typename... Args> inline std::string format(const std::string& fmt, Args... args)
{
  constexpr int capacity = 512;
  std::string buff(capacity, '\0');

  int ret = std::snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
  if (ret > capacity) {
    buff.reserve(ret);
    ret = std::snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
  }
  else if (ret < 0) {
    std::abort();
  }
  return buff;
}

} // namespace hf
