#include <algorithm>
#include <functional>
#include <string>
#include <vector>

std::string readFile(const std::string);
std::vector<std::string> readFileLines(const std::string);
std::vector<std::string> splitString(const std::string, const std::string);

template <typename T>
std::vector<T>
parseFileByLine(const std::string path,
                std::function<T(const std::string)> parseLineFunc) {
  auto lines = readFileLines(path);
  std::vector<T> output{};
  output.resize(lines.size());
  std::transform(lines.begin(), lines.end(), output.begin(), parseLineFunc);
  return output;
}
