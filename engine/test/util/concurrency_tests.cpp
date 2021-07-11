#include "engine/util/concurrency.h"

#include <thread>

#include <gmock/gmock.h>

namespace genebits::engine::tests
{
static_assert(cCacheLineSize > 0, "Cache line size must be bigger than 0");

TEST(Concurrency_Tests, SpinLock_Lock_LocksAfterDefaultState)
{
  SpinLock spin_lock;

  std::atomic<bool> locked = false;

  std::thread thread([&]()
    {
      spin_lock.lock();
      locked = true;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  spin_lock.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_Lock_AlreadyLocked)
{
  SpinLock spin_lock;

  spin_lock.lock();

  std::atomic<bool> locked = true;

  std::thread thread([&]()
    {
      spin_lock.lock();
      locked = false;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  spin_lock.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_Unlock_AllowLocking)
{
  SpinLock spin_lock;

  spin_lock.lock();

  spin_lock.unlock();

  std::atomic<bool> locked = false;

  std::thread thread([&]()
    {
      spin_lock.lock();
      locked = true;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  spin_lock.unlock();

  thread.join();
}

TEST(Concurrency_Tests, SpinLock_TryLock_Unlocked)
{
  SpinLock spin_lock;

  ASSERT_TRUE(spin_lock.try_lock());

  spin_lock.unlock();
}

TEST(Concurrency_Tests, SpinLock_TryLock_Locked)
{
  SpinLock spin_lock;

  spin_lock.lock();

  ASSERT_FALSE(spin_lock.try_lock());

  spin_lock.unlock();
}

TEST(Concurrency_Tests, SpinLock_TryLock_LocksAfterDefaultState)
{
  SpinLock spin_lock;

  spin_lock.try_lock();

  std::atomic<bool> locked = true;

  std::thread thread([&]()
    {
      spin_lock.lock();
      locked = false;
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  ASSERT_TRUE(locked);

  spin_lock.unlock();

  thread.join();
}

} // namespace genebits::engine::tests