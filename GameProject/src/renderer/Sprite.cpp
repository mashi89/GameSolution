#include "include/renderer/Sprite.h"

Sprite::Sprite(const std::string& texturePath)
    : m_TexturePath(texturePath)
    , m_X(0.0f)
    , m_Y(0.0f)
    , m_ScaleX(1.0f)
    , m_ScaleY(1.0f)
    , m_Rotation(0.0f)
{
}
