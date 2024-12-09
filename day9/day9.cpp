#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <util.h>
#include <utility>
#include <vector>

class Part {
public:
  int id;
  int startIndex;
  int length;
  long Checksum() {
    long n = 0;
    for (int i = 0; i < length; i++) {
      n += (long)startIndex + (long)i;
    }
    return id * n;
  }
};

using ParsedInput = std::pair<std::vector<Part>, std::vector<Part>>;

ParsedInput parse(const std::vector<int> &input) {
  auto index = 0;
  auto totalFileSize = 0;
  std::vector<Part> files{};
  std::vector<Part> freeSpace{};
  auto id = 0;
  for (auto i = 0; i < input.size(); i += 2) {
    files.push_back({id, index, input[i]});
    totalFileSize += input[i];
    index += input[i];
    id++;
    if (i + 1 < input.size() && input[i + 1] != 0) {
      freeSpace.push_back({0, index, input[i + 1]});
      index += input[i + 1];
    }
  }
  return {files, freeSpace};
}

long part1(ParsedInput &input) {

  std::vector<Part> files = input.first;
  std::queue<Part> freeSpace(input.second.begin(), input.second.end());

  auto mergeBlocks = [](auto &file, auto &free) -> Part {
    if (free.length >= file.length) {
      Part p = {file.id, free.startIndex, file.length};
      free.length -= file.length;
      free.startIndex += file.length;
      file.length = 0;
      return p;
    } else {
      Part p = {file.id, free.startIndex, free.length};
      file.length -= free.length;
      free.length = 0;
      return p;
    }
  };
  std::vector<Part> newFiles{};
  auto i = 0;
  auto free = freeSpace.front();
  freeSpace.pop();
  auto &file = files[files.size() - 1];
  while (freeSpace.size() > 0 && files.size() > 0 &&
         file.startIndex >= free.startIndex + free.length) {
    newFiles.push_back(mergeBlocks(file, free));
    if (file.length == 0) {
      files.resize(files.size() - 1);
      file = files[files.size() - 1];
    }
    if (free.length == 0) {
      free = freeSpace.front();
      freeSpace.pop();
    }
  }
  files[files.size() - 1] = file;
  files.reserve(files.size() + newFiles.size());
  files.insert(files.end(), newFiles.begin(), newFiles.end());

  return std::accumulate(files.begin(), files.end(), (long)0,
                         [](auto acc, auto &p) { return acc + p.Checksum(); });
}

long part2(ParsedInput &input) {
  std::vector<Part> files = input.first;
  std::vector<Part> freeSpace = input.second;

  auto defrag = [&freeSpace](Part &file) {
    for (auto it = freeSpace.begin(); it != freeSpace.end(); it++) {
      if (file.startIndex < it->startIndex) {
        break;
      }
      if (file.length <= it->length) {
        it->length -= file.length;
        file.startIndex = it->startIndex;
        it->startIndex += file.length;
        break;
      }
    }
  };

  std::for_each(files.rbegin(), files.rend(), defrag);
  return std::accumulate(files.begin(), files.end(), (long)0,
                         [](auto acc, auto &p) { return acc + p.Checksum(); });
}

int main(int argc, char *argv[]) {
  std::string input = readFile("day9.txt");
  input.erase(input.begin() + input.size() - 1, input.end());
  auto nums = input | std::views::transform([](auto &c) {
                return std::stoi(std::string(1, c));
              }) |
              std::ranges::to<std::vector<int>>();
  auto parsedInput = parse(nums);
  printf("Part 1: %ld\n", part1(parsedInput));
  printf("Part 2: %ld\n", part2(parsedInput));
  return 0;
}
