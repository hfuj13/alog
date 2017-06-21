#include <thread>
#include "gtest/gtest.h"
#include "alog.hpp"

using namespace std;
using namespace hf;

TEST(alog, init)
{
  // いきなり使っても落ちないことを確認
  alog log;
  log << "no print" << endl;

  // ログレベル初期値を確認
  alog::level_t expected = alog::SILENT;
  alog::level_t actual = log.level();
  ASSERT_EQ(expected, actual);
}


// UNDER_LVL, OVER_LVLは指定できないこと・落ちることを確認 (DEATH TEST)
TEST(alog, invalid_level)
{
  alog log;
  ASSERT_DEATH(log.level(alog::UNDER_LVL), "");
  ASSERT_DEATH(log.level(alog::OVER_LVL), "");
}

// 設定したレベルであることを確認
TEST(alog, set_get_level)
{
  alog log(cout);

  {
    alog::level_t expected = alog::VERBOSE;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

  {
    alog::level_t expected = alog::DEBUG;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

  {
    alog::level_t expected = alog::INFO;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

  {
    alog::level_t expected = alog::WARNING;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

  {
    alog::level_t expected = alog::ERROR;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

  {
    alog::level_t expected = alog::SILENT;
    log.level(expected);
    alog::level_t actual = log.level();
    ASSERT_EQ(expected, actual);
  }

}

TEST(alog, logoutput)
{
  std::ostream& ost = cout;
  alog log(ost);
  {
    log.force() << "====\n";
    log.level(alog::VERBOSE);
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
  {
    log.force() << "====\n";
    log.level(alog::DEBUG);
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
  {
    log.force() << "====\n";
    log.level(alog::INFO);
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
  {
    log.force() << "====\n";
    log.level(alog::WARNING);
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
  {
    log.force() << "====\n";
    log.level(alog::ERROR);
    SUCCEED();
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
  }
  {
    log.force() << "====\n";
    log.level(alog::SILENT);
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
}

TEST(alog, format)
{
  std::ostream& ost = cout;
  alog log(ost);
  {
    log.force() << "====\n";
    log.level(alog::VERBOSE);
    log.v("%s %d%s", "[verbose", 10, "]") << endl;
    log.v("[verbose 20]") << endl;
//    log.d() << "@@@@ debug @@@@" << endl;
//    log.i() << "@@@@ info @@@@" << endl;
//    log.w() << "@@@@ warning @@@@" << endl;
//    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
}

TEST(alog_singleton, init)
{
  alog& log1 = alog_single::get_instance();
  alog& log2 = alog_single::get_instance();

  ASSERT_EQ(&log1, &log2);
}

void worker1(alog* & p)
{
  alog& log = alog_single::get_instance();
  p = &log;
}
extern void worker2(alog* & p);
extern void worker3(alog* & p);

TEST(alog_singleton, multithread)
{
  alog* p1 = reinterpret_cast<alog*>(1);
  alog* p2 = reinterpret_cast<alog*>(2);
  alog* p3 = reinterpret_cast<alog*>(3);
  std::thread th1(worker1, ref(p1));
  std::thread th2(worker2, ref(p2));
  std::thread th3(worker3, ref(p3));
  th1.join();
  th2.join();
  th3.join();
  ASSERT_EQ(p1, p2);
  ASSERT_NE(p1, p3);
}
