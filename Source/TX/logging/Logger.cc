#include "TX/logging/Logger.h"
#include "TX/Exception.h"
#include "fmt/base.h"
#ifdef _WIN32
#else
#include <cstdlib>
#endif

template <>
struct fmt::formatter<TX::logging::Logger::Level> : formatter<string_view> {
  format_context::iterator format(TX::logging::Logger::Level level,
                                  format_context &ctx) const {
    string_view name = "U";
    switch (level) {
    case TX::logging::Logger::Level::Trace:
      name = "T";
      break;
    case TX::logging::Logger::Level::Debug:
      name = "D";
      break;
    case TX::logging::Logger::Level::Info:
      name = "I";
      break;
    case TX::logging::Logger::Level::Warn:
      name = "W";
      break;
    case TX::logging::Logger::Level::Error:
      name = "E";
      break;
    case TX::logging::Logger::Level::Fatal:
      name = "F";
      break;
    }
    return formatter<string_view>::format(name, ctx);
  }
};

namespace TX::logging {
static Logger::Reporter GLOBAL_REPORTER;
static Logger::Formatter GLOBAL_FORMATTER;
Logger::Level Logger::level_ = Logger::Level::Debug;
Logger::Reporter *Logger::reporter_ = &GLOBAL_REPORTER;
Logger::Formatter *Logger::formatter_ = &GLOBAL_FORMATTER;

void Logger::Log::clean() {
#ifdef TPKit_DIR
  file = file.substr(sizeof TPKit_DIR);
#endif
}

void Logger::Log::output() const {
  Logger::reporter_->report(*this);
  if (level == Logger::Level::Fatal)
    abort();
}

void Logger::Log::throwException() const { throw Exception(message, scope); }

std::string Logger::Formatter::format(const Logger::Log &log) {
  return fmt::format("[{:s}][{:d}][{:s}][{:s}:{:d}][{:s}] {}", log.level,
                     log.time.unixNano(), log.scope, log.file, log.line,
                     log.function, log.message);
}

void Logger::Reporter::report(const Logger::Log &log) {
  fmt::println("{:s}", Logger::formatter_->format(log));
}
} // namespace TX::logging
