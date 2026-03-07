#pragma once
#include "MathUtils.h"
#include <string>

class Entity
{
public:
    explicit Entity(const std::string& name = "Entity");
    virtual ~Entity() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    void SetPosition(Vec2 pos) { m_Position = pos; }
    Vec2 GetPosition()   const { return m_Position; }
    bool IsActive()      const { return m_Active; }
    void SetActive(bool v)     { m_Active = v; }
    const std::string& GetName() const { return m_Name; }

protected:
    std::string m_Name;
    Vec2        m_Position;
    bool        m_Active = true;
};
