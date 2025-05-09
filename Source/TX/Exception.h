#pragma once
#include <string_view>
#include <string>

namespace TX {
class Exception : std::exception {
public:
  explicit Exception(const std::string_view &message,
                     const std::string_view &scope = "TX") {
    info.append("[");
    info.append(scope);
    info.append("] ");
    info.append(message);
    info.append("\n Stack trace:\n");
    // TODO: stacktrace
  }

  [[nodiscard]] const char *what() const noexcept override {
    return info.c_str();
  }

private:
  std::string info;
};
} // namespace TX
