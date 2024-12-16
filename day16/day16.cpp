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

using Field = std::vector<std::vector<char>>;

using RecordEntry = std::pair<std::pair<int, int>, int>;
class Node {
public:
  std::pair<int, int> pos;
  int dir;
  long cost;
  std::set<std::pair<int, int>> path;

  bool operator>(const Node &other) const { return cost > other.cost; }
  bool operator==(const Node &other) const {
    return pos == other.pos && dir == other.dir;
  }
  const RecordEntry Record() const { return {pos, dir}; }
};

static std::array<std::pair<int, int>, 4> directions = {
    std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(1, 0),
    std::make_pair(0, -1)}; // TRBL

void Dijkstra(const Field &field, std::vector<Node> &start, Node &end,
              std::map<RecordEntry, long> &record) {
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
  for (auto &s : start) {
    s.path.insert(s.pos);
    queue.push(s);
  }
  while (!queue.empty()) {
    Node curr = queue.top();
    queue.pop();
    const auto r = curr.Record();
    if (record.contains(r) && curr.cost > record[r])
      continue;
    if (curr.pos == end.pos) {
      if (curr.cost < record[r]) {
        end.path = curr.path;
      } else {
        end.path.merge(curr.path);
      }
      record[r] = curr.cost;
    }
    record[r] = curr.cost;
    auto d = directions[curr.dir];

    queue.push(Node(curr.pos, (curr.dir + 1) % 4, curr.cost + 1000, curr.path));
    queue.push(Node(curr.pos, (curr.dir + 3) % 4, curr.cost + 1000, curr.path));
    if (field[curr.pos.first + d.first][curr.pos.second + d.second] != '#') {
      curr.path.insert(curr.pos);
      queue.push(Node({curr.pos.first + d.first, curr.pos.second + d.second},
                      curr.dir, curr.cost + 1, curr.path));
    }
  }
}

Node FindField(const Field &field, char c) {
  for (int i = 0; i < field.size(); i++) {
    for (int j = 0; j < field[0].size(); j++) {
      if (field[i][j] == c) {
        return Node({i, j}, 0, 0);
      }
    }
  }
  return {};
}
long part1(const Field &field) {
  Node start = FindField(field, 'S');
  start.dir = 1;
  std::map<RecordEntry, long> rec{};
  std::vector<Node> s{start};
  auto e = FindField(field, 'E');
  Dijkstra(field, s, e, rec);
  // for (auto &e : rec) {
  //   std::cout << e.first.first.first << "," << e.first.first.second << ","
  //             << e.first.second << ":" << e.second << "\n";
  // }
  auto result = 99999999;
  for (auto &n : rec | std::views::filter(
                           [&e](auto &r) { return r.first.first == e.pos; })) {
    if (n.second < result) {
      result = n.second;
    }
  }

  printf("Part 2: %ld\n", e.path.size() + 1);
  return result;
}
long part2(const Field &input) { return 0; }

int main(int argc, char *argv[]) {
  auto input = readFileLines("day16.txt");
  Field field{};
  field.resize(input.size());
  std::transform(input.begin(), input.end(), field.begin(),
                 [](auto &l) { return std::vector<char>(l.begin(), l.end()); });
  printf("Part 1: %ld\n", part1(field));
  return 0;
}
