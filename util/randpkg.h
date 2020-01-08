#ifndef MOCK_RANDPKG_H
#define MOCK_RANDPKG_H

#include <chrono>
#include <cstdlib>
#include <string>
#include <vector>

#define TIMESTAMP_LENGTH 19

static const int SendTimes = 50;
static const std::vector<int> PackageSize = {500, 1000, 1500, 2000, 2500};

std::string tstoa(int64_t ts);
int64_t atots(std::string a);
int64_t timestamp();
std::string randpkg(int length, int64_t);
ino64_t getts(std::string);

#endif
