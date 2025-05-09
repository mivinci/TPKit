#include <TX/Mutex.h>
#include <TX/RunLoop.h>

#include <unordered_map>

namespace TX {
struct RunLoopGlobalContext {
  std::unordered_map<Thread::Id, RunLoop *> runloop_map;
};
static Mutex<RunLoopGlobalContext> runLoopGlobalContext;

RunLoop *RunLoop::FromThreadId(const Thread::Id &id) {
  auto global_context = runLoopGlobalContext.Lock();
  if (const auto it = global_context->runloop_map.find(id);
      it != global_context->runloop_map.end())
    return it->second;
  const auto runloop = new RunLoop();
  global_context->runloop_map[id] = runloop;
  return runloop;
}

RunLoop::Scope *RunLoop::GetScopeLocked(const StringView &name,
                                        const bool create,
                                        MutexGuard<Shared> &guard) {
  if (const auto it = guard->mode_map_.find(name); it != guard->mode_map_.end())
    return it->second.Leak();
  if (!create) return nullptr;
  auto scope = new Scope(name, this);
  guard->mode_map_.emplace(name, scope);
  return scope;
}

RunLoop::Scope *RunLoop::GetScope(const StringView &name, bool create) {
  auto shared = shared_.Lock();
  return GetScopeLocked(name, create, shared);
}

RunLoop::Status RunLoop::Run(const Duration timeout,
                             const StringView scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, false, shared);
  if (!scope) return Status::Finished;
  Scope *previous_scope = shared->current_mode_;
  shared->current_mode_ = scope;
  Drop(shared);
  const Status status = RunSpec(scope, timeout);
  shared = shared_.Lock();
  shared->current_mode_ = previous_scope;
  return status;
}

RunLoop::Status RunLoop::RunSpec(Scope *scope, Duration timeout) {
  if (auto scope_locked = scope->shared_.Lock(); scope_locked->stopped)
    return Status::Stopped;
  // TODO
  DoObservers(scope, Activity::Entry);
  DoObservers(scope, Activity::BeforeSources);
  DoObservers(scope, Activity::BeforeWaiting);
  DoObservers(scope, Activity::AfterWaiting);
  DoObservers(scope, Activity::BeforeTimers);
  return Status::Finished;
}

void RunLoop::Stop() {}

void RunLoop::Wakeup() { cond_.NotifyOne(); }

void RunLoop::AddSource(Source *source, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, true, shared);
  TX_ASSERT(scope, "GetModeLocked failed to create a new RunLoop scope");
  scope->shared_.Lock()->source_set_.insert(source);
}

void RunLoop::RemoveSource(Source *source, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, false, shared);
  if (!scope) return;
  scope->shared_.Lock()->source_set_.erase(source);
}

void RunLoop::AddTimer(Timer *timer, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, true, shared);
  TX_ASSERT(scope, "GetModeLocked failed to create a new RunLoop scope");
  scope->shared_.Lock()->timer_set_.insert(timer);
}

void RunLoop::RemoveTimer(Timer *timer, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, false, shared);
  if (!scope) return;
  scope->shared_.Lock()->timer_set_.erase(timer);
}

void RunLoop::AddObserver(Observer *observer, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, true, shared);
  TX_ASSERT(scope, "GetModeLocked failed to create a new RunLoop scope");
  scope->shared_.Lock()->observer_set_.insert(observer);
}

void RunLoop::RemoveObserver(Observer *observer, const StringView &scope_name) {
  auto shared = shared_.Lock();
  Scope *scope = GetScopeLocked(scope_name, false, shared);
  if (!scope) return;
  scope->shared_.Lock()->observer_set_.erase(observer);
}

void RunLoop::DoObservers(const Scope *scope, const Activity activity) const {}

void RunLoop::Source::Signal() {
  uint64_t expected = 0;
  signaled_time_.compare_exchange_strong(expected, Now().UnixNano(),
                                         std::memory_order_acq_rel,
                                         std::memory_order_relaxed);
}

void RunLoop::Source::Clear() {
  signaled_time_.store(0, std::memory_order_release);
}

uint64_t RunLoop::Source::SignaledTime() const {
  return signaled_time_.load(std::memory_order_acquire);
}

Duration RunLoop::Scope::Timeout(const Option<Time>& now) const {
  // TODO
  return Duration(0);
}

StringView RunLoop::Scope::Default = "default";

}  // namespace TX