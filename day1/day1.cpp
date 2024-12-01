#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <numeric>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::pair<int, int>;

int part1(std::vector<Entry> input) {
  auto left = std::vector<int>{};
  auto right = std::vector<int>{};
  left.reserve(input.size());
  right.reserve(input.size());
  std::ranges::for_each(input, [&left, &right](const auto &e) {
    left.emplace_back(e.first);
    right.emplace_back(e.second);
  });
  std::ranges::sort(left, std::greater<int>());
  std::ranges::sort(right, std::greater<int>());
  auto result = 0;
  for (int i = 0; i < left.size(); i++) {
    result += abs(left[i] - right[i]);
  }
  return result;
}
int part2(std::vector<Entry> input) {
  auto left = std::vector<int>{};
  auto right = std::vector<int>{};
  left.reserve(input.size());
  right.reserve(input.size());
  std::ranges::for_each(input, [&left, &right](const auto &e) {
    left.emplace_back(e.first);
    right.emplace_back(e.second);
  });

  return std::accumulate(
      left.begin(), left.end(), 0, [&right](auto acc, auto n) {
        return acc + n * std::ranges::count_if(right,
                                               [&n](auto m) { return n == m; });
      });
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    auto l = s.substr(0, s.find(' '));
    auto r = s.substr(s.find_last_of(' ') + 1);
    return Entry{std::stoi(l), std::stoi(r)};
  };
  std::vector<Entry> input = parseFileByLine("day1.txt", parse);
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
