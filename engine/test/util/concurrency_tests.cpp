#include "genebits/engine/util/concurrency.h"

#include <thread>

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(cCacheLineSize > 0, "Cache line size must be bigger than 0");

TEST(Concurrency_Tests, SpinLock_Lock_LocksAfterDefaultState)
{
  SpinMutex mutex;

  std::atomic<bool> locked = false;

  std::thread thread(
    [&]()
    {
      mutex.lock();
      locked = true;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  mutex.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_Lock_AlreadyLocked)
{
  SpinMutex mutex;

  mutex.lock();

  std::atomic<bool> locked = true;

  std::thread thread(
    [&]()
    {
      mutex.lock();
      locked = false;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  mutex.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_Unlock_AllowLocking)
{
  SpinMutex mutex;

  mutex.lock();

  mutex.unlock();

  std::atomic<bool> locked = false;

  std::thread thread(
    [&]()
    {
      mutex.lock();
      locked = true;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  mutex.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_TryLock_Unlocked)
{
  SpinMutex mutex;

  ASSERT_TRUE(mutex.try_lock());

  mutex.unlock();
}

TEST(Concurrency_Tests, SpinLock_TryLock_Locked)
{
  SpinMutex mutex;

  mutex.lock();

  ASSERT_FALSE(mutex.try_lock());

  mutex.unlock();
}

TEST(Concurrency_Tests, SpinLock_TryLock_LocksAfterDefaultState)
{
  SpinMutex mutex;

  mutex.try_lock();

  std::atomic<bool> locked = true;

  std::thread thread(
    [&]()
    {
      mutex.lock();
      locked = false;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  mutex.unlock();

  thread.join();
}

} // namespace genebits::engine::tests