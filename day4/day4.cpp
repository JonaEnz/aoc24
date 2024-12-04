#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <numeric>
#include <ranges>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Entry = std::vector<char>;

std::vector<std::pair<int, int>> findChar(const std::vector<Entry> &field,
                                          char c) {
  std::vector<std::pair<int, int>> res{};
  for (auto i = 0; i < field.size(); ++i) {
    for (auto j = 0; j < field[i].size(); ++j) {
      if (c == field[i][j]) {
        res.push_back({i, j});
      }
    }
  }
  return res;
}

bool searchInDir(const std::vector<Entry> &field, int i, int j, int dirx,
                 int diry, std::string &s) {
  i += dirx;
  j += diry;
  if (field[i][j] != s[0]) {
    return false;
  }
  s.erase(0, 1);
  if (s.size() == 0) {
    return true;
  }
  return searchInDir(field, i, j, dirx, diry, s);
};

int checkFieldFor(const std::vector<Entry> &field, int i, int j,
                  std::string search) {
  std::vector<std::pair<int, int>> dirs{{1, 0},  {-1, 0}, {-1, -1}, {0, 1},
                                        {0, -1}, {-1, 1}, {1, -1},  {1, 1}};
  auto isMoveValid = [&field](int x, int y, int dirx, int diry, int n) {
    return (0 <= x + n * dirx) && (field.size() > x + dirx * n) &&
           (0 <= y + n * diry) && (field[0].size() > y + diry * n);
  };
  auto r = dirs | std::views::filter([i, j, &search, &isMoveValid](auto &d) {
             return isMoveValid(i, j, d.first, d.second, search.size());
           }) |
           std::views::transform([&field, i, j, search](auto &d) {
             std::string s = search;
             return searchInDir(field, i, j, d.first, d.second, s);
           });
  return std::count_if(r.begin(), r.end(), [](auto b) { return b; });
}

bool isXmas(const std::vector<Entry> &field, int i, int j) {
  if (i == 0 || j == 0 || i == field.size() - 1 || j == field[0].size() - 1) {
    return false;
  }
  auto checkTwo = [&field](int i1, int j1, int i2, int j2) {
    return (field[i1][j1] == 'M' && field[i2][j2] == 'S') ||
           (field[i1][j1] == 'S' && field[i2][j2] == 'M');
  };
  return checkTwo(i - 1, j - 1, i + 1, j + 1) &&
         checkTwo(i - 1, j + 1, i + 1, j - 1);
}

int part1(std::vector<Entry> input) {
  auto p = findChar(input, 'X');
  return std::accumulate(p.begin(), p.end(), 0, [&input](int acc, auto ij) {
    return acc + checkFieldFor(input, ij.first, ij.second, "MAS");
  });
}
int part2(std::vector<Entry> input) {
  auto p = findChar(input, 'A');
  return std::accumulate(p.begin(), p.end(), 0, [&input](int acc, auto ij) {
    return acc + (isXmas(input, ij.first, ij.second) ? 1 : 0);
  });
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    return Entry{s.begin(), s.end()};
  };
  std::vector<Entry> input = parseFileByLine("day4.txt", parse);
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
