#include <gtest/gtest.h>
#include <cmath>
#include "MathUtils.h"

// -------------------------------------------------------------------------
// MathUtils free functions
// -------------------------------------------------------------------------

TEST(MathUtils, ToRadiansZero)
{
    EXPECT_FLOAT_EQ(MathUtils::ToRadians(0.0f), 0.0f);
}

TEST(MathUtils, ToRadians180)
{
    EXPECT_NEAR(MathUtils::ToRadians(180.0f), MathUtils::PI, 1e-5f);
}

TEST(MathUtils, ToRadians90)
{
    EXPECT_NEAR(MathUtils::ToRadians(90.0f), MathUtils::PI / 2.0f, 1e-5f);
}

TEST(MathUtils, ToDegreesZero)
{
    EXPECT_FLOAT_EQ(MathUtils::ToDegrees(0.0f), 0.0f);
}

TEST(MathUtils, ToDegreesPi)
{
    EXPECT_NEAR(MathUtils::ToDegrees(MathUtils::PI), 180.0f, 1e-4f);
}

TEST(MathUtils, LerpMidpoint)
{
    EXPECT_FLOAT_EQ(MathUtils::Lerp(0.0f, 10.0f, 0.5f), 5.0f);
}

TEST(MathUtils, LerpAtZero)
{
    EXPECT_FLOAT_EQ(MathUtils::Lerp(0.0f, 10.0f, 0.0f), 0.0f);
}

TEST(MathUtils, LerpAtOne)
{
    EXPECT_FLOAT_EQ(MathUtils::Lerp(0.0f, 10.0f, 1.0f), 10.0f);
}

TEST(MathUtils, LerpNegativeRange)
{
    EXPECT_FLOAT_EQ(MathUtils::Lerp(-5.0f, 5.0f, 0.5f), 0.0f);
}

TEST(MathUtils, ClampWithinRange)
{
    EXPECT_FLOAT_EQ(MathUtils::Clamp(5.0f, 0.0f, 10.0f), 5.0f);
}

TEST(MathUtils, ClampBelowLow)
{
    EXPECT_FLOAT_EQ(MathUtils::Clamp(-1.0f, 0.0f, 10.0f), 0.0f);
}

TEST(MathUtils, ClampAboveHigh)
{
    EXPECT_FLOAT_EQ(MathUtils::Clamp(11.0f, 0.0f, 10.0f), 10.0f);
}

TEST(MathUtils, ClampAtBoundaries)
{
    EXPECT_FLOAT_EQ(MathUtils::Clamp(0.0f, 0.0f, 10.0f), 0.0f);
    EXPECT_FLOAT_EQ(MathUtils::Clamp(10.0f, 0.0f, 10.0f), 10.0f);
}

TEST(MathUtils, ToRadiansToDegreesRoundTrip)
{
    float degrees = 45.0f;
    EXPECT_NEAR(MathUtils::ToDegrees(MathUtils::ToRadians(degrees)), degrees, 1e-4f);
}

// -------------------------------------------------------------------------
// Vec2
// -------------------------------------------------------------------------

TEST(Vec2, DefaultConstruction)
{
    Vec2 v;
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vec2, ParameterizedConstruction)
{
    Vec2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.x, 3.0f);
    EXPECT_FLOAT_EQ(v.y, 4.0f);
}

TEST(Vec2, Addition)
{
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);
    Vec2 c = a + b;
    EXPECT_FLOAT_EQ(c.x, 4.0f);
    EXPECT_FLOAT_EQ(c.y, 6.0f);
}

TEST(Vec2, Subtraction)
{
    Vec2 a(5.0f, 7.0f);
    Vec2 b(2.0f, 3.0f);
    Vec2 c = a - b;
    EXPECT_FLOAT_EQ(c.x, 3.0f);
    EXPECT_FLOAT_EQ(c.y, 4.0f);
}

TEST(Vec2, ScalarMultiplication)
{
    Vec2 a(3.0f, 4.0f);
    Vec2 b = a * 2.0f;
    EXPECT_FLOAT_EQ(b.x, 6.0f);
    EXPECT_FLOAT_EQ(b.y, 8.0f);
}

TEST(Vec2, Length)
{
    Vec2 v(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.Length(), 5.0f);
}

TEST(Vec2, LengthZeroVector)
{
    Vec2 v;
    EXPECT_FLOAT_EQ(v.Length(), 0.0f);
}

TEST(Vec2, Normalized)
{
    Vec2 v(3.0f, 4.0f);
    Vec2 n = v.Normalized();
    EXPECT_NEAR(n.x, 0.6f, 1e-5f);
    EXPECT_NEAR(n.y, 0.8f, 1e-5f);
    EXPECT_NEAR(n.Length(), 1.0f, 1e-5f);
}

TEST(Vec2, NormalizedZeroVector)
{
    Vec2 v;
    Vec2 n = v.Normalized();
    EXPECT_FLOAT_EQ(n.x, 0.0f);
    EXPECT_FLOAT_EQ(n.y, 0.0f);
}

TEST(Vec2, DotProduct)
{
    Vec2 a(1.0f, 0.0f);
    Vec2 b(0.0f, 1.0f);
    EXPECT_FLOAT_EQ(a.Dot(b), 0.0f);  // perpendicular
}

TEST(Vec2, DotProductParallel)
{
    Vec2 a(2.0f, 3.0f);
    Vec2 b(2.0f, 3.0f);
    EXPECT_FLOAT_EQ(a.Dot(b), a.x * b.x + a.y * b.y);
}
