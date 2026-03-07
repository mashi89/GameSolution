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
