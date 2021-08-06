/* 

Implementation of std::function that can be constructed from lambdas, function pointers, functors.
Extra tasks (1/3):
+ It is possible to construct it from pointer to a member function (TODO try to avoid code duplicating)

*/

#pragma once

#include <type_traits>
#include <typeinfo>
#include <memory>

namespace mystd {

  template <typename>
  class function;

  template <typename R, typename... Args>
  class function<R(Args...)>
    {
      private:

      class AbstractFuncCaller
        {
          public:
          virtual ~AbstractFuncCaller() = default;
          virtual R Call(Args...) = 0;
        };

      std::unique_ptr<AbstractFuncCaller> p_func_caller;

      template <typename F>
      class FuncCaller : public AbstractFuncCaller
        {
          public:
          F deduced_type_func;

          FuncCaller(const F& f)
            : deduced_type_func(f)
            {
            }

          R Call(Args... args) override
            {
            return deduced_type_func(args...);
            }

          ~FuncCaller() = default;
        };

      public:

      function() = delete;
      function(const function&) = delete;
      function(function&&) = delete;

      template <typename F>
      function(F f)
        {
        p_func_caller = std::make_unique<FuncCaller<F>>(f);
        }

      ~function() = default;

      R operator () (Args... args)
        {
        if (!p_func_caller)
          {
          throw std::runtime_error("function is empty");
          }
        return p_func_caller->Call(args...);
        }
    };

  
  template <typename R, typename C, typename... Args>
  class function<R(C, Args...)>
    {
      private:

      class AbstractFuncCaller
        {
          public:
          virtual ~AbstractFuncCaller() = default;
          virtual R Call(C, Args...) = 0;
        };

      std::unique_ptr<AbstractFuncCaller> p_func_caller;

      template <typename F>
      class FuncCaller : public AbstractFuncCaller
        {

        F deduced_type_func;

          public:

          FuncCaller(const F& f)
            : deduced_type_func(f)
            {
            }

          R Call(C c, Args... args) override
            {
            if constexpr (std::is_member_function_pointer_v<F>)
              {
              return (c.*deduced_type_func)(args...);
              }
            else
              return deduced_type_func(c, args...);
            }

          ~FuncCaller() = default;
        };

      public:

      function() = delete;
      function(const function&) = delete;
      function(function&&) = delete;

      template <typename F>
      function(F f)
        {
        p_func_caller = std::make_unique<FuncCaller<F>>(f);
        }

      ~function() = default;

      R operator () (C c, Args... args)
        {
        if (!p_func_caller)
          {
          throw std::runtime_error("function is empty");
          }
        return p_func_caller->Call(c, args...);
        }

    };


  }
