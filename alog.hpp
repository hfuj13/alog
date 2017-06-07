/// A little LOGger

#pragma once

#include <iomanip>
#include <ostream>
#include <string>
#include <thread>

#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace hf {

/// C++14 later
/// Linux

inline std::string now_timestamp()
{
  return "[1999-06-06T06:06:06.666]";
}

class alog final {
public:
  enum level_t {
    UNDER_LVL = 0,
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    SILENT,
    OVER_LVL
  };

  static alog& get_instance()
  {
    return get_instance(nullptr);
  }
  static alog& get_instance(std::ostream& ost)
  {
    return get_instance(&ost);
  }

  bool operator==(const alog& rhs) const
  {
    return &rhs == this || (rhs.level_ == level_ && rhs.ost_ == ost_);
  }
  bool operator!=(const alog& rhs) const
  {
    return (rhs.level_ != level_ || rhs.ost_ != ost_);
  }
  template<typename... Args> static std::string format(const std::string& fmt, Args... args)
  {
    constexpr int capacity = 512;
    std::string buff(capacity, '\0');

    int ret = snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
    if (ret > capacity) {
      buff.reserve(ret);
      ret = snprintf(&buff[0], buff.capacity(), fmt.c_str(), args...);
    }
    else if (ret < 0) {
      abort();
    }
    std::string str(buff.c_str());
    return str;

  }

  // for verbose
  std::ostream& v()
  {
    return (*this)(VERBOSE) << "[V]";
  }
  template<typename... Args> std::ostream& v(const std::string& fmt, Args... args)
  {
    return v() << format(fmt, args...) << std::flush;
  }

  // for debug
  std::ostream& d()
  {
    return (*this)(DEBUG) << "[D]";
  }
  template<typename... Args> std::ostream& d(const std::string& fmt, Args... args)
  {
    return d() << format(fmt, args...) << std::flush;
  }

  // for info
  std::ostream& i()
  {
    return (*this)(INFO) << "[I]";
  }
  template<typename... Args> std::ostream& i(const std::string& fmt, Args... args)
  {
    return i() << format(fmt, args...) << std::flush;
  }

  // for warning
  std::ostream& w()
  {
    return (*this)(WARNING) << "[W]";
  }
  template<typename... Args> std::ostream& w(const std::string& fmt, Args... args)
  {
    return w() << format(fmt, args...) << std::flush;
  }

  // for error
  std::ostream& e()
  {
    return (*this)(ERROR) << "[E]";
  }
  template<typename... Args> std::ostream& e(const std::string& fmt, Args... args)
  {
    return e() << format(fmt, args...) << std::flush;
  }

  // ログレベル設定が何であっても強制的に出力する
  std::ostream& force()
  {
    return (*this)();
  }
  template<typename... Args> std::ostream& force(const std::string& fmt, Args... args)
  {
    return force() << format(fmt, args...) << std::flush;
  }

  template<typename T> friend std::ostream& operator<<(alog& log, const T& rhs);

  alog& level(level_t lvl)
  {
    assert(UNDER_LVL < lvl && lvl < OVER_LVL);
    if (lvl <= UNDER_LVL || OVER_LVL <= lvl) {
      abort();
    }

    level_ = lvl;
    return *this;
  }
  level_t level()
  {
    return level_;
  }

private:
  alog() = default;
  alog& operator=(const alog&) = delete;

  std::ostream& operator()()
  {
    return (*ost_) << now_timestamp() << "[thd:" << std::this_thread::get_id() << "] ";
  }
  std::ostream& operator()(level_t lvl)
  {
    if (lvl >= level_) {
      return (*ost_) << now_timestamp() << "[thd:" << std::this_thread::get_id() << "] ";
    }
    else {
      return null_ost_;
    }
  }

  static alog& get_instance(std::ostream* ost)
  {
    static alog log;
    if (ost != nullptr) {
      log.ost_ = ost;
    }
    return log;
  }

  std::ostream null_ost_{nullptr}; // /dev/null like ostream
  level_t level_ = level_t::SILENT;
  std::ostream* ost_ = &null_ost_;
};

template<typename T> std::ostream& operator<<(alog& log, const T& rhs)
{
  return (*log.ost_) << rhs;
}

} // namespace hf
