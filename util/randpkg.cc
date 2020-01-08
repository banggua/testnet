#include "randpkg.h"

std::string tstoa(int64_t ts) { return std::to_string(ts); }
int64_t atots(std::string a) { return atoll(a.c_str()); }

int64_t timestamp() {
  auto tp = std::chrono::system_clock::now();
  auto dur = tp.time_since_epoch();

  return std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();
}

std::string randpkg(int length, int64_t ts) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  std::string s;
  s.append(tstoa(ts));
  s.resize(length);

  for (int i = TIMESTAMP_LENGTH; i < length; ++i) {
    s[i] = (alphanum[rand() % (sizeof(alphanum) - 1)]);
  }

  return s;
}

ino64_t getts(std::string a) { return atots(a.substr(0, TIMESTAMP_LENGTH)); }
