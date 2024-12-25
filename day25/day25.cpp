#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <vector>

long part1(std::vector<std::array<int, 5>> &keys,
           std::vector<std::array<int, 5>> &locks) {
  auto matches = [](auto &key, auto &lock) {
    return std::ranges::all_of(std::views::iota(0, 5), [&key, &lock](auto i) {
      return (key[i] & lock[i]) == 0x0;
    });
  };
  return std::accumulate(
      keys.begin(), keys.end(), (long)0,
      [&locks, &matches](auto acc, auto &key) {
        return acc + std::ranges::count_if(locks, [&key, &matches](auto &lock) {
                 return matches(key, lock);
               });
      });
}

int main(int argc, char *argv[]) {
  auto input = readFile("day25.txt");
  std::vector<std::array<int, 5>> locks{};
  std::vector<std::array<int, 5>> keys{};
  while (input.size() > 0) {
    auto end = input.find("\n\n");
    end = end == std::string::npos ? input.size() + 1 : end;
    auto block = splitString(std::string(input.substr(0, end)), "\n");
    input.erase(0, end + 2);
    std::array<int, 5> line{};
    for (auto [j, b] : std::views::enumerate(block)) {
      for (int i = 0; i < 5; i++) {
        line[i] |= ((b[i] == '#' ? 1 : 0) << j);
      }
    }
    if ((line[0] & 0x1) == 0x0)
      locks.push_back(line);
    else
      keys.push_back(line);
  }
  printf("Part 1: %ld\n", part1(keys, locks));
  return 0;
}
