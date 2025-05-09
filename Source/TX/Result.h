#pragma once
#include "TX/Option.h"
#include "TX/Platform.h"
#include "TX/logging/Logger.h"
#include <variant>

namespace TX {
template <class T, class E> class Result {
public:
  static Result<T, E> Ok(T t) { return Result<T, E>(t); }
  static Result<T, E> Err(E e) { return Result<T, E>(e); }
  explicit Result() = default;
  bool isOk() { return std::holds_alternative<T>(inner_); }
  bool isErr() { return std::holds_alternative<E>(inner_); }

  Result &operator=(T &&t) noexcept {
    inner_.template emplace<T>(t);
    return *this;
  }
  Result &operator=(E &&e) noexcept {
    inner_.template emplace<E>(e);
    return *this;
  }

  T unwrap() {
    if (isErr())
      TX_FATAL("called unwrap on an error");
    return std::get<T>(inner_);
  }
  template <class V, class F> Result<V, E> map(F f) {
    return isErr() ? Result<V, E>::Err(std::get<E>(inner_))
                   : Result<V, E>::Ok(f());
  }

private:
  explicit Result(T t) : inner_(t) {}
  explicit Result(E e) : inner_(e) {}
  std::variant<T, E> inner_;
};

template <class E> class Result<void, E> {
public:
  static Result<void, E> Ok() { return Result<void, E>(); }
  static Result<void, E> Err(E e) { return Result<void, E>(e); }
  bool isOk() { return err.isNone(); }
  bool isErr() { return err.isSome(); }

  Result &operator=(E &&e) noexcept {
    err = e;
    return *this;
  }

  void unwrap() {
    if (isErr())
      TX_FATAL("called unwrap on an error");
  }
  template <class V, class F> Result<V, E> map(F f) {
    return isErr() ? Result<V, E>::Err(err.unwrap()) : Result<V, E>::Ok(f());
  }

private:
  explicit Result() : err(None) {}
  explicit Result(E e) : err(e) {}
  Option<E> err;
};
} // namespace TX
