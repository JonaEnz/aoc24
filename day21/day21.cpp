#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

static std::map<char, std::pair<int, int>> numKeypad = {
    {'0', {3, 1}}, {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}},
    {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}}, {'7', {0, 0}},
    {'8', {0, 1}}, {'9', {0, 2}}, {'A', {3, 2}}, {'X', {3, 0}}};
static std::map<char, std::pair<int, int>> dirKeypad = {
    {'<', {1, 0}}, {'>', {1, 2}}, {'^', {0, 1}},
    {'v', {1, 1}}, {'A', {0, 2}}, {'X', {0, 0}}};

bool isLegal(std::map<char, std::pair<int, int>> &map, std::pair<int, int> pos,
             std::string &path) {
  for (auto c : path) {
    switch (c) {
    case '<':
      pos.second -= 1;
      break;
    case '>':
      pos.second += 1;
      break;
    case 'v':
      pos.first += 1;
      break;
    case '^':
      pos.first -= 1;
      break;
    default:
    }
    if (map['X'] == pos) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> GeneratePaths(std::map<char, std::pair<int, int>> &map,
                                       const char start, const char end) {
  auto &pos = map[start];
  auto &newPos = map[end];
  auto xDis = newPos.first - pos.first;
  auto yDis = newPos.second - pos.second;
  std::vector<std::string> paths{};
  auto h = std::string(std::abs(yDis), yDis < 0 ? '<' : '>');
  auto v = std::string(std::abs(xDis), xDis < 0 ? '^' : 'v');
  auto opt1 = v + h + "A";
  auto opt2 = h + v + "A";
  if (isLegal(map, pos, opt1))
    paths.push_back(opt1);
  if (isLegal(map, pos, opt2))
    paths.push_back(opt2);
  return paths;
}

std::vector<std::string> PossiblePaths(std::map<char, std::pair<int, int>> &map,
                                       const std::string &sequence) {
  auto curr = 'A';
  return std::accumulate(sequence.begin(), sequence.end(),
                         std::vector<std::string>{""},
                         [&map, &curr](const auto &acc, auto c) {
                           std::vector<std::string> o{};
                           for (auto &path : GeneratePaths(map, curr, c))
                             for (auto &p : acc)
                               o.push_back(p + path);
                           curr = c;
                           return o;
                         });
}

static std::map<std::tuple<char, char, int>, long> cache{};

long MinButtonPresses(std::map<char, std::pair<int, int>> &map,
                      const char posNow, const char nextPos, const int depth) {
  if (depth >= 0 && cache.contains({posNow, nextPos, depth})) {
    return cache[{posNow, nextPos, depth}];
  }
  auto presses = GeneratePaths(map, posNow, nextPos);
  if (depth <= 0) {
    return 1;
  }
  long record = LONG_MAX;
  for (auto &route : presses) {
    auto pos = 'A';
    long result = 0;
    for (auto c : route) {
      result += MinButtonPresses(map, pos, c, depth - 1);
      pos = c;
    }
    if (result < record)
      record = result;
  }
  if (depth > 0)
    cache[{posNow, nextPos, depth}] = record;
  return record;
}

long part12(const std::vector<std::string> &input, int depth) {
  cache = {};
  long answer = 0;
  for (auto &s : input) {
    auto complexity = std::stoi(s.substr(0, s.size() - 1));
    auto paths = PossiblePaths(numKeypad, s);
    long record = LONG_MAX;
    for (auto &path : paths) {
      auto curr = 'A';
      long r = 0;
      for (auto c : path) {
        r += MinButtonPresses(dirKeypad, curr, c, depth);
        curr = c;
      }
      if (r < record)
        record = r;
    }
    answer += record * complexity;
  }
  return answer;
}

int main(int argc, char *argv[]) {
  auto input = readFileLines("day21.txt");
  printf("Part 1: %ld\n", part12(input, 2));
  printf("Part 2: %ld\n", part12(input, 25));
  return 0;
}
