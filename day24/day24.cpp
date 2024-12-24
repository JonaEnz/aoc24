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
int part2(Input &input) {
  auto setAdd = [&input](long a, long b) {
    for (auto index = 0; true; index++) {
      auto k1 =
          "x" + std::string(index < 10 ? "0" : "") + std::to_string(index);
      auto k2 =
          "y" + std::string(index < 10 ? "0" : "") + std::to_string(index);
      if (!input.contains(k1) || !input.contains(k2)) {
        return;
      }
      input[k1]->value = (std::byte)(a & 0x1);
      input[k2]->value = (std::byte)(b & 0x1);
      b >>= 1;
      a >>= 1;
    }
  };
  for (long i = 1; i < ((long)1 << 60); i <<= 1) {
    setAdd(i - 1, 1);
    auto plus = part1(input);
    if (plus != i)
      std::cout << i - 1 << "+1=" << plus << "\n";
  }
  // Just a helper, solved manually with graphviz
  return 0;
}

int main(int argc, char *argv[]) {
  auto lines = readFileLines("day24.txt");
  Input input{};
  auto split = std::ranges::find(lines, "");
  for (auto it = lines.begin(); it != split; it++) {
    auto s = splitString(*it, ": ");
    input[s[0]] = std::make_shared<Bit>(nullptr, nullptr, Op::OR,
                                        (std::byte)std::stoi(s[1]));
  }
  split++;
  for (auto it = split; it != lines.end(); it++) {
    auto s = splitString(*it, " ");
    input[s[4]] = std::make_shared<Bit>();
  }
  for (auto it = split; it != lines.end(); it++) {
    auto s = splitString(*it, " ");
    auto bit = input[s[4]];
    bit->left = input[s[0]];
    if (s[1] == "XOR") {
      bit->op = Op::XOR;
    } else if (s[1] == "OR") {
      bit->op = Op::OR;
    } else if (s[1] == "AND") {
      bit->op = Op::AND;
    }
    bit->right = input[s[2]];
    bit->value = (std::byte)0x2;
  }

  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
