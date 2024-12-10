#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <vector>

using Field = std::vector<std::vector<char>>;

int part1(const Field &input) {
  std::array<std::vector<std::pair<int, int>>, 10> data{};
  for (auto i = 0; i < input.size(); i++) {
    for (auto j = 0; j < input[0].size(); j++) {
      data[(int)input[i][j] - '0'].push_back({i, j});
    };
  }

  // Messed up and did part2 first by accident. Copied everything and made the
  // code work. \o/
  std::map<std::pair<int, int>, std::set<std::pair<int, int>>> points{};
  for (auto &e : data[0]) {
    points[e] = std::set<std::pair<int, int>>{{e}};
  }
  auto addNeighbors = [&points](auto &point) {
    std::vector<std::pair<int, int>> neighbors = {
        {point.first - 1, point.second},
        {point.first + 1, point.second},
        {point.first, point.second - 1},
        {point.first, point.second + 1}};
    return std::accumulate(neighbors.begin(), neighbors.end(),
                           std::set<std::pair<int, int>>{},
                           [&points](auto acc, auto &p) {
                             if (points.contains(p)) {
                               auto p2 = points[p];
                               acc.merge(p2);
                             }
                             return acc;
                           });
  };
  for (auto list = data.begin() + 1; list != data.end(); list++) {
    std::map<std::pair<int, int>, std::set<std::pair<int, int>>> newPoints{};
    std::ranges::for_each(*list,
                          [&](auto &p) { newPoints[p] = addNeighbors(p); });
    points = newPoints;
  }

  return std::accumulate(
      points.begin(), points.end(), (long)0,
      [](auto acc, auto &n) { return acc + n.second.size(); });
}

int part2(const Field &input) {
  std::array<std::vector<std::pair<int, int>>, 10> data{};
  for (auto i = 0; i < input.size(); i++) {
    for (auto j = 0; j < input[0].size(); j++) {
      data[(int)input[i][j] - '0'].push_back({i, j});
    };
  }

  std::map<std::pair<int, int>, int> points{};
  for (auto &e : data[9]) {
    points[e] = 1;
  }
  auto addNeighbors = [&points](auto &point) {
    std::vector<std::pair<int, int>> neighbors = {
        {point.first - 1, point.second},
        {point.first + 1, point.second},
        {point.first, point.second - 1},
        {point.first, point.second + 1}};
    return std::accumulate(neighbors.begin(), neighbors.end(), 0,
                           [&points](auto acc, auto &p) {
                             return acc + (points.contains(p) ? points[p] : 0);
                           });
  };
  for (auto list = ++data.rbegin(); list != data.rend(); list++) {
    std::map<std::pair<int, int>, int> newPoints{};
    std::ranges::for_each(*list,
                          [&](auto &p) { newPoints[p] = addNeighbors(p); });
    points = newPoints;
  }

  return std::accumulate(points.begin(), points.end(), 0,
                         [](auto acc, auto &n) { return acc + n.second; });
}

int main(int argc, char *argv[]) {
  auto lines = readFileLines("day10.txt");
  Field input = lines | std::views::transform([](auto &l) {
                  return std::vector<char>(l.begin(), l.end());
                }) |
                std::ranges::to<Field>();
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
