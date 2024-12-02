#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::vector<int>;

int part1(std::vector<Entry> input) {
  auto allDesc = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (e[i + 1] >= e[i]) {
        return false;
      }
    }
    return true;
  };
  auto allInc = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (e[i + 1] <= e[i]) {
        return false;
      }
    }
    return true;
  };
  auto diffAtMostThree = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (abs(e[i + 1] - e[i]) > 3) {
        return false;
      }
    }
    return true;
  };
  auto safe = [allInc, allDesc, diffAtMostThree](Entry &e) {
    return (allInc(e) || allDesc(e)) && diffAtMostThree(e);
  };
  return std::ranges::count_if(input, safe);
}
int part2(std::vector<Entry> input) {
  auto allDesc = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (e[i + 1] >= e[i]) {
        return false;
      }
    }
    return true;
  };
  auto allInc = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (e[i + 1] <= e[i]) {
        return false;
      }
    }
    return true;
  };
  auto diffAtMostThree = [](Entry &e) {
    for (auto i = 0; i < e.size() - 1; i++) {
      if (abs(e[i + 1] - e[i]) > 3) {
        return false;
      }
    }
    return true;
  };
  auto safe = [allInc, allDesc, diffAtMostThree](Entry &e) {
    for (int j = 0; j < e.size(); j++) {
      Entry eCopy(e);
      eCopy.erase(eCopy.begin() + j);
      if ((allInc(eCopy) || allDesc(eCopy)) && diffAtMostThree(eCopy)) {
        return true;
      }
    }
    return false;
  };
  return std::ranges::count_if(input, safe);
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    auto e = Entry{};
    for (auto v : s | std::views::split(' ')) {
      auto s2 = std::string(v.begin(), v.end());
      if (s2.size() > 0) {
        e.push_back(std::stoi(s2));
      }
    }
    return e;
  };
  std::vector<Entry> input = parseFileByLine("day02.txt", parse);
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
