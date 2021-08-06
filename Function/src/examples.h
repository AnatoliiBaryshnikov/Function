/*

Some examples of mystd::function usage

*/

#pragma once

#include <iostream>
#include <functional>
#include "function.h" 

namespace examples {

  struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_ + i << '\n'; }
    int num_;
    };

  void print_num(int i)
    {
    std::cout << i << '\n';
    }

  struct PrintNum {
    void operator()(int i) const
      {
      std::cout << i << '\n';
      }
    };

  // a function:
  int half(int x) { return x / 2; }

  // a function object class:
  struct third_t {
    int operator()(int x) { return x / 3; }
    };

  // a class with data members:
  struct MyValue {
    int value;
    int fifth() { return value / 5; }
    };

  /// <summary>
  /// Example of usage from https://en.cppreference.com/w/cpp/utility/functional/function
  /// 
  /// Output:
  /// -9
  /// 42
  /// 31337
  /// 314160
  /// 314160
  /// 18
  /// 5!= 120;  6!= 720;  7!= 5040;
  /// 
  /// </summary>
  void example_from_cpp_ref()
    {
    std::cout << "\nExample of usage from https://en.cppreference.com/w/cpp/utility/functional/function \n\n";
    // store a free function
    mystd::function<void(int)> f_display = print_num;
    f_display(-9);

    // store a lambda
    mystd::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();

    // store the result of a call to std::bind
    mystd::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // store a call to a member function
    mystd::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // store a call to a function object
    mystd::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);

    auto factorial = [](int n) {
      // store a lambda object to emulate "recursive lambda"; aware of extra overhead
      mystd::function<int(int)> fac = [&](int n) { return (n < 2) ? 1 : n * fac(n - 1); };
      // note that "auto fac = [&](int n){...};" does not work in recursive calls
      return fac(n);
      };
    for (int i{ 5 }; i != 8; ++i) { std::cout << i << "! = " << factorial(i) << ";  "; }

    }

  /// <summary>
  /// Example of usage from https://www.cplusplus.com/reference/functional/function/function/
  /// 
  /// Output:
  /// fn1(60) : 30
  /// fn2(60) : 30
  /// fn3(60) : 20
  /// fn4(60) : 15
  /// fn5(60) : -60
  /// fifth(sixty) : 12
  /// 15
  /// 5
  /// 50
  /// 
  /// </summary>
  void example_from_cplusplus()
    {
    std::cout << "\n\nExample of usage from https://www.cplusplus.com/reference/functional/function/function/ \n\n";

    mystd::function<int(int)> fn1 = half;                    // function
    mystd::function<int(int)> fn2 = &half;                   // function pointer
    mystd::function<int(int)> fn3 = third_t();               // function object
    mystd::function<int(int)> fn4 = [](int x) {return x / 4; };  // lambda expression
    mystd::function<int(int)> fn5 = std::negate<int>();      // standard function object

    std::cout << "fn1(60): " << fn1(60) << '\n';
    std::cout << "fn2(60): " << fn2(60) << '\n';
    std::cout << "fn3(60): " << fn3(60) << '\n';
    std::cout << "fn4(60): " << fn4(60) << '\n';
    std::cout << "fn5(60): " << fn5(60) << '\n';

    // stuff with members:
    mystd::function<int(MyValue&)> fifth = &MyValue::fifth;  // pointer to member function
    MyValue sixty{ 60 };
    std::cout << "fifth(sixty): " << fifth(sixty) << '\n';

    // an array of functions:
    mystd::function<int(int, int)> fn[] = {
      std::plus<int>(),
      std::minus<int>(),
      std::multiplies<int>()
      };
    for (auto& x : fn) std::cout << x(10, 5) << '\n';

    }


  }