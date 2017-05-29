/// A little LOGger

#pragma once

#include <ostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <pthread.h>

namespace hf {

/// C++14 later
/// Linux

inline std::string now_timestamp()
{
  return "[1999-06-06T06:06:06.666]";
}

class alog /* final */ {
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

  // for test
  std::ostream& t()
  {
    return (*this)() << "[TEST]";
  }
  // for verbose
  std::ostream& v()
  {
    return (*this)(VERBOSE) << "[V]";
  }
  // for debug
  std::ostream& d()
  {
    return (*this)(DEBUG) << "[D]";
  }
  // for info
  std::ostream& i()
  {
    return (*this)(INFO) << "[I]";
  }
  // for warning
  std::ostream& w()
  {
    return (*this)(WARNING) << "[W]";
  }
  // for error
  std::ostream& e()
  {
    return (*this)(ERROR) << "[E]";
  }
  // ログレベル設定が何であっても強制的に出力する
  std::ostream& force()
  {
    return (*this)();
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
    return (*this) << now_timestamp() << "[thd:" << pthread_self() << "] ";
  }
  std::ostream& operator()(level_t lvl)
  {
    if (lvl >= level_) {
      return (*this) << now_timestamp() << "[thd:" << pthread_self() << "] ";
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

  std::ostream null_ost_{nullptr}; // 何も出力しない ostream用
  level_t level_ = level_t::SILENT;
  std::ostream* ost_ = &null_ost_;
};

template<typename T> std::ostream& operator<<(alog& log, const T& rhs)
{
  return (*log.ost_) << rhs;
}

} // namespace hf
