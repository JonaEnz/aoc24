#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

class Robot {
public:
  Robot() = default;
  Robot(std::pair<long, long> p, std::pair<long, long> v) : mPos{p}, mVel{v} {};

private:
  std::pair<long, long> mPos;
  std::pair<long, long> mVel;

public:
  void Print() const;
  std::pair<long, long> PositionAfter(const std::pair<long, long> &, int) const;
};

void Robot::Print() const {
  std::cout << mPos.first << " " << mPos.second << " " << mVel.first << " "
            << mVel.second << "\n";
}
std::pair<long, long>
Robot::PositionAfter(const std::pair<long, long> &fieldSize, int sec) const {
  std::pair<long, long> p = {
      (long)(mPos.first + mVel.first * sec) % fieldSize.first,
      (long)(mPos.second + mVel.second * sec) % fieldSize.second};
  if (p.first < 0) {
    p.first += fieldSize.first;
  }
  if (p.second < 0) {
    p.second += fieldSize.second;
  }

  return p;
}

long part1(const std::vector<Robot> &input) {
  const std::pair<long, long> fieldSize = {101, 103};
  std::array<int, 4> quadrants{};
  const int xMid = (fieldSize.first - 1) / 2;
  const int yMid = (fieldSize.second - 1) / 2;
  for (auto &r : input) {
    auto pos = r.PositionAfter(fieldSize, 100);
    if (pos.first == xMid || pos.second == yMid) {
      continue;
    }
    ++quadrants[(pos.first < xMid ? 0 : 1) + (pos.second < yMid ? 0 : 2)];
  }
  return std::accumulate(quadrants.begin(), quadrants.end(), 1,
                         [](auto acc, auto q) { return acc * q; });
}
long part2(const std::vector<Robot> &input) {
  const std::pair<long, long> fieldSize = {101, 103};
  auto sec = 0;
  std::set<std::pair<long, long>> robotPos{};
  auto isTree = false;
  while (!isTree) {
    ++sec;
    robotPos = {};
    for (auto &r : input) {
      auto p = r.PositionAfter(fieldSize, sec);
      if (robotPos.contains(p)) {
        break;
      }
      robotPos.insert(p);
    }
    isTree = robotPos.size() == input.size();
  }

  for (auto x = 0; x < fieldSize.first; x++) {
    for (auto y = 0; y < fieldSize.second; y++) {
      std::cout << (robotPos.contains({x, y}) ? "x" : " ");
    }
    std::cout << "\n";
  }

  return sec;
}

int main(int argc, char *argv[]) {
  std::regex regex("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)");
  std::function<Robot(const std::string)> parse =
      [&regex](const std::string s) {
        std::smatch smatch;
        std::regex_match(s, smatch, regex);
        return Robot{{std::stoi(smatch[1].str()), std::stoi(smatch[2].str())},
                     {std::stoi(smatch[3].str()), std::stoi(smatch[4].str())}};
      };
  std::vector<Robot> input = parseFileByLine("day14.txt", parse);
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %ld\n", part2(input));
  return 0;
}
