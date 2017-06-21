#include "alog.hpp"

using namespace hf;
void worker2(alog* & p)
{
  alog& log = alog_single::get_instance();
  p = &log;
}
void worker3(alog* & p)
{
  static alog log;
  p = &log;
}
