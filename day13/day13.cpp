#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <regex>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::pair<long, long>;

long part1(const std::vector<Entry> &input) {
  long tokens = 0;
  for (auto i = 0; i < input.size(); i += 4) {
    auto &buttonA = input[i];
    auto &buttonB = input[i + 1];
    auto &goal = input[i + 2];

    double det = (double)(buttonA.first * buttonB.second -
                          buttonB.first * buttonA.second);
    auto a =
        (double)(goal.first * buttonB.second - goal.second * buttonB.first) /
        det;
    auto b =
        (double)(goal.second * buttonA.first - goal.first * buttonA.second) /
        det;
    if (a - std::floor(a) == 0.0 && b - std::floor(b) == 0) {
      tokens += 3 * a + b;
    }
  }
  return tokens;
}
long part2(std::vector<Entry> &input) {
  const long add = 10000000000000;
  for (auto i = 0; i < input.size(); i += 4) {
    auto &goal = input[i + 2];
    goal.first += add;
    goal.second += add;
  }
  return part1(input);
}

int main(int argc, char *argv[]) {
  std::regex regex(".*: X.(\\d+), Y.(\\d+)");
  std::function<Entry(const std::string)> parse =
      [&regex](const std::string s) {
        if (s.size() == 0) {
          return Entry{0, 0};
        }
        std::smatch smatch;
        std::regex_match(s, smatch, regex);
        return Entry{
            std::stoi(smatch[1].str()),
            std::stoi(smatch[2].str()),
        };
      };
  std::vector<Entry> input = parseFileByLine("day13.txt", parse);
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %ld\n", part2(input));
  return 0;
}
