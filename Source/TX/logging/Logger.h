#pragma once
#include "TX/time/Time.h"
#include "fmt/printf.h"
#include <string_view>
#include <utility>

namespace TX::logging {
#define TX_LOG(level, scope, ...)                                              \
  for (bool shouldLog =                                                        \
           TX::logging::Logger::shouldLog(TX::logging::Logger::Level::level);  \
       shouldLog; shouldLog = false)                                           \
  TX::logging::Logger::Log(TX::logging::Logger::Level::level, __FILE__,        \
                           __LINE__, __FUNCTION__, scope, __VA_ARGS__)         \
      .output()

#define TX_THROW(...)                                                          \
  for (TX::logging::Logger::Log log(TX::logging::Logger::Level::Error,         \
                                    __FILE__, __LINE__, __FUNCTION__, "throw", \
                                    __VA_ARGS__);                              \
       ; log.throwException())

#define TX_DEBUG(...) TX_LOG(Debug, "TX", __VA_ARGS__)
#define TX_INFO(...) TX_LOG(Info, "TX", __VA_ARGS__)
#define TX_WARN(...) TX_LOG(Warn, "TX", __VA_ARGS__)
#define TX_ERROR(...) TX_LOG(Error, "TX", __VA_ARGS__)
#define TX_FATAL(...) TX_LOG(Fatal, "TX", __VA_ARGS__)

#define TX_DEBUG_SCOPE(scope, ...) TX_LOG(Debug, scope, __VA_ARGS__)
#define TX_INFO_SCOPE(scope, ...) TX_LOG(Info, scope, __VA_ARGS__)
#define TX_WARN_SCOPE(scope, ...) TX_LOG(Warn, scope, __VA_ARGS__)
#define TX_ERROR_SCOPE(scope, ...) TX_LOG(Error, scope, __VA_ARGS__)
#define TX_FATAL_SCOPE(scope, ...) TX_LOG(Fatal, scope, __VA_ARGS__)

class Logger {
public:
  enum class Level { Trace, Debug, Info, Warn, Error, Fatal };
  class Log {
  public:
    template <typename... Args>
    explicit Log(Level level, const char *file, int line, const char *function,
                 const char *scope, const char *format, Args &&...args)
        : level(level), line(line), file(file), scope(scope),
          function(function) {
      message = fmt::sprintf(format, args...);
      clean();
    }
    explicit Log(Level level, const char *file, int line, const char *function,
                 const char *scope)
        : level(level), line(line), file(file), scope(scope),
          function(function) {
      clean();
    }
    Log(Log &) = delete;
    void output() const;
    void throwException() const;

  private:
    void clean();

  public:
    Level level;
    int line;
    std::string_view file;
    std::string_view scope;
    std::string_view function;
    std::string message;
    TX::time::Time time;
  };

  class Reporter {
  public:
    virtual void report(const Log &);
  };

  class Formatter {
  public:
    virtual std::string format(const Log &);
  };

  static Logger::Level level_;
  static Logger::Reporter *reporter_;
  static Logger::Formatter *formatter_;
  static void setLevel(Logger::Level level) { level_ = level; }
  static void setReporter(Reporter *r) { reporter_ = r; }
  static void setFormatter(Formatter *f) { formatter_ = f; }
  static bool shouldLog(Logger::Level level) { return level >= level_; }
};
} // namespace TX::logging
