#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

enum Direction { Up = 0, Left, Down, Right };
using Field = std::vector<std::vector<char>>;

Direction parseChar(char c) {
  switch (c) {
  case '^':
    return Direction::Up;
  case '<':
    return Direction::Left;
  case '>':
    return Direction::Right;
  case 'v':
  default:
    return Direction::Down;
  }
}

std::pair<int, int> findRobot(const Field &field) {
  for (auto i = 0; i < field.size(); i++) {
    for (auto j = 0; j < field[i].size(); j++) {
      if (field[i][j] == '@')
        return {i, j};
    }
  }
  return {-1, -1};
}

long BoxGPSScore(const Field &field) {
  long result = 0;
  for (auto i = 0; i < field.size(); i++) {
    for (auto j = 0; j < field[i].size(); j++) {
      if (field[i][j] == 'O' || field[i][j] == '[')
        result += 100 * i + j;
    }
  }
  return result;
}

bool tryPushBlock(Field &field, std::pair<int, int> &p, Direction d) {
  auto c = field[p.first][p.second];
  std::pair<int, int> next = p;
  switch (d) {
  case Direction::Up:
    if (next.first > 0)
      next.first--;
    break;
  case Direction::Left:
    if (next.second > 0)
      next.second--;
    break;
  case Direction::Right:
    if (next.second < field[0].size() - 1)
      next.second++;
    break;
  case Direction::Down:
    if (next.first < field.size() - 1)
      next.first++;
    break;
  }
  if (next == p || field[next.first][next.second] == '#') {
    return false;
  }
  auto nextCopy = next;
  if ((int)d % 2 == 0) {
    if (field[next.first][next.second] == '[') {
      std::pair<int, int> other = {next.first, next.second + 1};
      if (other != p && !tryPushBlock(field, other, d)) {
        return false;
      }
    } else if (field[next.first][next.second] == ']') {
      std::pair<int, int> other = {next.first, next.second - 1};
      if (other != p && !tryPushBlock(field, other, d)) {
        return false;
      }
    }
  }
  if (field[next.first][next.second] == '.' ||
      tryPushBlock(field, nextCopy, d)) {
    field[next.first][next.second] = c;
    field[p.first][p.second] = '.';
    p = next;
    return true;
  }
  return false;
}

long part1(Field field, std::queue<Direction> queue) {
  auto robot = findRobot(field);
  while (!queue.empty()) {
    tryPushBlock(field, robot, queue.front());
    queue.pop();
  }
  return BoxGPSScore(field);
}

long part2(Field field, std::queue<Direction> queue) {
  auto robot = findRobot(field);
  while (!queue.empty()) {
    auto newField = field;
    if (tryPushBlock(newField, robot, queue.front()))
      field = newField;
    queue.pop();
  }
  return BoxGPSScore(field);
}

int main(int argc, char *argv[]) {
  auto lines = readFileLines("day15.txt");
  Field field{};
  std::queue<Direction> moveQueue{};
  auto it = lines.begin();
  while ((*it).size() > 0) {
    field.push_back(std::vector<char>(it->begin(), it->end()));
    ++it;
  }
  it++;
  for (; it != lines.end(); it++) {
    std::ranges::for_each(
        *it, [&moveQueue](auto c) { moveQueue.push(parseChar(c)); });
  }
  printf("Part 1: %ld\n", part1(field, moveQueue));
  std::map<char, std::string> m = {
      {'#', "##"}, {'O', "[]"}, {'.', ".."}, {'@', "@."}};
  std::transform(field.begin(), field.end(), field.begin(), [&m](auto l) {
    std::string s{};
    std::ranges::for_each(l, [&s, &m](char c) { s.append(m[c]); });
    return std::vector<char>(s.begin(), s.end());
  });
  printf("Part 2: %ld\n", part2(field, moveQueue));
  return 0;
}
