#include "util.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

std::string readFile(const std::string path) {
  auto inputFile = std::filesystem::path(path);
  std::ifstream file(inputFile, std::ios::in);
  std::string s;
  auto fsize = std::filesystem::file_size(path);
  s.resize(fsize);
  file.read(s.data(), fsize);
  return s;
}

std::vector<std::string> readFileLines(const std::string path) {
  auto rawInput = readFile(path);
  std::vector<std::string> input{};
  size_t pos = 0;
  while ((pos = rawInput.find('\n')) != std::string::npos) {
    input.push_back(rawInput.substr(0, pos));
    rawInput.erase(0, pos + 1);
  }
  return input;
}
