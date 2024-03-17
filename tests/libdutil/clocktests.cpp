#include "libdutil/clock.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ClockTests : public TestBase
{};
}  // namespace

TEST_F(ClockTests, testClockInstanceConstruction)
{
  {
    Clock clock;
    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::LOCAL);
    EXPECT_EQ(clock.getType(), Clock::Type::SYSTEM);
  }

  {
    Clock clock(ConstructionData()
                    .setEnum<Clock::Type>(Clock::Type::STEADY)
                    .setEnum<Clock::Zone>(Clock::Zone::UTC));

    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::UTC);
    EXPECT_EQ(clock.getType(), Clock::Type::STEADY);
  }

  {
    Clock clock(ConstructionData()
                    .setEnum<Clock::Type>(Clock::Type::HIGH_RESOLUTION)
                    .setEnum<Clock::Zone>(Clock::Zone::LOCAL));

    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::LOCAL);
    EXPECT_EQ(clock.getType(), Clock::Type::HIGH_RESOLUTION);
  }
}

TEST_F(ClockTests, testElapsedTimeBetweenTimePoints)
{
  // Test routine is as follows:
  // compare the elapsed time value from a clock object with a local std::chrono clock object.

  // Caser 1: System Clock
  {
    Clock clock;
    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::LOCAL);
    EXPECT_EQ(clock.getType(), Clock::Type::SYSTEM);

    std::chrono::system_clock::time_point ref_start;
    std::chrono::system_clock::time_point ref_stop;
    ref_start = std::chrono::system_clock::now();

    // let some time pass
    int count{0};
    while (count++ < 10000000) {}

    clock.advance();
    ref_stop = std::chrono::system_clock::now();

    auto clock_elapsed_nano_sec = clock.elapsed();
    auto clock_elapsed_micro_sec = clock.elapsed(TIME::UnitPrefix::MICRO);
    auto clock_elapsed_milli_sec = clock.elapsed(TIME::UnitPrefix::MILLI);

    auto ref_elapsed_nano_sec
        = std::chrono::duration_cast<std::chrono::nanoseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_mirco_sec
        = std::chrono::duration_cast<std::chrono::microseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_milli_sec
        = std::chrono::duration_cast<std::chrono::milliseconds>(ref_stop - ref_start).count();

    ASSERT_TRUE(ref_elapsed_nano_sec);
    ASSERT_TRUE(clock_elapsed_nano_sec);
    ASSERT_GE(ref_elapsed_mirco_sec / 100, clock_elapsed_micro_sec / 100);
    ASSERT_EQ(ref_elapsed_milli_sec, clock_elapsed_milli_sec);
  }

  // Case 2: Steady Clock
  {
    Clock clock(ConstructionData()
                    .setEnum<Clock::Type>(Clock::Type::STEADY)
                    .setEnum<Clock::Zone>(Clock::Zone::LOCAL));
    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::LOCAL);
    EXPECT_EQ(clock.getType(), Clock::Type::STEADY);

    std::chrono::steady_clock::time_point ref_stop;
    std::chrono::steady_clock::time_point ref_start = std::chrono::steady_clock::now();

    // let some time pass
    int count{0};
    while (count++ < 10000000) {}

    clock.advance();
    ref_stop = std::chrono::steady_clock::now();

    auto clock_elapsed_nano_sec = clock.elapsed();
    auto clock_elapsed_micro_sec = clock.elapsed(TIME::UnitPrefix::MICRO);
    auto clock_elapsed_milli_sec = clock.elapsed(TIME::UnitPrefix::MILLI);

    auto ref_elapsed_nano_sec
        = std::chrono::duration_cast<std::chrono::nanoseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_mirco_sec
        = std::chrono::duration_cast<std::chrono::microseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_milli_sec
        = std::chrono::duration_cast<std::chrono::milliseconds>(ref_stop - ref_start).count();

    ASSERT_TRUE(ref_elapsed_nano_sec);
    ASSERT_TRUE(clock_elapsed_nano_sec);
    ASSERT_GE(ref_elapsed_mirco_sec / 100, clock_elapsed_micro_sec / 100);
    ASSERT_EQ(ref_elapsed_milli_sec, clock_elapsed_milli_sec);
  }

  // Case 3: High Resolution Clock
  {
    Clock clock(ConstructionData()
                    .setEnum<Clock::Type>(Clock::Type::HIGH_RESOLUTION)
                    .setEnum<Clock::Zone>(Clock::Zone::LOCAL));
    EXPECT_EQ(clock.getTimeZone(), Clock::Zone::LOCAL);
    EXPECT_EQ(clock.getType(), Clock::Type::HIGH_RESOLUTION);

    std::chrono::high_resolution_clock::time_point ref_stop;
    std::chrono::high_resolution_clock::time_point ref_start
        = std::chrono::high_resolution_clock::now();

    // let some time pass
    int count{0};
    while (count++ < 10000000) {}

    clock.advance();
    ref_stop = std::chrono::high_resolution_clock::now();

    auto clock_elapsed_nano_sec = clock.elapsed();
    auto clock_elapsed_micro_sec = clock.elapsed(TIME::UnitPrefix::MICRO);
    auto clock_elapsed_milli_sec = clock.elapsed(TIME::UnitPrefix::MILLI);

    auto ref_elapsed_nano_sec
        = std::chrono::duration_cast<std::chrono::nanoseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_mirco_sec
        = std::chrono::duration_cast<std::chrono::microseconds>(ref_stop - ref_start).count();
    auto ref_elapsed_milli_sec
        = std::chrono::duration_cast<std::chrono::milliseconds>(ref_stop - ref_start).count();

    ASSERT_TRUE(ref_elapsed_nano_sec);
    ASSERT_TRUE(clock_elapsed_nano_sec);
    ASSERT_GE(ref_elapsed_mirco_sec / 100, clock_elapsed_micro_sec / 100);
    ASSERT_EQ(ref_elapsed_milli_sec, clock_elapsed_milli_sec);
  }
}
