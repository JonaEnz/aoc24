#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::vector<char>;
using Point = std::pair<int, int>;
using AntennaMap = std::map<char, std::vector<Point>>;

bool isValidPoint(const Point &point, const Point &size) {
  return point.first >= 0 && point.second >= 0 && point.first < size.first &&
         point.second < size.second;
}

std::vector<Point>
Antinodes(const AntennaMap &map,
          std::function<std::vector<Point>(Point &, Point &)> antinodeFunc) {
  std::vector<Point> nodes{};
  for (auto [c, vec] : map) {
    for (auto i = 0; i < vec.size() - 1; i++) {
      for (auto j = i + 1; j < vec.size(); j++) {
        auto anti = antinodeFunc(vec[i], vec[j]);
        nodes.insert(nodes.end(), anti.begin(), anti.end());
      }
    }
  }
  return nodes;
}

int part1(const AntennaMap &input, Point &size) {
  auto antinodes = [](auto &l, auto &r) {
    auto diffX = l.first - r.first;
    auto diffY = l.second - r.second;
    return std::vector<Point>{{l.first + diffX, l.second + diffY},
                              {r.first - diffX, r.second - diffY}};
  };

  auto n = Antinodes(input, antinodes) |
           std::views::filter(
               [&size](const auto &p) { return isValidPoint(p, size); }) |
           std::ranges::to<std::set<Point>>();

  return n.size();
}

int part2(const AntennaMap &input, Point &size) {

  auto antinodes = [&size](auto l, auto r) {
    auto diffX = l.first - r.first;
    auto diffY = l.second - r.second;
    std::vector<Point> out{};
    while (isValidPoint(l, size)) {
      out.push_back(l);
      l.first += diffX;
      l.second += diffY;
    }
    while (isValidPoint(r, size)) {
      out.push_back(r);
      r.first -= diffX;
      r.second -= diffY;
    }
    return out;
  };
  auto n = Antinodes(input, antinodes) |
           std::views::filter(
               [&size](const auto &p) { return isValidPoint(p, size); }) |
           std::ranges::to<std::set<Point>>();

  return n.size();
}

AntennaMap Antennas(const std::vector<Entry> &input) {
  AntennaMap m{};
  for (auto i = 0; i < input.size(); i++) {
    for (auto j = 0; j < input[i].size(); j++) {
      auto c = input[i][j];
      if (c != '.') {
        if (m.contains(c)) {
          m[c].push_back({i, j});
        } else {
          m[c] = std::vector<Point>{{i, j}};
        }
      }
    }
  }
  return m;
}
int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    return std::vector<char>(s.begin(), s.end());
  };
  auto field = parseFileByLine("day8.txt", parse);
  Point size = {field.size(), field[0].size()};
  auto input = Antennas(field);
  printf("Part 1: %d\n", part1(input, size));
  printf("Part 2: %d\n", part2(input, size));
  return 0;
}
