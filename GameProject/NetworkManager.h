#pragma once
#include <string>
#include <functional>

enum class ConnectionStatus { Disconnected, Connecting, Connected };
using MessageCallback = std::function<void(const std::string&)>;

class NetworkManager
{
public:
    static NetworkManager& GetInstance();
    bool Connect(const std::string& host, int port);
    void Disconnect();
    bool Send(const std::string& message);
    void SetOnMessageReceived(MessageCallback cb) { m_OnMessage = cb; }
    ConnectionStatus GetStatus() const { return m_Status; }
    void Update();

private:
    NetworkManager() = default;
    ConnectionStatus m_Status = ConnectionStatus::Disconnected;
    MessageCallback  m_OnMessage;
};

// inline implementations
inline NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

inline bool NetworkManager::Connect(const std::string& host, int port)
{
    (void)host; (void)port;
    m_Status = ConnectionStatus::Connected;
    return true;
}

inline void NetworkManager::Disconnect()
{
    m_Status = ConnectionStatus::Disconnected;
}

inline bool NetworkManager::Send(const std::string& message)
{
    (void)message;
    return m_Status == ConnectionStatus::Connected;
}

inline void NetworkManager::Update()
{
    // stub: no network activity
}

