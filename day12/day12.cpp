#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <util.h>
#include <vector>

using Field = std::vector<std::vector<char>>;

int part1(const Field &input) {
  auto result = 0;
  std::vector<std::vector<bool>> visited = {};
  visited.resize(input.size());
  int visitedCount = 0;
  std::fill_n(visited.begin(), input.size(),
              std::vector<bool>(input[0].size()));
  std::vector<std::vector<std::pair<int, int>>> regions{};
  std::queue<std::pair<int, int>> newRegions{{std::make_pair(0, 0)}};
  auto getNeighbors = [](auto &p) {
    return std::vector<std::pair<int, int>>{{p.first + 1, p.second},
                                            {p.first - 1, p.second},
                                            {p.first, p.second + 1},
                                            {p.first, p.second - 1}};
  };
  auto inField = [&input](auto &p) {
    return p.first >= 0 && p.second >= 0 && p.first < input.size() &&
           p.second < input[0].size();
  };

  auto regionValue = [&input, &inField, &getNeighbors](auto &region) {
    return std::accumulate(
        region.begin(), region.end(), 0,
        [&getNeighbors, &inField, &region](auto acc, auto &p) {
          return acc + std::ranges::count_if(
                           getNeighbors(p), [&inField, &region](auto &n) {
                             return std::ranges::find(region, n) ==
                                    region.end();
                           });
        });
  };

  while (visitedCount < input.size() * input[0].size()) {
    std::queue<std::pair<int, int>> regionQueue{{newRegions.front()}};
    auto a = regionQueue.front();
    newRegions.pop();
    if (visited[a.first][a.second]) {
      continue;
    }
    auto regionCount = 0;
    std::vector<std::pair<int, int>> region{};
    while (!regionQueue.empty()) {
      auto &point = regionQueue.front();
      regionQueue.pop();
      if (visited[point.first][point.second]) {
        continue;
      }
      regionCount++;
      visitedCount++;
      visited[point.first][point.second] = true;
      region.push_back(point);
      for (auto &neigh : getNeighbors(point)) {
        if (inField(neigh) && !visited[neigh.first][neigh.second]) {
          if (input[point.first][point.second] ==
              input[neigh.first][neigh.second]) {
            regionQueue.push(neigh);
          } else {
            newRegions.push(neigh);
          }
        }
      }
    }
    regions.push_back(region);
    result += regionCount * regionValue(region);
  }

  return result;
}
int part2(const Field &input) { return 0; }

int main(int argc, char *argv[]) {
  auto input = readFileLines("day12.txt") | std::views::transform([](auto l) {
                 return std::vector<char>(l.begin(), l.end());
               }) |
               std::ranges::to<std::vector<std::vector<char>>>();
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
