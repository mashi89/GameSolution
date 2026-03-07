#pragma once
#include <string>

struct Color
{
    float r, g, b, a;
    Color(float r=0, float g=0, float b=0, float a=1.f) : r(r),g(g),b(b),a(a) {}
    static Color White() { return {1,1,1,1}; }
    static Color Black() { return {0,0,0,1}; }
    static Color Red()   { return {1,0,0,1}; }
    static Color Green() { return {0,1,0,1}; }
    static Color Blue()  { return {0,0,1,1}; }
};

class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual bool Initialize(int width, int height, const std::string& title) = 0;
    virtual void Shutdown() = 0;
    virtual void Clear(Color color = Color::Black()) = 0;
    virtual void Present() = 0;
    int GetWidth()  const { return m_Width;  }
    int GetHeight() const { return m_Height; }

protected:
    int m_Width = 0, m_Height = 0;
    std::string m_Title;
};
