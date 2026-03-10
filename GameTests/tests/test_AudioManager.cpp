#include <gtest/gtest.h>
#include "AudioManager.h"

class AudioManagerTest : public ::testing::Test
{
protected:
    AudioManager& am = AudioManager::GetInstance();

    void SetUp() override
    {
        am.Shutdown();          // clear any leftover state
        am.Initialize();
        am.SetMasterVolume(1.0f);
    }

    void TearDown() override
    {
        am.Shutdown();
    }
};

TEST_F(AudioManagerTest, InitializeReturnsTrue)
{
    EXPECT_TRUE(am.Initialize());
}

TEST_F(AudioManagerTest, DefaultMasterVolumeIsOne)
{
    EXPECT_FLOAT_EQ(am.GetMasterVolume(), 1.0f);
}

TEST_F(AudioManagerTest, SetMasterVolume)
{
    am.SetMasterVolume(0.5f);
    EXPECT_FLOAT_EQ(am.GetMasterVolume(), 0.5f);
}

TEST_F(AudioManagerTest, SetMasterVolumeZero)
{
    am.SetMasterVolume(0.0f);
    EXPECT_FLOAT_EQ(am.GetMasterVolume(), 0.0f);
}

TEST_F(AudioManagerTest, LoadSoundDoesNotCrash)
{
    EXPECT_NO_THROW(am.LoadSound("explosion", "sounds/explosion.wav"));
}

TEST_F(AudioManagerTest, LoadMultipleSounds)
{
    EXPECT_NO_THROW(am.LoadSound("bg",    "sounds/background.ogg"));
    EXPECT_NO_THROW(am.LoadSound("click", "sounds/click.wav"));
}

TEST_F(AudioManagerTest, PlaySoundDoesNotCrash)
{
    am.LoadSound("beep", "sounds/beep.wav");
    EXPECT_NO_THROW(am.PlaySound("beep"));
}

TEST_F(AudioManagerTest, PlaySoundWithVolumeAndLoop)
{
    am.LoadSound("music", "sounds/music.ogg");
    EXPECT_NO_THROW(am.PlaySound("music", 0.8f, true));
}

TEST_F(AudioManagerTest, StopSoundDoesNotCrash)
{
    am.LoadSound("wind", "sounds/wind.wav");
    EXPECT_NO_THROW(am.StopSound("wind"));
}

TEST_F(AudioManagerTest, StopAllDoesNotCrash)
{
    am.LoadSound("a", "sounds/a.wav");
    am.LoadSound("b", "sounds/b.wav");
    EXPECT_NO_THROW(am.StopAll());
}

TEST_F(AudioManagerTest, ShutdownClearsState)
{
    am.LoadSound("s1", "sounds/s1.wav");
    am.Shutdown();
    // After Shutdown, PlaySound should not crash on an unknown id.
    EXPECT_NO_THROW(am.PlaySound("s1"));
}
