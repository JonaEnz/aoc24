#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <ranges>
#include <string>
#include <util.h>
#include <vector>

using Entry = std::vector<int>;

namespace {

bool allDescOrInc(Entry &e) {
  return std::ranges::is_sorted(e, std::less{}) ||
         std::ranges::is_sorted(e, std::greater{});
};
bool diffAtMostThree(Entry &e) {
  for (auto [a, b] : e | std::views::pairwise) {
    if (auto d = abs(a - b); d > 3 || d < 1) {
      return false;
    }
  }
  return true;
};

bool isSafe(Entry &e) { return allDescOrInc(e) && diffAtMostThree(e); };
} // namespace

int part1(std::vector<Entry> input) {
  return std::ranges::count_if(input, isSafe);
}
int part2(std::vector<Entry> input) {
  auto isDampSafe = [](Entry &e) {
    for (int j = 0; j < e.size(); j++) {
      Entry eCopy(e);
      eCopy.erase(eCopy.begin() + j);
      if (isSafe(eCopy)) {
        return true;
      }
    }
    return false;
  };
  return std::ranges::count_if(input, isDampSafe);
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
