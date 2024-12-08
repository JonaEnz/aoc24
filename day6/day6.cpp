#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <vector>

enum Fields { Empty, Full, Player };
enum Direction { Up, Right, Down, Left };
using Entry = std::vector<Fields>;
using Field = std::vector<Entry>;
using PlayerT = std::tuple<int, int, Direction>;

static std::map<Direction, std::pair<int, int>> dir = {
    {Up, {-1, 0}}, {Right, {0, 1}}, {Left, {0, -1}}, {Down, {1, 0}}};

std::pair<int, int> getPlayerPos(const std::vector<Entry> &field) {
  for (auto i = 0; i < field.size(); ++i) {
    for (auto j = 0; j < field[i].size(); ++j) {
      if (field[i][j] == Fields::Player) {
        return {i, j};
      }
    }
  }
  return {0, 0};
}

bool nextPlayerPos(const Field &field, PlayerT &player) {
  auto &d = dir[std::get<2>(player)];
  auto i = std::get<0>(player) + d.first;
  auto j = std::get<1>(player) + d.second;
  if (i < 0 || j < 0 || i >= field.size() || j >= field[i].size()) {
    return false;
  }
  if (field[i][j] == Fields::Full) {
    std::get<2>(player) = static_cast<Direction>((std::get<2>(player) + 1) % 4);
  } else {
    std::get<0>(player) = i;
    std::get<1>(player) = j;
  }
  return true;
}

int part1(const Field &input) {
  auto p = getPlayerPos(input);
  PlayerT player = {p.first, p.second, Direction::Up};
  std::set<std::pair<int, int>> s = {{p.first, p.second}};
  while (nextPlayerPos(input, player)) {
    s.insert({std::get<0>(player), std::get<1>(player)});
  }

  return s.size();
}

int getPlayerId(PlayerT &player) {
  return std::get<0>(player) * 10000 + std::get<1>(player) * 10 +
         std::get<2>(player);
}

bool getsStuck(Field &field, PlayerT init, PlayerT &curr,
               std::set<PlayerT> set) {
  field[std::get<0>(curr)][std::get<1>(curr)] = Fields::Full;
  auto lastPos = std::get<2>(init);
  while (nextPlayerPos(field, init)) {
    if (lastPos == std::get<2>(init)) {
      continue;
    }
    lastPos = std::get<2>(init);
    if (set.contains(init)) {
      field[std::get<0>(curr)][std::get<1>(curr)] = Fields::Empty;
      return true;
    }
    set.insert(init);
  }
  field[std::get<0>(curr)][std::get<1>(curr)] = Fields::Empty;
  return false;
}

int part2(Field &input) {
  auto p = getPlayerPos(input);
  PlayerT player = {p.first, p.second, Direction::Up};
  PlayerT init = player;
  std::set<PlayerT> set = {init};
  std::set<std::pair<int, int>> stuckSet = {};
  while (nextPlayerPos(input, player)) {
    set.insert(player);
    std::pair<int, int> playerPos = {std::get<0>(player), std::get<1>(player)};
    if (!stuckSet.contains(playerPos) && getsStuck(input, init, player, set)) {
      stuckSet.insert(playerPos);
    }
  }

  return stuckSet.size();
}

int main(int argc, char *argv[]) {
  std::function<Entry(const std::string)> parse = [](const std::string s) {
    return s | std::views::transform([](auto c) {
             switch (c) {
             case '#':
               return Fields::Full;
             case '^':
               return Fields::Player;
             default:
               return Fields::Empty;
             }
           }) |
           std::ranges::to<Entry>();
  };
  Field input = parseFileByLine("day6.txt", parse);
  printf("Part 1: %d\n", part1(input));
  printf("Part 2: %d\n", part2(input));
  return 0;
}
