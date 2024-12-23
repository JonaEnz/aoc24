#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

using Graph = std::map<std::string, std::vector<std::string>>;

std::vector<std::vector<std::string>> is3Clique(Graph &g) {
  std::vector<std::vector<std::string>> result{};
  std::vector<std::string> keys{};
  for (auto &[k, _] : g)
    keys.push_back(k);
  auto isConnected = [&g](auto &a, auto &b) {
    return std::ranges::find(g[a], b) != g[a].end();
  };
  for (auto i = 0; i < keys.size(); i++) {
    for (auto j = i + 1; j < keys.size(); j++) {
      if (!isConnected(keys[i], keys[j]))
        continue;
      for (auto k = j + 1; k < keys.size(); k++) {
        if (isConnected(keys[j], keys[k]) && isConnected(keys[i], keys[k]))
          result.push_back({keys[i], keys[j], keys[k]});
      }
    }
  }
  return result;
}

void BronKerbosch(Graph &g, std::vector<std::string> &R,
                  std::vector<std::string> &P, std::vector<std::string> &X,
                  std::vector<std::string> &o) {
  if (P.size() == 0 && X.size() == 0 && R.size() > o.size()) {
    o = R;
  }
  for (auto &v : P) {
    auto R2 = R;
    auto P2 = P;
    auto X2 = X;
    R2.push_back(v);
    auto &N = g[v];
    P2.erase(std::remove_if(
                 P2.begin(), P2.end(),
                 [&N](auto &e) { return std::ranges::find(N, e) == N.end(); }),
             P2.end());
    X2.erase(std::remove_if(
                 X2.begin(), X2.end(),
                 [&N](auto &e) { return std::ranges::find(N, e) == N.end(); }),
             X2.end());

    BronKerbosch(g, R2, P2, X2, o);
    P.erase(
        std::remove_if(P.begin(), P.end(), [&v](auto &e) { return v == e; }),
        P.end());
    X.push_back(v);
  }
}

long part1(Graph &g) {
  auto v = is3Clique(g);
  auto startsWithT = [](auto &s) { return s[0] == 't'; };
  return std::ranges::count_if(v, [&startsWithT](auto a) {
    return startsWithT(a[0]) || startsWithT(a[1]) || startsWithT(a[2]);
  });
}

std::string part2(Graph &g) {
  std::vector<std::string> R{};
  std::vector<std::string> P{};
  for (auto &[k, _] : g)
    P.push_back(k);
  std::vector<std::string> X{};
  std::vector<std::string> o{};
  BronKerbosch(g, R, P, X, o);
  std::ranges::sort(o);
  std::string s{};
  for (auto &s2 : o)
    s += s2 + ",";
  s.erase(s.begin() + s.size() - 1, s.end());
  return s;
}

int main(int argc, char *argv[]) {
  std::function<std::pair<std::string, std::string>(const std::string)> parse =
      [](const std::string s) {
        auto split = splitString(s, "-");
        return std::pair<std::string, std::string>{split[0], split[1]};
      };
  std::vector<std::pair<std::string, std::string>> vec =
      parseFileByLine("day23.txt", parse);
  std::map<std::string, std::vector<std::string>> input{};
  for (auto &[l, r] : vec) {
    if (input.contains(l))
      input[l].push_back(r);
    else
      input[l] = {r};
    if (input.contains(r))
      input[r].push_back(l);
    else
      input[r] = {l};
  }
  printf("Part 1: %ld\n", part1(input));
  printf("Part 2: %s\n", part2(input).c_str());
  return 0;
}
