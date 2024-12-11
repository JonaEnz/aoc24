#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <vector>

using Entry = long;

static std::map<int, std::pair<long, long>> memo{};

std::pair<long, long> nextStones(long n) {
  if (n == 0) {
    return {1, -1};
  }
  auto numDigits = (int)std::floor(std::log10(n)) + 1;
  if (numDigits % 2 == 0) {
    // Split
    long split = std::pow(10, (numDigits / 2));
    return {(long)(n / split), n % split};
  }
  return {n * 2024, -1};
}

long run(std::vector<Entry> input, int iterations) {
  std::map<long, long> stones{};
  std::ranges::for_each(input, [&stones](auto n) { stones[n] = 1; });
  for (auto i : std::views::iota(0, iterations)) {
    std::map<long, long> newStones{};

    auto insertInMap = [&newStones](std::pair<long, long> pair) {
      if (newStones.contains(pair.first)) {
        newStones[pair.first] += pair.second;
      } else {
        newStones[pair.first] = pair.second;
      }
    };

    for (auto &[n, count] : stones) {
      if (!memo.contains(n)) {
        memo[n] = nextStones(n);
      }
      const auto &stones = memo[n];
      insertInMap({stones.first, count});
      if (stones.second != -1) {
        insertInMap({stones.second, count});
      }
    }
    stones = newStones;
  }
  return std::accumulate(stones.begin(), stones.end(), (long)0,
                         [](auto acc, auto &p) { return acc + p.second; });
}

long part1(std::vector<Entry> &input) { return run(input, 25); }
long part2(std::vector<Entry> &input) { return run(input, 75); }

int main(int argc, char *argv[]) {
  auto input = splitString(readFile("day11.txt"), " ") |
               std::views::transform([](auto w) { return std::stol(w); }) |
               std::ranges::to<std::vector<long>>();
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %ld\n", part2(input));
  return 0;
}
