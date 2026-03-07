#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "GameLoop.h"

TEST(GameLoop, InitiallyNotRunning)
{
    GameLoop loop;
    EXPECT_FALSE(loop.IsRunning());
}

TEST(GameLoop, InitialDeltaTimeIsZero)
{
    GameLoop loop;
    EXPECT_FLOAT_EQ(loop.GetDeltaTime(), 0.0f);
}

TEST(GameLoop, FPSIsZeroWhenDeltaTimeIsZero)
{
    GameLoop loop;
    EXPECT_FLOAT_EQ(loop.GetFPS(), 0.0f);
}

TEST(GameLoop, StartMakesIsRunningTrue)
{
    GameLoop loop;
    loop.Start();
    EXPECT_TRUE(loop.IsRunning());
}

TEST(GameLoop, StopMakesIsRunningFalse)
{
    GameLoop loop;
    loop.Start();
    loop.Stop();
    EXPECT_FALSE(loop.IsRunning());
}

TEST(GameLoop, MultipleStartStopCycles)
{
    GameLoop loop;
    for (int i = 0; i < 3; ++i)
    {
        loop.Start();
        EXPECT_TRUE(loop.IsRunning());
        loop.Stop();
        EXPECT_FALSE(loop.IsRunning());
    }
}

TEST(GameLoop, StopWithoutStartDoesNotCrash)
{
    GameLoop loop;
    EXPECT_NO_THROW(loop.Stop());
    EXPECT_FALSE(loop.IsRunning());
}
