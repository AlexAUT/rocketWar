#pragma once

#include <aw/util/reflection/reflection.hpp>
#include <aw/util/reflection/typeDescriptor.hpp>
#include <aw/util/reflection/typeResolver.hpp>
#include <aw/util/types.hpp>

#include <aw/util/type/staticForLoop.hpp>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace aw::reflect
{
template <typename OwningClass, typename MemberType>
class ClassMember
{
public:
  using MemberPtr = MemberType OwningClass::*;
  using ConstMemberPtr = const MemberType OwningClass::*;

  using ConstRefSetter = void (OwningClass::*)(const MemberType&);
  using ValueSetter = void (OwningClass::*)(MemberType);

  using RefGetter = const MemberType& (OwningClass::*)(void)const;

  using UnderylingDescriptor = typename TypeResolver<MemberType>::type;

public:
  ClassMember(std::string name, MemberPtr memberPtr) :
      mName(name),
      mHasMemberPtr(true),
      mMemberPtr(memberPtr),
      mConstMemberPtr(memberPtr)
  {
  }

  ClassMember(std::string name, RefGetter getter) : mName(name), mRefGetter(getter) {}
  ClassMember(std::string name, RefGetter getter, ValueSetter setter) :
      mName{name},
      mRefGetter{getter},
      mValueSetter{setter}
  {
  }

  constexpr std::string_view name() const { return mName; }

  constexpr MemberType& value(OwningClass& instance)
  {
    if (mMemberPtr)
      return instance.*mMemberPtr;
    else
      return (instance.*mRefGetter)();
  }

  constexpr void value(OwningClass& instance, const MemberType& value)
  {
    if (mMemberPtr)
      instance.*mMemberPtr = value;
    else if (mValueSetter)
      (instance.*mValueSetter)(value);
  }

  static inline auto& underlyingDescriptor() { return TypeResolver<MemberType>::get(); }

private:
  std::string mName;
  bool mHasMemberPtr{false};
  MemberPtr mMemberPtr{nullptr};
  MemberPtr mConstMemberPtr{nullptr};

  RefGetter mRefGetter{nullptr};

  ValueSetter mValueSetter{nullptr};
};

// Just to make Template handling easier
class ClassTypeDescriptorBase
{
};

template <typename OwningClass, typename... MemberTypes>
class ClassTypeDescriptor : public TypeDescriptor, public ClassTypeDescriptorBase
{
public:
  using Class = OwningClass;

public:
  //  ClassTypeDescriptor(std::string name, size_t size) : TypeDescriptor(name, size) {}
  ClassTypeDescriptor(std::string name, ClassMember<OwningClass, MemberTypes>... members) :
      TypeDescriptor(name, sizeof(OwningClass)),
      mMembers{members...}
  {
  }

  constexpr size_t memberCount() const { return sizeof...(MemberTypes); }

  template <typename Function>
  constexpr void forAllMembers(Function&& fun)
  {
    staticFor<sizeof...(MemberTypes)>([this, &fun](auto index) { fun(std::get<index>(mMembers)); });
  }

private:
  std::tuple<ClassMember<OwningClass, MemberTypes>...> mMembers;
};

#define REFLECT() static inline auto& getTypeDescriptor();

#define REFLECT_BEGIN(TypeName)                                                                    \
  template <>                                                                                      \
  inline auto& aw::reflect::getReflection<TypeName>()                                              \
  {                                                                                                \
    using T = TypeName;                                                                            \
    static ClassTypeDescriptor descriptor                                                                              \
  (                                                                                                                    \
#TypeName

#define REFLECT_MEMBER(name) , ClassMember(#name, &T::name)
#define REFLECT_PRIVATE_MEMBER(name, getter, setter) , ClassMember(#name, getter, setter)

#define REFLECT_END(TypeName)                                                                      \
);                                                                                                 \
  return descriptor;                                                                               \
  }                                                                                                \
  auto& TypeName::getTypeDescriptor() { return aw::reflect::getReflection<TypeName>(); }

} // namespace aw::reflect
