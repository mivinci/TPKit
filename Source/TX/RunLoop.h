#pragma once
#include <unordered_set>

#include "TX/Condvar.h"
#include "TX/Duration.h"
#include "TX/Mutex.h"
#include "TX/Thread.h"

namespace TX {
class RunLoop final {
 public:
  explicit RunLoop() = default;

  enum class Status { Finished, Timeout, Stopped };
  enum class Activity : uint8_t {
    Entry = 1U << 0,
    BeforeTimers = 1U << 1,
    BeforeSources = 1U << 2,
    BeforeWaiting = 1U << 3,
    AfterWaiting = 1U << 4,
    Exit = 1U << 5,
    All = 0xFF,
  };

  Status Run(Duration timeout = Duration::FOREVER,
             StringView scope_name = Scope::Default);
  void Stop();
  void Wakeup();

  class Observer {
   public:
    explicit Observer(RunLoop *run_loop, const bool once = false,
                      const Activity activities = Activity::All)
        : run_loop_(run_loop), activities_(activities), once_(once) {}
    virtual ~Observer() = default;
    virtual void OnActivity(RunLoop *, Activity) = 0;

   private:
    RunLoop *run_loop_;
    Activity activities_;
    bool once_;
  };

  class Scope;

  class EventObserver {
   public:
    virtual ~EventObserver() = default;
    virtual void OnSchedule(RunLoop *, Scope *) = 0;
    virtual void OnCancel(RunLoop *, Scope *) = 0;
    virtual void OnPerform(RunLoop *, Scope *) = 0;
  };

  class Source : public EventObserver {
   public:
    void Signal();
    void Clear();
    TX_NODISCARD uint64_t SignaledTime() const;
    TX_NODISCARD bool IsSignaled() const { return SignaledTime() != 0; }

   private:
    std::atomic<uint64_t> signaled_time_;
  };

  class Timer : public EventObserver {
   public:
    explicit Timer(const Time deadline, const Duration period)
        : deadline_(deadline), period_(period) {}

   private:
    Time deadline_;
    Duration period_;
  };

  class Scope {
   public:
    explicit Scope(const StringView &name, RunLoop *run_loop)
        : name_(name), run_loop_(run_loop) {}

    TX_NODISCARD Duration Timeout(const Option<Time>& now = None) const;
    static StringView Default;

   private:
    friend RunLoop;
    struct Shared {
      bool stopped = false;
      std::unordered_set<Source *> source_set_;
      std::unordered_set<Timer *> timer_set_;
      std::unordered_set<Observer *> observer_set_;
    };
    Mutex<Shared> shared_;
    StringView name_;
    RunLoop *run_loop_;
  };

  void AddSource(Source *source, const StringView &scope_name);
  void RemoveSource(Source *source, const StringView &scope_name);

  void AddTimer(Timer *timer, const StringView &scope_name);
  void RemoveTimer(Timer *timer, const StringView &scope_name);

  void AddObserver(Observer *observer, const StringView &scope_name);
  void RemoveObserver(Observer *observer, const StringView &scope_name);

  static RunLoop *Current() { return FromThreadId(Thread::Current()); }
  static RunLoop *Main() { return FromThreadId(Thread::Main()); }

 private:
  struct Shared {
    std::unordered_map<StringView, Own<Scope>> mode_map_;
    Scope *current_mode_ = nullptr;
  };

  static RunLoop *FromThreadId(const Thread::Id &id);
  Status RunSpec(Scope *scope, Duration timeout);
  TX_NODISCARD Scope *GetScope(const StringView &name, bool create);
  TX_NODISCARD Scope *GetScopeLocked(const StringView &name, bool create,
                                     MutexGuard<Shared> &guard);
  void DoObservers(const Scope *scope, Activity activity) const;

 private:
  Mutex<Shared> shared_;
  Condvar cond_;
};
}  // namespace TX
