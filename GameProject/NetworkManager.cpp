#include "NetworkManager.h"

NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

bool NetworkManager::Connect(const std::string& host, int port)
{
    (void)host; (void)port;
    m_Status = ConnectionStatus::Connected;
    return true;
}

void NetworkManager::Disconnect()
{
    m_Status = ConnectionStatus::Disconnected;
}

bool NetworkManager::Send(const std::string& message)
{
    (void)message;
    return m_Status == ConnectionStatus::Connected;
}

void NetworkManager::Update()
{
    // stub: no network activity
}
