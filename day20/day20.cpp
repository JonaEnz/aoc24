#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Field = std::vector<std::vector<char>>;

using Point = std::pair<int, int>;

static std::array<std::pair<int, int>, 4> directions = {
    std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(1, 0),
    std::make_pair(0, -1)}; // TRBL

Point FindFieldByChar(const Field &field, char c) {
  for (int i = 0; i < field.size(); ++i) {
    for (int j = 0; j < field[0].size(); ++j) {
      if (field[i][j] == c)
        return {i, j};
    }
  }
  return {-1, -1};
}

std::vector<Point> FindPath(const Field &field, Point &start, Point &end) {
  auto curr = start;
  std::vector<Point> path{start};
  while (curr != end) {
    for (auto &d : directions) {
      Point next = {curr.first + d.first, curr.second + d.second};
      if (field[next.first][next.second] != '#' &&
          std::ranges::find(path, next) == path.end()) {
        path.push_back(next);
        curr = next;
        break;
      }
    }
  }
  return path;
}

long part1(const Field &field) {
  auto start = FindFieldByChar(field, 'S');
  auto end = FindFieldByChar(field, 'E');
  auto path = FindPath(field, start, end);
  auto isValidShortcut = [](Point &s, Point &e) {
    return std::abs(s.first - e.first) + std::abs(s.second - e.second) == 2;
  };
  auto result = 0;
  const int cheat = 2;
  const int timeSavingLimit = 100 + cheat;
  for (auto i = 0; i < path.size() - timeSavingLimit; ++i) {
    for (auto j = i + timeSavingLimit; j < path.size(); ++j) {
      result += isValidShortcut(path[i], path[j]) ? 1 : 0;
    }
  }
  return result;
}
long part2(Field &field) {
  auto start = FindFieldByChar(field, 'S');
  auto end = FindFieldByChar(field, 'E');
  auto path = FindPath(field, start, end);
  const int cheat = 20;
  const int minSavings = 100;
  auto cheatLen = [](Point &s, Point &e) {
    return std::abs(s.first - e.first) + std::abs(s.second - e.second);
  };
  auto result = 0;
  for (auto i = 0; i < path.size() - minSavings; ++i) {
    for (auto j = i + minSavings; j < path.size(); ++j) {
      auto c = cheatLen(path[i], path[j]);
      result += (c <= cheat && j - i - c >= minSavings) ? 1 : 0;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  auto input = readFileLines("day20.txt");
  Field field{};
  field.resize(input.size());
  std::transform(input.begin(), input.end(), field.begin(),
                 [](auto &l) { return std::vector<char>(l.begin(), l.end()); });
  printf("Part 1: %ld\n", part1(field));
  printf("Part 2: %ld\n", part2(field));

  return 0;
}
