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
#include <vector>

long NextSecretNumber(long secret) {
  auto mixAndPrune = [](long n, long m) { return (n ^ m) % 16777216; };
  secret = mixAndPrune(secret * 64, secret);
  secret = mixAndPrune(secret / 32, secret);
  secret = mixAndPrune(secret * 2048, secret);
  return secret;
}

long part1(const std::vector<long> &input) {
  auto lambda = [](auto secret) {
    for (auto _ : std::views::iota(0, 2000)) {
      secret = NextSecretNumber(secret);
    }
    return secret;
  };
  return std::accumulate(
      input.begin(), input.end(), (long)0,
      [&lambda](auto acc, auto n) { return acc + lambda(n); });
}

template <> struct std::hash<std::array<int, 4>> {
  auto operator()(const std::array<int, 4> &o) const {
    return o[0] + o[1] * 100 + o[2] * 10000 + o[3] * 1000000;
  }
};

using Map = std::unordered_map<std::array<int, 4>, long>;
long part2(std::vector<long> input) {
  auto getPriceMap = [](long secret) {
    Map sequencePrices{};
    std::array<int, 4> history = {0, 0, 0, 0};
    for (auto i : std::views::iota(0, 2000)) {
      auto lastPrice = secret % 10;
      secret = NextSecretNumber(secret);
      auto price = secret % 10;
      history = {
          history[1],
          history[2],
          history[3],
          (int)(price - lastPrice),
      };
      if (!sequencePrices.contains(history) && i >= 4) {
        sequencePrices[history] = price;
      }
    }
    return sequencePrices;
  };
  std::vector<Map> sequences(input.size());
  std::transform(input.begin(), input.end(), sequences.begin(), getPriceMap);
  std::unordered_map<std::array<int, 4>, bool> seen{};
  for (auto &e : sequences) {
    for (auto &[h, _] : e) {
      if (!seen.contains(h))
        seen[h] = true;
    }
  }

  long record = 0;
  for (auto &h : seen) {
    long sum = 0;
    for (auto &s : sequences) {
      sum += s[h.first];
    }
    if (sum > record) {
      record = sum;
    }
  }
  return record;
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
