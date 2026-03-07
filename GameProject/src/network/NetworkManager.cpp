#include "include/network/NetworkManager.h"

NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

bool NetworkManager::Connect(const std::string& host, int port)
{
    m_Status = ConnectionStatus::Connecting;
    // Stub: mark as connected
    m_Status = ConnectionStatus::Connected;
    return true;
}

void NetworkManager::Disconnect()
{
    m_Status = ConnectionStatus::Disconnected;
}

bool NetworkManager::Send(const std::string& message)
{
    return m_Status == ConnectionStatus::Connected;
}

void NetworkManager::Update()
{
}
