#include <gtest/gtest.h>
#include "Sprite.h"

TEST(Sprite, DefaultConstruction)
{
    Sprite s;
    EXPECT_FLOAT_EQ(s.GetX(), 0.0f);
    EXPECT_FLOAT_EQ(s.GetY(), 0.0f);
    EXPECT_FLOAT_EQ(s.GetRotation(), 0.0f);
    EXPECT_TRUE(s.GetTexturePath().empty());
}

TEST(Sprite, ConstructionWithTexturePath)
{
    Sprite s("textures/hero.png");
    EXPECT_EQ(s.GetTexturePath(), "textures/hero.png");
}

TEST(Sprite, SetPosition)
{
    Sprite s;
    s.SetPosition(100.0f, 200.0f);
    EXPECT_FLOAT_EQ(s.GetX(), 100.0f);
    EXPECT_FLOAT_EQ(s.GetY(), 200.0f);
}

TEST(Sprite, SetPositionNegative)
{
    Sprite s;
    s.SetPosition(-50.0f, -75.0f);
    EXPECT_FLOAT_EQ(s.GetX(), -50.0f);
    EXPECT_FLOAT_EQ(s.GetY(), -75.0f);
}

TEST(Sprite, SetRotation)
{
    Sprite s;
    s.SetRotation(45.0f);
    EXPECT_FLOAT_EQ(s.GetRotation(), 45.0f);
}

TEST(Sprite, SetRotationNegative)
{
    Sprite s;
    s.SetRotation(-90.0f);
    EXPECT_FLOAT_EQ(s.GetRotation(), -90.0f);
}

TEST(Sprite, SetScale)
{
    Sprite s;
    // Sprite::SetScale stores the values internally but does not expose
    // scale getters; verify the call is accepted without error.
    EXPECT_NO_THROW(s.SetScale(2.0f, 3.0f));
    EXPECT_FLOAT_EQ(s.GetX(), 0.0f);  // position unaffected by scale
}

TEST(Sprite, MultiplePositionUpdates)
{
    Sprite s;
    s.SetPosition(10.0f, 20.0f);
    s.SetPosition(30.0f, 40.0f);
    EXPECT_FLOAT_EQ(s.GetX(), 30.0f);
    EXPECT_FLOAT_EQ(s.GetY(), 40.0f);
}

TEST(Sprite, TexturePathPreserved)
{
    const std::string path = "assets/sprites/player_idle.png";
    Sprite s(path);
    EXPECT_EQ(s.GetTexturePath(), path);
}
