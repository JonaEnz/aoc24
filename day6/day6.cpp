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
class State {
public:
  std::pair<int, int> pos;
  Direction dir;
  bool nextPlayerPos(const Field &);
  int Id();
  bool getsStuck(Field &, State init, std::set<State>);
  bool operator<(const State &other) const {
    if (dir != other.dir)
      return dir < other.dir;
    else
      return pos < other.pos;
  };
};
static std::map<Direction, std::pair<int, int>> directions = {
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

bool State::nextPlayerPos(const Field &field) {
  auto &d = directions[dir];
  auto i = pos.first + d.first;
  auto j = pos.second + d.second;
  if (i < 0 || j < 0 || i >= field.size() || j >= field[i].size()) {
    return false;
  }
  if (field[i][j] == Fields::Full) {
    dir = static_cast<Direction>((dir + 1) % 4);
  } else {
    pos = {i, j};
  }
  return true;
}

int part1(const Field &input) {
  auto p = getPlayerPos(input);
  class State player = {p, Direction::Up};
  std::set<std::pair<int, int>> s = {{p.first, p.second}};
  while (player.nextPlayerPos(input)) {
    s.insert(player.pos);
  }

  return s.size();
}

int State::Id() { return pos.first * 10000 + pos.second * 10 + dir; }

bool State::getsStuck(Field &field, State init, std::set<State> set) {
  field[pos.first][pos.second] = Fields::Full;
  auto lastPos = init.dir;
  while (init.nextPlayerPos(field)) {
    if (lastPos == init.dir) {
      continue;
    }
    lastPos = init.dir;
    if (set.contains(init)) {
      field[pos.first][pos.second] = Fields::Empty;
      return true;
    }
    set.insert(init);
  }
  field[pos.first][pos.second] = Fields::Empty;
  return false;
}

int part2(Field &input) {
  auto p = getPlayerPos(input);
  class State player = {p, Direction::Up};
  auto init = player;
  std::set<State> set = {init};
  std::set<std::pair<int, int>> stuckSet = {};
  while (player.nextPlayerPos(input)) {
    set.insert(player);
    std::pair<int, int> playerPos = player.pos;
    if (!stuckSet.contains(playerPos) && player.getsStuck(input, init, set)) {
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
