#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Field = std::vector<std::vector<bool>>;

using RecordEntry = std::pair<int, int>;
class Node {
public:
  std::pair<int, int> pos;
  int dir;
  long cost;

  bool operator>(const Node &other) const {
    return pos.first + pos.second > other.pos.first + other.pos.second;
  }
  bool operator==(const Node &other) const {
    return pos == other.pos && dir == other.dir;
  }
  const RecordEntry Record() const { return pos; }
};

static std::array<std::pair<int, int>, 4> directions = {
    std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(1, 0),
    std::make_pair(0, -1)}; // TRBL

void Dijkstra(const Field &field, std::vector<Node> &start,
              std::map<RecordEntry, long> &record) {
  auto inField = [&field](const RecordEntry &pos) {
    return pos.first >= 0 && pos.second >= 0 && pos.first < field.size() &&
           pos.second < field[0].size();
  };

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
  for (auto &s : start) {
    queue.push(s);
  }
  while (!queue.empty()) {
    Node curr = queue.top();
    queue.pop();
    const auto r = curr.Record();
    if (record.contains(r) && curr.cost >= record[r])
      continue;
    record[r] = curr.cost;
    for (int j = 0; j < 4; j++) {
      auto d = directions[j];

      RecordEntry nextPos = {curr.pos.first + d.first,
                             curr.pos.second + d.second};
      if (inField(nextPos) && !field[nextPos.first][nextPos.second]) {
        queue.push(Node(nextPos, j, curr.cost + 1));
      }
    }
  }
}

long part1(const Field &field) {
  Node start = Node({0, 0}, 0, 0);
  std::map<RecordEntry, long> rec{};
  std::vector<Node> s{start};
  Dijkstra(field, s, rec);
  return rec[std::make_pair(70, 70)];
}
long part2(Field &field, std::vector<RecordEntry> &stones) {
  for (int i = 1024; i < stones.size(); i++) {
    auto p = stones[i];
    field[p.first][p.second] = true;
    Node start = Node({0, 0}, 0, 0);
    std::map<RecordEntry, long> rec{};
    std::vector<Node> s{start};
    Dijkstra(field, s, rec);
    if (!rec.contains({70, 70})) {
      return i;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  auto input = readFileLines("day18.txt");
  Field field{};
  field.resize(71);
  std::transform(field.begin(), field.end(), field.begin(),
                 [](auto &l) { return std::vector<bool>(71, false); });
  std::vector<RecordEntry> stones{};
  for (auto &l : input) {
    auto split = splitString(l, ",");
    stones.push_back({std::stoi(split[0]), std::stoi(split[1])});
  }
  for (int i = 0; i < 1024; i++) {
    auto p = stones[i];
    field[p.first][p.second] = true;
  }
  printf("Part 1: %ld\n", part1(field));
  auto i2 = part2(field, stones);
  printf("Part 2: %d,%d\n", stones[i2].first, stones[i2].second);

  return 0;
}
