#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <vector>

int part1(const std::vector<std::string> &flags,
          const std::vector<std::string> input) {
  auto strLenCmp = [](auto &l, auto &r) { return l.size() > r.size(); };
  auto isPossible = [&flags](auto target) {
    std::priority_queue<std::string, std::vector<std::string>,
                        decltype(strLenCmp)>
        queue{};
    queue.push(target);
    std::set<std::string> seen{};
    while (!queue.empty()) {
      auto candidate = queue.top();
      queue.pop();

      if (candidate.size() == 0) {
        return true;
      }
      if (seen.contains(candidate))
        continue;
      seen.insert(candidate);
      for (auto &f : flags) {
        if (f.size() > candidate.size())
          continue;
        if (0 == std::strncmp(candidate.c_str(), f.c_str(), f.size())) {
          queue.push({candidate.begin() + f.size(), candidate.end()});
        }
      }
    }
    return false;
  };
  return std::ranges::count_if(input, isPossible);
}
long part2(const std::vector<std::string> &flags,
           std::vector<std::string> input) {
  std::map<std::string, long> waysPossible{};
  std::function<long(std::string)> calcWaysPossible =
      [&waysPossible, &flags, &calcWaysPossible](auto candidate) {
        if (candidate.size() == 0) {
          return (long)1;
        }
        long sum = 0;
        for (auto &f : flags) {
          if (f.size() > candidate.size())
            continue;
          if (0 == std::strncmp(candidate.c_str(), f.c_str(), f.size())) {
            std::string next{candidate.begin() + f.size(), candidate.end()};
            if (!waysPossible.contains(next))
              waysPossible[next] = calcWaysPossible(next);
            sum += waysPossible[next];
          }
        }
        waysPossible[candidate] = sum;
        return sum;
      };
  return std::accumulate(input.begin(), input.end(), (long)0,
                         [&calcWaysPossible](auto acc, auto &t) {
                           return acc + calcWaysPossible(t);
                         });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> input = readFileLines("day19.txt");
  std::vector<std::string> flags = splitString(input[0], ", ");
  input.erase(input.begin(), input.begin() + 2);
  printf("Part 1: %d\n", part1(flags, input));
  printf("Part 2: %ld\n", part2(flags, input));
  return 0;
}
