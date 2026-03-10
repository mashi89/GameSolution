#pragma once
#include <cmath>

namespace MathUtils
{
    constexpr float PI = 3.14159265358979f;
    inline float ToRadians(float d)             { return d * PI / 180.0f; }
    inline float ToDegrees(float r)             { return r * 180.0f / PI; }
    inline float Lerp(float a, float b, float t){ return a + (b - a) * t; }
    inline float Clamp(float v, float lo, float hi){ return v < lo ? lo : v > hi ? hi : v; }
}

struct Vec2
{
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
    Vec2  operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2  operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2  operator*(float s)       const { return {x * s,   y * s};   }
    float Length()                 const { return std::sqrt(x*x + y*y); }
    Vec2  Normalized()             const { float l = Length(); return l > 0 ? Vec2{x/l,y/l} : Vec2{}; }
    float Dot(const Vec2& o)       const { return x*o.x + y*o.y; }
};

namespace MathUtils
{
    // Returns the new position of circle A after resolving its overlap with
    // stationary circle B.  posA and posB are world-space centre coordinates;
    // radiusA and radiusB are the (positive) collision radii of each circle.
    // When the circles do not overlap, or when their centres are coincident
    // (distance == 0, preventing a safe push direction), posA is returned
    // unchanged.
    inline Vec2 ResolveCircleOverlap(Vec2 posA, Vec2 posB, float radiusA, float radiusB)
    {
        const Vec2  diff    = posA - posB;
        const float distSq  = diff.Dot(diff);
        const float minDist = radiusA + radiusB;
        if (distSq > 0.0f && distSq < minDist * minDist)
        {
            const float dist    = std::sqrt(distSq);
            const float overlap = minDist - dist;
            return posA + diff * (overlap / dist);
        }
        return posA;
    }
}
