#pragma once
#include <string>

class Sprite
{
public:
    Sprite() = default;
    explicit Sprite(const std::string& texturePath);
    void SetPosition(float x, float y) { m_X = x; m_Y = y; }
    void SetScale(float sx, float sy)  { m_ScaleX = sx; m_ScaleY = sy; }
    void SetRotation(float angle)      { m_Rotation = angle; }
    float GetX()        const { return m_X; }
    float GetY()        const { return m_Y; }
    float GetRotation() const { return m_Rotation; }
    const std::string& GetTexturePath() const { return m_TexturePath; }

private:
    std::string m_TexturePath;
    float m_X=0, m_Y=0, m_ScaleX=1, m_ScaleY=1, m_Rotation=0;
};

// inline implementation
inline Sprite::Sprite(const std::string& texturePath)
    : m_TexturePath(texturePath)
{
}

