#include <gtest/gtest.h>
#include "Renderer.h"

// -------------------------------------------------------------------------
// RenderColor
// -------------------------------------------------------------------------

TEST(RenderColor, DefaultConstruction)
{
    RenderColor c;
    EXPECT_FLOAT_EQ(c.r, 0.0f);
    EXPECT_FLOAT_EQ(c.g, 0.0f);
    EXPECT_FLOAT_EQ(c.b, 0.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(RenderColor, ParameterizedConstruction)
{
    RenderColor c(0.1f, 0.2f, 0.3f, 0.4f);
    EXPECT_FLOAT_EQ(c.r, 0.1f);
    EXPECT_FLOAT_EQ(c.g, 0.2f);
    EXPECT_FLOAT_EQ(c.b, 0.3f);
    EXPECT_FLOAT_EQ(c.a, 0.4f);
}

TEST(RenderColor, White)
{
    RenderColor c = RenderColor::White();
    EXPECT_FLOAT_EQ(c.r, 1.0f);
    EXPECT_FLOAT_EQ(c.g, 1.0f);
    EXPECT_FLOAT_EQ(c.b, 1.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(RenderColor, Black)
{
    RenderColor c = RenderColor::Black();
    EXPECT_FLOAT_EQ(c.r, 0.0f);
    EXPECT_FLOAT_EQ(c.g, 0.0f);
    EXPECT_FLOAT_EQ(c.b, 0.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(RenderColor, Red)
{
    RenderColor c = RenderColor::Red();
    EXPECT_FLOAT_EQ(c.r, 1.0f);
    EXPECT_FLOAT_EQ(c.g, 0.0f);
    EXPECT_FLOAT_EQ(c.b, 0.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(RenderColor, Green)
{
    RenderColor c = RenderColor::Green();
    EXPECT_FLOAT_EQ(c.r, 0.0f);
    EXPECT_FLOAT_EQ(c.g, 1.0f);
    EXPECT_FLOAT_EQ(c.b, 0.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(RenderColor, Blue)
{
    RenderColor c = RenderColor::Blue();
    EXPECT_FLOAT_EQ(c.r, 0.0f);
    EXPECT_FLOAT_EQ(c.g, 0.0f);
    EXPECT_FLOAT_EQ(c.b, 1.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);
}
