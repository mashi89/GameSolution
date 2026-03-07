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
