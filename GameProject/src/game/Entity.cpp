#include "include/game/Entity.h"

Entity::Entity(const std::string& name)
    : m_Name(name)
    , m_Position()
    , m_Active(true)
{
}
