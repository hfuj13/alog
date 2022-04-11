#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>

namespace hf {

template<typename... Args> inline std::string format(const std::string& fmt, Args... args)
{
  std::string buff;
  int ret = std::snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
  if (ret >= buff.capacity()) {
    buff.reserve(ret+1);
    ret = std::snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
  }
  else if (ret < 0) {
    std::abort();
  }
  return buff.c_str();
}

} // namespace hf
