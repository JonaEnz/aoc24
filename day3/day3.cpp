#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::string;

std::pair<int, int> part1NextMul(std::string &s) {
  std::regex mulRegex("mul\\(([0-9]+),([0-9]+)\\)",
                      std::regex_constants::ECMAScript);
  std::smatch match;
  if (!std::regex_search(s, match, mulRegex)) {
    s = "";
    return {0, 0};
  }

  std::pair<int, int> p = {std::stoi(std::string(match[1])),
                           std::stoi(std::string(match[2]))};
  s = match.suffix();
  return p;
}
int part1(std::vector<Entry> input) {
  auto s = input[0];
  auto result = 0;
  while (s.size() > 0) {
    auto [a, b] = part1NextMul(s);
    result += a * b;
  }
  return result;
}
void filterDoDont(std::string &in, std::string &out, bool initDo) {
  if (initDo) {
    auto pos = in.find("don't()");
    if (pos == std::string::npos) {
      out.append(in);
      return;
    }
    out.append(in.substr(0, pos));
    in.erase(0, pos + 7);
    filterDoDont(in, out, false);
    return;
  }

  auto pos = in.find("do()");
  in.erase(0, pos + 4);
  filterDoDont(in, out, true);
}
int part2(std::vector<Entry> input) {

  auto s = input[0];
  std::string s2 = "";
  filterDoDont(s, s2, true);
  return part1({s2});
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    return std::string(s.begin(), s.end());
  };
  std::vector<Entry> input = parseFileByLine("day3.txt", parse);
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
