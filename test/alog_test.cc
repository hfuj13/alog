#include "gtest/gtest.h"
#include "alog.hpp"

using namespace std;
using namespace hf;

TEST(alog, init)
{
  // いきなり使っても落ちないことを確認
  alog& log = alog::get_instance();
  log << "no print" << endl;

  // ログレベル初期値を確認
  alog::level_t expected = alog::SILENT;
  alog::level_t actual = log.level();
  ASSERT_EQ(expected, actual);
}


// 各get_instance()で同じインスタンスを得ることを確認
TEST(alog, get_instance)
{
  {
    alog& expected = alog::get_instance();
    alog& actual = alog::get_instance(cout);
    ASSERT_EQ(expected, actual);
  }
}

// UNDER_LVL, OVER_LVLは指定できないこと・落ちることを確認 (DEATH TEST)
TEST(alog, invalid_level)
{
  {
    alog& log = alog::get_instance();
    ASSERT_DEATH(log.level(alog::UNDER_LVL), "");
  }

  {
    alog& log = alog::get_instance();
    ASSERT_DEATH(log.level(alog::OVER_LVL), "");
  }
}

// 設定したレベルであることを確認
TEST(alog, set_get_level)
{
  alog& log = alog::get_instance(cout);

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
  {
    alog& log = alog::get_instance(ost);
    log.t() << "@@@@ TEST @@@@" << endl;
    SUCCEED();
  }
  {
    alog& log = alog::get_instance(ost);
    log.force() << "====\n";
    log.level(alog::VERBOSE);
    log.t() << "@@@@ TEST @@@@" << endl;
    log.v() << "@@@@ verbose @@@@" << endl;
    log.d() << "@@@@ debug @@@@" << endl;
    log.i() << "@@@@ info @@@@" << endl;
    log.w() << "@@@@ warning @@@@" << endl;
    log.e() << "@@@@ error @@@@" << endl;
    SUCCEED();
  }
  {
    alog& log = alog::get_instance(ost);
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
    alog& log = alog::get_instance(ost);
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
    alog& log = alog::get_instance(ost);
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
    alog& log = alog::get_instance(ost);
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
    alog& log = alog::get_instance(ost);
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
