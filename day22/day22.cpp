#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <vector>

long NextSecretNumber(long secret) {
  auto mixAndPrune = [&secret](long n) { return (n ^ secret) % 16777216; };
  secret = mixAndPrune(secret * 64);
  secret = mixAndPrune(secret / 32);
  secret = mixAndPrune(secret * 2048);
  return secret;
}

long part1(const std::vector<long> &input) {
  auto nextSecret2000 = [](auto secret) {
    for (auto _ : std::views::iota(0, 2000)) {
      secret = NextSecretNumber(secret);
    }
    return secret;
  };
  return std::accumulate(
      input.begin(), input.end(), (long)0,
      [&nextSecret2000](auto acc, auto n) { return acc + nextSecret2000(n); });
}

using Map = std::unordered_map<int, int>;
long part2(std::vector<long> input) {
  auto getPriceMap = [](long secret) {
    Map sequencePrices{};
    int history = 0;
    for (auto i : std::views::iota(0, 2000)) {
      auto lastPrice = secret % 10;
      secret = NextSecretNumber(secret);
      auto price = secret % 10;
      history = (history << 8) + (price - lastPrice + 10);
      if (!sequencePrices.contains(history) && i >= 4) {
        sequencePrices[history] = price;
      }
    }
    return sequencePrices;
  };
  Map sum{};
  for (auto &in : input) {
    for (auto &[h, p] : getPriceMap(in)) {
      sum[h] += p;
    }
  }
  return std::ranges::max(
      sum | std::views::transform([](auto &v) { return v.second; }));
}

int main(int argc, char *argv[]) {
  std::function<long(const std::string)> parse = [](const std::string s) {
    return std::stol(s);
  };
  std::vector<long> input = parseFileByLine("day22.txt", parse);
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %ld\n", part2(input));
  return 0;
}
