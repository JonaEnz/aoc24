#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::pair<int, int>;

int part1(std::vector<Entry> &input, std::vector<std::vector<int>> &input2) {
  std::map<int, std::set<int>> forcedMap{};
  std::ranges::for_each(input, [&forcedMap](auto &pair) {
    if (forcedMap.contains(pair.second)) {
      forcedMap[pair.second].insert(pair.first);
    } else {
      forcedMap[pair.second] = {pair.first};
    }
  });
  return std::accumulate(
      input2.begin(), input2.end(), 0, [&forcedMap](int acc, auto &l) {
        std::set<int> s{};
        std::for_each(l.rbegin(), l.rend(), [&forcedMap, &s](auto n) {
          s.erase(n);
          for (auto q : forcedMap[n]) {
            s.insert(q);
          }
        });
        auto res = std::ranges::all_of(
            s, [&l](auto q) { return std::ranges::find(l, q) == l.end(); });
        return acc + (res ? l[int(l.size() / 2)] : 0);
      });
}
int part2(std::vector<Entry> &input, std::vector<std::vector<int>> &input2) {
  auto sorting = [&input](const auto &l, const auto &r) {
    return std::ranges::find(input, std::pair<int, int>{r, l}) == input.end();
  };
  return std::accumulate(input2.begin(), input2.end(), 0,
                         [&sorting, &input](auto acc, auto row) {
                           std::ranges::sort(row, sorting);
                           return acc + row[int(row.size() / 2)];
                         });
}

int main(int argc, char *argv[]) {
  auto in = readFileLines("day5.txt");
  std::vector<Entry> input{};
  std::vector<std::vector<int>> input2{};
  auto i = 0;
  while (in[i].size() > 1) {
    auto split = splitString(in[i], "|");
    input.push_back({std::stoi(split[0]), std::stoi(split[1])});
    i++;
  }
  for (++i; i < in.size(); i++) {
    auto split = splitString(in[i], ",");
    std::vector<int> a{};
    a.resize(split.size());
    std::transform(split.begin(), split.end(), a.begin(),
                   [](auto n) { return std::stoi(n); });
    input2.push_back(a);
  }
  auto p1 = part1(input, input2);
  printf("Part 1: %d\n", p1);
  printf("Part 2: %d\n", part2(input, input2) - p1);
  return 0;
}
