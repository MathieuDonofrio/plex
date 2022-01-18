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
    MOCK_METHOD(void, IntrusiveAddRef, ());
    MOCK_METHOD(bool, IntrusiveDropRef, ());
    MOCK_METHOD(bool, IntrusiveUniqueRef, ());
    MOCK_METHOD(size_t, IntrusiveRefCount, ());
  };

  static size_t mock_deleter_calls = 0;

  template<typename Type>
  void MockRefDeleter(Type* instance)
  {
    mock_deleter_calls++;

    RefDefaultDeleter(instance);
  }

} // namespace

TEST(Ref_Tests, Constructor_Default_Null)
{
  Ref<Object> obj;

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);
}

TEST(Ref_Tests, Constructor_NullInstance_Null)
{
  Ref<Object> obj(nullptr);

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);
}

TEST(Ref_Tests, Constructor_Instance_Unique)
{
  Object* instance = new Object;

  Ref<Object> obj(instance);

  EXPECT_TRUE(obj);
  EXPECT_TRUE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 1);
  EXPECT_EQ(obj.Get(), instance);
}

TEST(Ref_Tests, MoveConstructor_Unique_Unique)
{
  Object* instance = new Object;

  Ref<Object> obj(instance);
  Ref<Object> moved(std::move(obj));

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_TRUE(moved);
  EXPECT_TRUE(moved.Unique());
  EXPECT_EQ(moved.UseCount(), 1);
  EXPECT_EQ(moved.Get(), instance);
}

TEST(Ref_Tests, CopyConstructor_Null_Null)
{
  Ref<Object> obj(nullptr);
  Ref<Object> copied(obj);

  EXPECT_FALSE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 0);
  EXPECT_EQ(obj.Get(), nullptr);

  EXPECT_FALSE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 0);
  EXPECT_EQ(copied.Get(), nullptr);
}

TEST(Ref_Tests, CopyConstructor_Unique_2Ref)
{
  Object* instance = new Object;

  Ref<Object> obj(instance);
  Ref<Object> copied(obj);

  EXPECT_TRUE(obj);
  EXPECT_FALSE(obj.Unique());
  EXPECT_EQ(obj.UseCount(), 2);
  EXPECT_EQ(obj.Get(), instance);

  EXPECT_TRUE(copied);
  EXPECT_FALSE(copied.Unique());
  EXPECT_EQ(copied.UseCount(), 2);
  EXPECT_EQ(copied.Get(), instance);
}

TEST(Ref_Tests, CopyConstructor_2Ref_3Ref)
{
  Object* instance = new Object;

  Ref<Object> obj(instance);
  Ref<Object> copied1(obj);
  Ref<Object> copied2(obj);

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

TEST(Ref_Tests, Destructor_Unique_Deleted)
{
  Object* instance = new Object;

  mock_deleter_calls = 0;

  {
    Ref<Object, MockRefDeleter> obj(instance);

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);
  }

  EXPECT_EQ(mock_deleter_calls, 1);
}

TEST(Ref_Tests, Destructor_2Ref_Dereferenced)
{
  Object* instance = new Object;

  mock_deleter_calls = 0;

  {
    Ref<Object, MockRefDeleter> obj(instance);

    {

      Ref<Object, MockRefDeleter> copied(obj);

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

TEST(Ref_Tests, Destructor_3Ref_Dereferenced)
{
  Object* instance = new Object;

  mock_deleter_calls = 0;

  {
    Ref<Object, MockRefDeleter> obj(instance);

    {
      Ref<Object, MockRefDeleter> copied1(obj);

      {
        Ref<Object, MockRefDeleter> copied2(obj);

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

TEST(Ref_Tests, Destructor_2RefSameBlock_Dereferenced)
{
  Object* instance = new Object;

  mock_deleter_calls = 0;

  {
    Ref<Object, MockRefDeleter> obj(instance);

    EXPECT_TRUE(obj);
    EXPECT_TRUE(obj.Unique());
    EXPECT_EQ(obj.UseCount(), 1);
    EXPECT_EQ(obj.Get(), instance);

    Ref<Object, MockRefDeleter> copied(obj);

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
} // namespace genebits::engine::tests