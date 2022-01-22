#include "genebits/engine/util/ref.h"

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
namespace
{
  class Object
  {
    double a; // Dummy fields
    double b;
    double c;
    double d;
    double e;
    double f;
  };

  class RefCountedObject : public RefCounted, public Object
  {};

  class AtomicRefCountedObject : public AtomicRefCounted, public Object
  {};

  class MockRefCountedObject : public Object
  {
  public:
    MOCK_METHOD(void, IntrusiveAddRef, (), (const));
    MOCK_METHOD(bool, IntrusiveDropRef, (), (const));
    MOCK_METHOD(bool, IntrusiveUniqueRef, (), (const));
    MOCK_METHOD(size_t, IntrusiveRefCount, (), (const));

    size_t counter = 0;
  };

  template<typename Type>
  class ParentWrapper : public Type
  {};

  static size_t mock_deleter_calls = 0;

  template<typename Type>
  void MockRefDeleter(void* instance)
  {
    mock_deleter_calls++;

    delete static_cast<Type*>(instance);
  }
} // namespace

static_assert(!IntrusiveRefType<Object>, "Object should not be intrusive");
static_assert(IntrusiveRefType<RefCountedObject>, "RefCountedObject should be intrusive");
static_assert(IntrusiveRefType<AtomicRefCountedObject>, "RefCountedObject should be intrusive");
static_assert(IntrusiveRefType<MockRefCountedObject>, "RefCountedObject should be intrusive");

template<class T>
class Ref_Tests : public testing::Test
{};

using testing::Types;

typedef Types<Object, RefCountedObject, AtomicRefCountedObject> Ref_Tests_Types;

TYPED_TEST_SUITE(Ref_Tests, Ref_Tests_Types);

TYPED_TEST(Ref_Tests, Constructor_Default_Null)
{
  Ref<TypeParam> obj;

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);
}

TYPED_TEST(Ref_Tests, Constructor_NullInstance_Null)
{
  Ref<TypeParam> obj(nullptr);

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);
}

TYPED_TEST(Ref_Tests, Destructor_NullInstance_DeleterNotCalled)
{
  mock_deleter_calls = 0;

  Ref<TypeParam> obj(nullptr, MockRefDeleter<TypeParam>);

  EXPECT_EQ(mock_deleter_calls, 0);
}

TYPED_TEST(Ref_Tests, Constructor_Instance_Unique)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TYPED_TEST(Ref_Tests, Constructor_Downcast_Unique)
{
  ParentWrapper<TypeParam>* instance = new ParentWrapper<TypeParam>;

  Ref<TypeParam> obj(instance);

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TYPED_TEST(Ref_Tests, Destructor_Unique_Deleted)
{
  TypeParam* instance = new TypeParam;

  mock_deleter_calls = 0;

  {
    Ref<TypeParam> obj(instance, MockRefDeleter<TypeParam>);

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);
  }

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, MoveConstructor_Unique_Unique)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> moved(std::move(obj));

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_TRUE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 1);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveConstructor_Downcast_Unique)
{
  ParentWrapper<TypeParam>* instance = new ParentWrapper<TypeParam>;

  Ref<ParentWrapper<TypeParam>> obj(instance);
  Ref<TypeParam> moved(std::move(obj));

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_TRUE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 1);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyConstructor_Null_Null)
{
  Ref<TypeParam> obj(nullptr);
  Ref<TypeParam> copied(obj);

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_FALSE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 0);
  EXPECT_EQ(copied.Get(), nullptr);
}

TYPED_TEST(Ref_Tests, CopyConstructor_Unique_2Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied(obj);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyConstructor_2Ref_3Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied1(obj);
  Ref<TypeParam> copied2(obj);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 3);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied1);
  EXPECT_FALSE(copied1.Unique());
  EXPECT_EQ(copied1.UseCount(), 3);
  EXPECT_EQ(copied1.Get(), instance);

  EXPECT_TRUE(copied2);
  EXPECT_FALSE(copied2.Unique());
  EXPECT_EQ(copied2.UseCount(), 3);
  EXPECT_EQ(copied2.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveConstructor_2Ref_2Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied(obj);

  Ref<TypeParam> moved(std::move(copied));

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_FALSE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 0);
  EXPECT_EQ(copied.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 2);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveConstructor_ExistingRef_Deleted)
{
  TypeParam* instance1 = new TypeParam;
  TypeParam* instance2 = new TypeParam;

  mock_deleter_calls = 0;

  Ref<TypeParam> obj1(instance1, MockRefDeleter<TypeParam>);
  Ref<TypeParam> obj2(instance2);

  obj1 = std::move(obj2);

  EXPECT_TRUE(obj1);
  EXPECT_TRUE(obj1.Unique());
  EXPECT_EQ(obj1.UseCount(), 1);
  EXPECT_EQ(obj1.Get(), instance2);

  EXPECT_FALSE(obj2);
  EXPECT_FALSE(obj2.Unique());
  EXPECT_EQ(obj2.UseCount(), 0);
  EXPECT_EQ(obj2.Get(), nullptr);

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, CopyConstructor_ExistingRef_Deleted)
{
  TypeParam* instance1 = new TypeParam;
  TypeParam* instance2 = new TypeParam;

  mock_deleter_calls = 0;

  Ref<TypeParam> obj1(instance1, MockRefDeleter<TypeParam>);
  Ref<TypeParam> obj2(instance2);

  obj1 = obj2;

  EXPECT_TRUE(obj1);
  EXPECT_FALSE(obj1.Unique());
  EXPECT_EQ(obj1.UseCount(), 2);
  EXPECT_EQ(obj1.Get(), instance2);

  EXPECT_TRUE(obj2);
  EXPECT_FALSE(obj2.Unique());
  EXPECT_EQ(obj2.UseCount(), 2);
  EXPECT_EQ(obj2.Get(), instance2);

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, Destructor_2Ref_Dereferenced)
{
  TypeParam* instance = new TypeParam;

  mock_deleter_calls = 0;

  {
    Ref<TypeParam> obj(instance, MockRefDeleter<TypeParam>);

    {

      Ref<TypeParam> copied(obj);

      EXPECT_TRUE(obj);
      EXPECT_FALSE(obj.Unique());
      EXPECT_EQ(obj.UseCount(), 2);
      EXPECT_EQ(obj.Get(), instance);

      EXPECT_TRUE(copied);
      EXPECT_FALSE(copied.Unique());
      EXPECT_EQ(copied.UseCount(), 2);
      EXPECT_EQ(copied.Get(), instance);
    }

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);
  }

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, Destructor_3Ref_Dereferenced)
{
  TypeParam* instance = new TypeParam;

  mock_deleter_calls = 0;

  {
    Ref<TypeParam> obj(instance, MockRefDeleter<TypeParam>);

    {
      Ref<TypeParam> copied1(obj);

      {
        Ref<TypeParam> copied2(obj);

        EXPECT_TRUE(obj);
        EXPECT_FALSE(obj.Unique());
        EXPECT_EQ(obj.UseCount(), 3);
        EXPECT_EQ(obj.Get(), instance);

        EXPECT_TRUE(copied1);
        EXPECT_FALSE(copied1.Unique());
        EXPECT_EQ(copied1.UseCount(), 3);
        EXPECT_EQ(copied1.Get(), instance);

        EXPECT_TRUE(copied2);
        EXPECT_FALSE(copied2.Unique());
        EXPECT_EQ(copied2.UseCount(), 3);
        EXPECT_EQ(copied2.Get(), instance);
      }

      EXPECT_TRUE(obj);
      EXPECT_FALSE(obj.Unique());
      EXPECT_EQ(obj.UseCount(), 2);
      EXPECT_EQ(obj.Get(), instance);

      EXPECT_TRUE(copied1);
      EXPECT_FALSE(copied1.Unique());
      EXPECT_EQ(copied1.UseCount(), 2);
      EXPECT_EQ(copied1.Get(), instance);
    }

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);
  }

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, Destructor_2RefSameBlock_Dereferenced)
{
  TypeParam* instance = new TypeParam;

  mock_deleter_calls = 0;

  {
    Ref<TypeParam> obj(instance, MockRefDeleter<TypeParam>);

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);

    Ref<TypeParam> copied(obj);

    EXPECT_TRUE(obj);
    EXPECT_FALSE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 2);
    EXPECT_EQ(obj.Get(), instance);

    EXPECT_TRUE(copied);
    EXPECT_FALSE(copied.Unique());
    EXPECT_EQ(copied.UseCount(), 2);
    EXPECT_EQ(copied.Get(), instance);
  }

  EXPECT_EQ(mock_deleter_calls, 1);
}

TYPED_TEST(Ref_Tests, MoveAssignment_SelfAssingment_Same)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  obj = std::move(obj);

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveAssignment_SelfAssingment2Ref_Same)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied(obj);

  obj = std::move(obj);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyAssignment_SelfAssingment_Same)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  obj = obj;

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyAssignment_SelfAssingment2Ref_Same)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied(obj);

  obj = obj;

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveAssignment_Unique_Unique)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> moved;

  moved = std::move(obj);

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_TRUE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 1);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyAssignment_Null_Null)
{
  Ref<TypeParam> obj(nullptr);
  Ref<TypeParam> copied;

  copied = obj;

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_FALSE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 0);
  EXPECT_EQ(copied.Get(), nullptr);
}

TYPED_TEST(Ref_Tests, CopyAssignment_Unique_2Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied;

  copied = obj;

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyAssignment_2Ref_3Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied1;
  copied1 = obj;
  Ref<TypeParam> copied2;
  copied2 = obj;

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 3);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied1);
  EXPECT_FALSE(copied1.Unique());
  EXPECT_EQ(copied1.UseCount(), 3);
  EXPECT_EQ(copied1.Get(), instance);

  EXPECT_TRUE(copied2);
  EXPECT_FALSE(copied2.Unique());
  EXPECT_EQ(copied2.UseCount(), 3);
  EXPECT_EQ(copied2.Get(), instance);
}

TYPED_TEST(Ref_Tests, CopyAssignment_Downcast_2Ref)
{
  ParentWrapper<TypeParam>* instance = new ParentWrapper<TypeParam>;

  Ref<ParentWrapper<TypeParam>> obj(instance);
  Ref<TypeParam> copied;
  copied = obj;

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveAssignment_2Ref_2Ref)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);
  Ref<TypeParam> copied = obj;

  Ref<TypeParam> moved;
  moved = std::move(copied);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_FALSE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 0);
  EXPECT_EQ(copied.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 2);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, MoveAssignment_Downcast_2Ref)
{
  ParentWrapper<TypeParam>* instance = new ParentWrapper<TypeParam>;

  Ref<ParentWrapper<TypeParam>> obj(instance);
  Ref<ParentWrapper<TypeParam>> copied = obj;

  Ref<TypeParam> moved;
  moved = std::move(copied);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(moved);
  EXPECT_FALSE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 2);
  EXPECT_EQ(moved.Get(), instance);
}

TYPED_TEST(Ref_Tests, EqualityOperator_ItselfAndNull_True)
{
  Ref<TypeParam> obj(nullptr);

  EXPECT_TRUE(obj == obj);
  EXPECT_FALSE(obj != obj);
}

TYPED_TEST(Ref_Tests, EqualityOperator_Itself_True)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  EXPECT_TRUE(obj == obj);
  EXPECT_FALSE(obj != obj);
}

TYPED_TEST(Ref_Tests, EqualityOperator_BothNull_True)
{
  Ref<TypeParam> obj1(nullptr);
  Ref<TypeParam> obj2(nullptr);

  EXPECT_TRUE(obj1 == obj2);
  EXPECT_FALSE(obj1 != obj2);
}

TYPED_TEST(Ref_Tests, EqualityOperator_LhsNull_False)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj1(nullptr);
  Ref<TypeParam> obj2(instance);

  EXPECT_FALSE(obj1 == obj2);
  EXPECT_TRUE(obj1 != obj2);
}

TYPED_TEST(Ref_Tests, EqualityOperator_RhsNull_False)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj1(instance);
  Ref<TypeParam> obj2(nullptr);

  EXPECT_FALSE(obj1 == obj2);
  EXPECT_TRUE(obj1 != obj2);
}

TYPED_TEST(Ref_Tests, EqualityOperator_SameInstance_True)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj1(instance);
  Ref<TypeParam> obj2(obj1);

  EXPECT_TRUE(obj1 == obj2);
  EXPECT_FALSE(obj1 != obj2);
}

TYPED_TEST(Ref_Tests, EqualityOperator_NullAndNullptr_True)
{
  Ref<TypeParam> obj(nullptr);

  EXPECT_TRUE(obj == nullptr);
  EXPECT_FALSE(obj != nullptr);
}

TYPED_TEST(Ref_Tests, EqualityOperator_NotNullAndNullptr_False)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  EXPECT_FALSE(obj == nullptr);
  EXPECT_TRUE(obj != nullptr);
}

TYPED_TEST(Ref_Tests, Swap_Same_Same)
{
  TypeParam* instance = new TypeParam;

  Ref<TypeParam> obj(instance);

  obj.Swap(obj);

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TYPED_TEST(Ref_Tests, Swap_Different_Swapped)
{
  TypeParam* instance1 = new TypeParam;
  TypeParam* instance2 = new TypeParam;

  Ref<TypeParam> obj1(instance1);
  Ref<TypeParam> obj2(instance2);

  obj1.Swap(obj2);

  EXPECT_TRUE(obj1);
  EXPECT_TRUE(obj1.Unique());
  EXPECT_EQ(obj1.UseCount(), 1);
  EXPECT_EQ(obj1.Get(), instance2);

  EXPECT_TRUE(obj2);
  EXPECT_TRUE(obj2.Unique());
  EXPECT_EQ(obj2.UseCount(), 1);
  EXPECT_EQ(obj2.Get(), instance1);
}

TYPED_TEST(Ref_Tests, MakeRef_Default_Unique)
{
  Ref<TypeParam> obj = MakeRef<TypeParam>();

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_NE(obj.Get(), nullptr);
}

TYPED_TEST(Ref_Tests, MakeRef_Default_Ref2)
{
  Ref<TypeParam> obj = MakeRef<TypeParam>();

  {
    Ref<TypeParam> copy = obj;

    EXPECT_TRUE(obj);
    EXPECT_FALSE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 2);
    EXPECT_NE(obj.Get(), nullptr);

    EXPECT_TRUE(copy);
    EXPECT_FALSE(copy.Unique());
    EXPECT_EQ(copy.UseCount(), 2);
    EXPECT_NE(copy.Get(), nullptr);
  }

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_NE(obj.Get(), nullptr);
}

TEST(Ref_Tests, Destructor_Unique_CallIntrusiveDropRef)
{
  MockRefCountedObject* instance = new MockRefCountedObject;

  EXPECT_CALL(*instance, IntrusiveUniqueRef()).Times(testing::AtMost(1)).WillOnce([&]() { return true; });

  EXPECT_CALL(*instance, IntrusiveDropRef()).Times(1).WillOnce([&]() { return true; });

  {
    Ref<MockRefCountedObject> obj(instance);
  }
}

TEST(Ref_Tests, Destructor_2Ref_CallIntrusiveDropRef)
{
  MockRefCountedObject* instance = new MockRefCountedObject;

  EXPECT_CALL(*instance, IntrusiveUniqueRef()).Times(testing::AtMost(1)).WillOnce([&]() { return true; });

  EXPECT_CALL(*instance, IntrusiveAddRef()).Times(1).WillOnce([&]() { instance->counter++; });

  EXPECT_CALL(*instance, IntrusiveDropRef())
    .Times(2)
    .WillOnce(
      [&]()
      {
        instance->counter--;
        return false;
      })
    .WillOnce([&]() { return true; });

  {
    Ref<MockRefCountedObject> obj(instance);

    Ref<MockRefCountedObject> copied(obj);
  }
}

TEST(Ref_Tests, CopyConstructor_Unique_CallIntrusiveAddRef)
{
  MockRefCountedObject* instance = new MockRefCountedObject;

  EXPECT_CALL(*instance, IntrusiveUniqueRef()).Times(testing::AtMost(1)).WillOnce([&]() { return true; });

  Ref<MockRefCountedObject> obj(instance);

  EXPECT_CALL(*instance, IntrusiveAddRef()).Times(1);

  Ref<MockRefCountedObject> copied(obj);

  EXPECT_CALL(*instance, IntrusiveDropRef())
    .Times(2)
    .WillOnce([&]() { return false; })
    .WillOnce([&]() { return true; });
}

TEST(Ref_Tests, CopyAssignment_Unique_CallIntrusiveAddRef)
{
  MockRefCountedObject* instance = new MockRefCountedObject;

  EXPECT_CALL(*instance, IntrusiveUniqueRef()).Times(testing::AtMost(1)).WillOnce([&]() { return true; });

  Ref<MockRefCountedObject> obj(instance);

  EXPECT_CALL(*instance, IntrusiveAddRef()).Times(1);

  Ref<MockRefCountedObject> copied;
  copied = obj;

  EXPECT_CALL(*instance, IntrusiveDropRef())
    .Times(2)
    .WillOnce([&]() { return false; })
    .WillOnce([&]() { return true; });
}

} // namespace genebits::engine::tests