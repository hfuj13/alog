/// A little LOGger

#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <iomanip>
#include <ostream>
#include <string>
#include <thread>

namespace hf {

/// C++14 later

/// @brief get now timestamp. UTC only yet
///
/// @param [in] parenthesis: ture: output with '[' and ']' <br>
//      false : output with raw
/// @retval string transformed timestamp (e.g. [2016-12-11T13:24:13.058] or 2016-12-11T13:24:13.058 etc.). the output format is like the ISO8601 (that is it include milliseconds)
inline std::string now_timestamp()
{
  std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
  //std::chrono::nanoseconds nsec_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
  std::chrono::milliseconds msec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
  //std::chrono::milliseconds msec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(nsec_since_epoch);
  std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(msec_since_epoch);

  std::time_t tt = sec.count();
  std::size_t msec_part = msec_since_epoch.count() % 1000;
  //std::size_t nsec = nsec_since_epoch.count() % (1000 * 1000); 

  struct tm stm = {0};
  tzset();
  gmtime_r(&tt, &stm);

  std::ostringstream oss;

  oss << (stm.tm_year+1900) << '-'
      << std::setw(2) << std::setfill('0') << (stm.tm_mon+1) << '-'
      << std::setw(2) << std::setfill('0') << stm.tm_mday
      << 'T'
      << std::setw(2) << std::setfill('0') << stm.tm_hour
      << ':'
      << std::setw(2) << std::setfill('0') << stm.tm_min
      << ':'
      << std::setw(2) << std::setfill('0') << stm.tm_sec
      << '.' << std::setw(3) << std::setfill('0') << msec_part
      //<< std::setw(3) << std::setfill('0') << nsec
      ;

  std::string str;
  str += "[" + oss.str() + "]";

  return str;
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

//  class scoped final {
//  public:
//    scoped() = delete;
//    scoped(const scoped&) = default;
//    scoped(scoped&&) = default;
//
//    scoped(const std::string& str)
//    : scoped(level_t::DEBUG, str) {}
//    scoped(level_t loglevel, const std::string& str)
//    : loglevel_(loglevel), oss_(str)
//    {
//      log_(loglevel_) << "[[[[ " << oss_.str() << std::endl;
//    }
//    ~scoped()
//    {
//      log_(loglevel_) << "]]]] " << oss_.str() << std::endl;
//    }
//    std::string str()
//    {
//      return oss_.str();
//    }
//    scoped& clear()
//    {
//      oss_.str("");
//      return *this;
//    }
//    template<typename T> friend std::ostream& operator<<(scoped& slog, const T& rhs);
//  private:
//    alog& log_;
//    level_t loglevel_ = level_t::DEBUG;
//    std::ostringstream oss_;
//  };

  alog() = default;
  alog(const alog&) = default;
  alog(alog&&) = default;
  alog(std::ostream& ost)
  {
    ost_ = &ost;
  }
  ~alog() = default;

  alog& operator=(const alog&) = default;
  alog& operator=(alog&&) = default;
  std::ostream& operator()(level_t lvl)
  {
    return lvl >= level_ ? output() : null_ost_;
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

  friend class alog_single;
private:
  std::ostream& output()
  {
    return (*ost_) << now_timestamp() << "[thd:" << std::this_thread::get_id() << "] ";
  }

  std::ostream& operator()()
  {
    return output();
  }

  std::ostream null_ost_{nullptr}; // /dev/null like ostream
  level_t level_ = level_t::SILENT;
  std::ostream* ost_ = &null_ost_;
};

template<typename T> inline std::ostream& operator<<(alog& log, const T& rhs)
{
  return (*log.ost_) << rhs;
}

//template<typename T> std::ostream& operator<<(alog::scoped& slog, const T& rhs)
//{
//  return slog.oss_ << rhs;
//}

#if 0
template<typename T = alog>
class alog_singleton final {
public:
  alog_singleton() = delete;
  alog_singleton(const alog_singleton&) = delete;
  alog_singleton(alog_singleton&&) = delete;
  alog_singleton& operator=(const alog_singleton&) = delete;
  alog_singleton& operator=(alog_singleton&&) = delete;
  static alog& get_instance()
  {
    static_assert(std::is_base_of<alog, T>::value, "T is invalid. T must be the alog class.");
    static std::ostream null_ost{nullptr}; // /dev/null like ostream
    return get_instance(null_ost);
  }

  static alog& get_instance(std::ostream& ost)
  {
    static alog log;
    log.ost_ = &ost;
    return log;
  }

private:
};
using alog_single = alog_singleton<alog>;
#else
class alog_single final {
public:
  alog_single() = delete;
  alog_single(const alog_single&) = delete;
  alog_single(alog_single&&) = delete;
  alog_single& operator=(const alog_single&) = delete;
  alog_single& operator=(alog_single&&) = delete;
  static alog& get_instance()
  {
    static std::ostream null_ost{nullptr}; // /dev/null like ostream
    return get_instance(null_ost);
  }

  static alog& get_instance(std::ostream& ost)
  {
    static alog log;
    log.ost_ = &ost;
    return log;
  }
};
#endif

} // namespace hf
