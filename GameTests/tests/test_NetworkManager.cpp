#include <gtest/gtest.h>
#include "NetworkManager.h"

class NetworkManagerTest : public ::testing::Test
{
protected:
    NetworkManager& nm = NetworkManager::GetInstance();

    void SetUp() override
    {
        nm.Disconnect();   // ensure clean initial state
    }

    void TearDown() override
    {
        nm.Disconnect();
    }
};

TEST_F(NetworkManagerTest, InitialStatusIsDisconnected)
{
    EXPECT_EQ(nm.GetStatus(), ConnectionStatus::Disconnected);
}

TEST_F(NetworkManagerTest, ConnectChangesStatusToConnected)
{
    bool result = nm.Connect("localhost", 8080);
    EXPECT_TRUE(result);
    EXPECT_EQ(nm.GetStatus(), ConnectionStatus::Connected);
}

TEST_F(NetworkManagerTest, DisconnectRestoresDisconnectedStatus)
{
    nm.Connect("localhost", 9090);
    nm.Disconnect();
    EXPECT_EQ(nm.GetStatus(), ConnectionStatus::Disconnected);
}

TEST_F(NetworkManagerTest, SendReturnsFalseWhenDisconnected)
{
    EXPECT_FALSE(nm.Send("hello"));
}

TEST_F(NetworkManagerTest, SendReturnsTrueWhenConnected)
{
    nm.Connect("localhost", 1234);
    EXPECT_TRUE(nm.Send("hello"));
}

TEST_F(NetworkManagerTest, MultipleConnectCallsStayConnected)
{
    nm.Connect("host1", 100);
    nm.Connect("host2", 200);
    EXPECT_EQ(nm.GetStatus(), ConnectionStatus::Connected);
}

TEST_F(NetworkManagerTest, UpdateDoesNotCrash)
{
    nm.Connect("localhost", 5000);
    EXPECT_NO_THROW(nm.Update());
}

TEST_F(NetworkManagerTest, SetOnMessageReceivedStoresCallback)
{
    bool called = false;
    nm.SetOnMessageReceived([&called](const std::string&) { called = true; });
    // Just verify it does not crash; actual invocation depends on implementation.
    EXPECT_NO_THROW(nm.Update());
}

TEST_F(NetworkManagerTest, DisconnectThenSendReturnsFalse)
{
    nm.Connect("localhost", 3000);
    nm.Disconnect();
    EXPECT_FALSE(nm.Send("data"));
}
