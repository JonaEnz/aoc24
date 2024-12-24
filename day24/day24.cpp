#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

enum Op { OR, XOR, AND };

typedef struct Bit {
  std::shared_ptr<Bit> left;
  std::shared_ptr<Bit> right;
  Op op;
  std::byte value;
  std::string id;
  std::vector<std::shared_ptr<Bit>> in;
} Bit;
using Input = std::map<std::string, std::shared_ptr<Bit>>;

std::byte getValue(std::shared_ptr<Bit> b) {
  if (b->value != (std::byte)0x2) {
    return b->value;
  }
  std::byte v;
  switch (b->op) {
  case Op::OR:
    v = getValue(b->left) | getValue(b->right);
    break;
  case Op::XOR:
    v = getValue(b->left) ^ getValue(b->right);
    break;
  case Op::AND:
    v = getValue(b->left) & getValue(b->right);
    break;
  }
  // b->value = v;
  return v;
}

long part1(Input &input) {
  long res = 0;
  auto index = 0;
  while (true) {
    auto k = "z" + std::string(index < 10 ? "0" : "") + std::to_string(index);
    if (input.contains(k)) {
      res += (long)getValue(input[k]) << index;
    } else {
      return res;
    }
    index++;
  }
}
std::string part2(Input &input) {

  auto badBit = [](std::shared_ptr<Bit> bit) {
    if (bit->left == nullptr || bit->right == nullptr) {
      return false;
    }
    if (bit->id.starts_with("z") && bit->op != Op::XOR) {
      return true;
    }
    if (bit->op == Op::XOR && !bit->id.starts_with("z") &&
        !bit->left->id.starts_with("y") && !bit->right->id.starts_with("y")) {
      return true;
    }
    if (bit->op == Op::XOR &&
        !std::ranges::any_of(bit->in,
                             [](auto &b) { return b->id.starts_with("z"); }) &&
        (bit->left->id.starts_with("y") || bit->right->id.starts_with("y"))) {
      return true;
    }
    if (bit->op == Op::AND &&
        std::ranges::any_of(bit->in,
                            [](auto &b) { return b->id.starts_with("z"); }) &&
        (bit->left->id.starts_with("y") || bit->right->id.starts_with("y"))) {
      return true;
    }
    if (bit->op == Op::OR &&
        (bit->left->id.starts_with("y") || bit->right->id.starts_with("y"))) {
      return true;
    }
    return false;
  };
  auto badNodes = std::views::filter(
                      input, [&badBit](auto &b) { return badBit(b.second); }) |
                  std::views::transform([](auto &a) { return a.second->id; }) |
                  std::ranges::to<std::vector<std::string>>();
  std::erase_if(badNodes, [&input, badNodes](auto &b) {
    return input[b]->left->id == "y00" || input[b]->right->id == "y00" ||
           input[b]->id == "z45" ||
           std::ranges::any_of(input[b]->in,
                               [](auto &b2) { return b2->id == "z45"; }) ||
           std::ranges::any_of(input[b]->in, [&badNodes, &input](auto &i) {
             return std::ranges::find(badNodes, i->id) != badNodes.end();
           });
  });
  std::ranges::sort(badNodes);
  auto s = std::accumulate(badNodes.begin(), badNodes.end(), std::string{},
                           [](auto acc, auto &b) { return acc + b + ","; });
  s.pop_back();
  return s;
}

int main(int argc, char *argv[]) {
  auto lines = readFileLines("day24.txt");
  Input input{};
  auto split = std::ranges::find(lines, "");
  for (auto it = lines.begin(); it != split; it++) {
    auto s = splitString(*it, ": ");
    input[s[0]] = std::make_shared<Bit>(nullptr, nullptr, Op::OR,
                                        (std::byte)std::stoi(s[1]), s[0],
                                        std::vector<std::shared_ptr<Bit>>{});
  }
  split++;
  for (auto it = split; it != lines.end(); it++) {
    auto s = splitString(*it, " ");
    input[s[4]] =
        std::make_shared<Bit>(nullptr, nullptr, Op::OR, (std::byte)0, s[4],
                              std::vector<std::shared_ptr<Bit>>{});
  }
  for (auto it = split; it != lines.end(); it++) {
    auto s = splitString(*it, " ");
    auto bit = input[s[4]];
    bit->left = input[s[0]];
    bit->left->in.push_back(bit);
    if (s[1] == "XOR") {
      bit->op = Op::XOR;
    } else if (s[1] == "OR") {
      bit->op = Op::OR;
    } else if (s[1] == "AND") {
      bit->op = Op::AND;
    }
    bit->right = input[s[2]];
    bit->right->in.push_back(bit);
    bit->value = (std::byte)0x2;
  }

  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %s\n", part2(input).c_str());
  return 0;
}
