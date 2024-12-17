#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::pair<int, int>;

class Program {
public:
  std::array<long, 3> state;
  std::vector<long> instructions;
  std::vector<long> output;
  int pointer;
  bool Run();
  long Combo(int n) const;
};

long Program::Combo(int n) const {
  switch (n) {
  case 0:
  case 1:
  case 3:
    return n;
  case 4:
  case 5:
  case 6:
    return state[n - 4];
  }
  return 0;
}

bool Program::Run() {
  if (instructions.size() <= pointer + 1)
    return false;
  auto literal = instructions[pointer + 1];
  long combo = Combo(literal);
  switch (instructions[pointer]) {
  case 0: // adv
    state[0] /= std::pow(2, combo);
    break;
  case 1: // bxl
    state[1] ^= literal;
    break;
  case 2: // bst
    state[1] = combo % 8;
    break;
  case 3: // jnz
    if (state[0] != 0) {
      pointer = literal;
      return true;
    }
    break;
  case 4: // bxc
    state[1] ^= state[2];
    break;
  case 5: // out
    output.push_back(combo % 8);
    break;
  case 6: // bdv
    state[1] = state[0] / std::pow(2, combo);
    break;
  case 7: // cdv
  default:
    state[2] = state[0] / std::pow(2, combo);
    break;
  }
  pointer += 2;
  return true;
}

std::string part1(Program p) {
  std::string out = "";
  while (p.Run()) {
  }
  out = "";
  for (auto i : p.output) {
    out += std::to_string(i) + ",";
  }
  out.resize(out.size() - 1);
  return out;
}
long part2(Program p) {
  std::vector<long> answer = {};
  long result = 0;
  auto compLen = 1;
  std::queue<std::pair<long, int>> candidates = {};
  candidates.push(std::make_pair((long)0, 1));
  while (!candidates.empty()) {
    std::tie(result, compLen) = candidates.front();
    candidates.pop();
    const auto searchFor =
        std::vector<long>(p.instructions.end() - compLen, p.instructions.end());
    for (long i = 0; i < 8; ++i) {
      Program pCopy = p;
      pCopy.state = {result + i, 0, 0};
      while (pCopy.Run()) {
      }
      if (searchFor == pCopy.output) {
        if (compLen == p.instructions.size()) {
          answer.push_back(result + i);
        } else {
          candidates.push(std::make_pair((result + i) * 8, compLen + 1));
        }
      }
    }
  }
  return *std::min_element(answer.begin(), answer.end());
}

int main(int argc, char *argv[]) {
  Program program{};
  program.state = {0, 0, 0};
  // program.state = {<Input here>, 0, 0};

  // program.instructions = {Your Input Here!};
  program.instructions = {0, 0};

  printf("Part 1: %s\n", part1(program).c_str());
  printf("Part 2: %ld\n", part2(program));
  return 0;
}
