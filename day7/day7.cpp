#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>
#include <util.h>
#include <vector>

class Entry {
public:
  long result;
  long value;
  std::stack<int> nums;
  Entry() = default;
  Entry(std::string s);
  bool Options(bool part2 = false);
  void Print();
};

Entry::Entry(const std::string s) {
  auto split = splitString(s, " ");
  result = std::stol(split[0].substr(0, split[0].length() - 1));
  value = result;
  split.erase(split.begin());
  nums = std::stack<int>{};
  for (auto n : split) {
    nums.push(std::stoi(n));
  }
}

bool Entry::Options(bool part2) {
  auto reverseConcat = [](uint64_t result, int n) {
    uint64_t mul = 1;
    while (mul <= n) {
      mul *= 10;
    }
    return (result % mul == n) ? (result - n) / mul : -1;
  };
  if (result < 0) {
    return 0;
  }
  if (nums.size() == 1) {
    return result == nums.top();
  }
  bool res = 0;
  if ((result % nums.top()) == 0) {
    Entry div = *this;
    div.result /= nums.top();
    div.nums.pop();
    res = res || div.Options(part2);
  }
  if (part2) {
    Entry concat = *this;
    concat.result = reverseConcat(concat.result, concat.nums.top());
    concat.nums.pop();
    res = res || concat.Options(part2);
  }
  result -= nums.top();
  nums.pop();
  return res || Options(part2);
}

void Entry::Print() {
  std::cout << result << ": ";
  std::stack<int> copy = nums;
  while (!copy.empty()) {
    std::cout << copy.top() << ", ";
    copy.pop();
  }
  std::cout << "\n";
}

uint64_t part1(std::vector<Entry> input) {
  return std::accumulate(
      input.begin(), input.end(), (uint64_t)0,
      [](uint64_t acc, auto &e) { return acc + (e.Options() ? e.value : 0); });
}
uint64_t part2(std::vector<Entry> input) {
  return std::accumulate(input.begin(), input.end(), (uint64_t)0,
                         [](uint64_t acc, auto &e) {
                           return acc + (e.Options(true) ? e.value : 0);
                         });
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    return Entry(s);
  };
  std::vector<Entry> input = parseFileByLine("day7.txt", parse);
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %ld\n", part2(input));
  return 0;
}
