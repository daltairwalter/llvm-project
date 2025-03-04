//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// Test the mandates

// template<class F> constexpr auto transform_error(F&& f) &;
// template<class F> constexpr auto transform_error(F&& f) const &;
//
// Let G be remove_cv_t<invoke_result_t<F, decltype(error())>>
// G is a valid template argument for unexpected ([expected.un.general]) and the declaration
// G g(invoke(std::forward<F>(f), error())); is well-formed.

// template<class F> constexpr auto transform_error(F&& f) &&;
// template<class F> constexpr auto transform_error(F&& f) const &&;
//
// Let G be remove_cv_t<invoke_result_t<F, decltype(std::move(error()))>>.
// G is a valid template argument for unexpected ([expected.un.general]) and the declaration
// G g(invoke(std::forward<F>(f), std::move(error()))); is well-formed.

#include <expected>
#include <utility>

static int val;

template <class T>
std::unexpected<int> return_unexpected(T) {
  return std::unexpected<int>(1);
}

template <class T>
int& return_no_object(T) {
  return val;
}

// clang-format off
void test() {

  // Test & overload
  {
    std::expected<int, int> e;
    e.transform_error(return_unexpected<int&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
    // expected-error-re@*:* {{static assertion failed {{.*}}[expected.object.general] A program that instantiates the definition of template expected<T, E> for {{.*}} is ill-formed.}}

    e.transform_error(return_no_object<int&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
    // expected-error-re@*:* {{static assertion failed {{.*}}[expected.object.general] A program that instantiates the definition of template expected<T, E> for {{.*}} is ill-formed.}}
  }

  // Test const& overload
  {
    const std::expected<int, int> e;
    e.transform_error(return_unexpected<const int &>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
    e.transform_error(return_no_object<const int &>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
  }

  // Test && overload
  {
    std::expected<int, int> e;
    std::move(e).transform_error(return_unexpected<int&&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
    std::move(e).transform_error(return_no_object<int&&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
  }

  // Test const&& overload
  {
    const std::expected<int, int> e;
    std::move(e).transform_error(return_unexpected<const int&&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
    std::move(e).transform_error(return_no_object<const int&&>); // expected-error-re@*:* {{static assertion failed {{.*}}The result of {{.*}} must be a valid template argument for unexpected}}
  }
}
// clang-format on
