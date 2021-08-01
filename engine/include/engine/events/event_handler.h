#ifndef GENEBITS_ENGINE_UTIL_EVENTHANDLER_H
#define GENEBITS_ENGINE_UTIL_EVENTHANDLER_H

namespace genebits::engine
{
namespace
{
  template<typename Event>
  class EventFunctor
  {
  public:
    virtual void operator()(const Event& event) = 0;
  };

  template<typename Event>
  class FreeEventFunctor : public EventFunctor<Event>
  {
  public:
    using FunctionType = void (*)(const Event&);

    constexpr FreeEventFunctor(FunctionType function) noexcept
      : function_(function)
    {
    }

    void operator()(const Event& event) override
    {
      function_(event);
    }

  private:
    FunctionType function_;
  };

  template<typename ClassType, typename Event>
  class MemberEventFunctor : public EventFunctor<Event>
  {
  public:
    using FunctionType = void (ClassType::*)(const Event&);

    constexpr MemberEventFunctor(FunctionType function, ClassType* instance) noexcept
      : function_(function), instance_(instance)
    {
    }

    void operator()(const Event& event) override
    {
      (instance_->*function_)(event);
    }

  private:
    FunctionType function_;
    ClassType* instance_;
  };

  template<typename ClassType, typename Event>
  class ConstMemberEventFunctor : public EventFunctor<Event>
  {
  public:
    using FunctionType = void (ClassType::*)(const Event&) const;

    constexpr ConstMemberEventFunctor(FunctionType function, ClassType* instance) noexcept
      : function_(function), instance_(instance)
    {
    }

    void operator()(const Event& event) override
    {
      (instance_->*function_)(event);
    }

  private:
    FunctionType function_;
    ClassType* instance_;
  };
} // namespace

template<typename Event>
class EventHandler
{
public:
  static constexpr size_t cLargestFunctorSize = sizeof(&EventFunctor<Event>::operator()) + sizeof(void*);

  constexpr EventHandler(void (*function)(const Event& event)) noexcept
  {
    new (functor_) FreeEventFunctor<Event>(function);
  }

  template<typename ClassType>
  constexpr EventHandler(void (ClassType::*function)(const Event& event), ClassType* instance) noexcept
  {
    new (functor_) MemberEventFunctor<ClassType, Event>(function, instance);
  }

  template<typename ClassType>
  constexpr EventHandler(void (ClassType::*function)(const Event& event) const, ClassType* instance) noexcept
  {
    new (functor_) ConstMemberEventFunctor<ClassType, Event>(function, instance);
  }

  void Invoke(const Event& event)
  {
    (*reinterpret_cast<EventFunctor<Event>*>(&functor_))(event);
  }

private:
  // Use stack memory and placement new to avoid heap allocation
  char functor_[cLargestFunctorSize];
};

} // namespace genebits::engine

#endif
