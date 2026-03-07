#include <gtest/gtest.h>
#include <memory>
#include "GameManager.h"
#include "GameState.h"

// -------------------------------------------------------------------------
// Minimal concrete GameState that records lifecycle calls via external
// counters so results stay accessible even after the state is destroyed.
// -------------------------------------------------------------------------

class TrackingState : public GameState
{
public:
    int& enters;
    int& exits;
    int& resumes;
    GameStateType stateType;
    std::string   stateName;

    TrackingState(int& e, int& x, int& r,
                  GameStateType t = GameStateType::Playing,
                  const std::string& n = "Tracking")
        : enters(e), exits(x), resumes(r), stateType(t), stateName(n) {}

    void OnEnter()  override { ++enters; }
    void OnExit()   override { ++exits;  }
    void OnResume() override { ++resumes; }
    void Update(float) override {}
    void Render()       override {}
    GameStateType GetType() const override { return stateType; }
    std::string   GetName() const override { return stateName; }
};

// -------------------------------------------------------------------------
// Fixture: ensures the singleton stack is empty before/after every test.
// -------------------------------------------------------------------------

class GameManagerTest : public ::testing::Test
{
protected:
    GameManager& gm = GameManager::GetInstance();

    void SetUp() override { drain(); }
    void TearDown() override { drain(); }

private:
    void drain()
    {
        while (gm.GetCurrentState() != nullptr)
            gm.PopState();
    }
};

// -------------------------------------------------------------------------
// Tests
// -------------------------------------------------------------------------

TEST_F(GameManagerTest, InitiallyNoCurrentState)
{
    EXPECT_EQ(gm.GetCurrentState(), nullptr);
}

TEST_F(GameManagerTest, PushStateCallsOnEnter)
{
    int enters = 0, exits = 0, resumes = 0;
    gm.PushState(std::make_unique<TrackingState>(enters, exits, resumes));
    EXPECT_EQ(enters, 1);
    EXPECT_EQ(exits,  0);
}

TEST_F(GameManagerTest, PushStateBecomesCurrentState)
{
    int e = 0, x = 0, r = 0;
    gm.PushState(std::make_unique<TrackingState>(e, x, r,
                 GameStateType::Playing, "Alpha"));
    ASSERT_NE(gm.GetCurrentState(), nullptr);
    EXPECT_EQ(gm.GetCurrentState()->GetName(), "Alpha");
}

TEST_F(GameManagerTest, PopStateCallsOnExit)
{
    int enters = 0, exits = 0, resumes = 0;
    gm.PushState(std::make_unique<TrackingState>(enters, exits, resumes));
    gm.PopState();
    EXPECT_EQ(exits, 1);
    EXPECT_EQ(gm.GetCurrentState(), nullptr);
}

TEST_F(GameManagerTest, PopStateCallsOnResumeForPreviousState)
{
    int e1 = 0, x1 = 0, r1 = 0;
    int e2 = 0, x2 = 0, r2 = 0;
    gm.PushState(std::make_unique<TrackingState>(e1, x1, r1,
                 GameStateType::MainMenu, "Menu"));
    gm.PushState(std::make_unique<TrackingState>(e2, x2, r2,
                 GameStateType::Playing, "Play"));
    EXPECT_EQ(r1, 0);
    gm.PopState();          // pops Play, resumes Menu
    EXPECT_EQ(r1, 1);
    EXPECT_EQ(x2, 1);
}

TEST_F(GameManagerTest, PopOnEmptyStackDoesNotCrash)
{
    EXPECT_NO_THROW(gm.PopState());
    EXPECT_EQ(gm.GetCurrentState(), nullptr);
}

TEST_F(GameManagerTest, StackReturnsTopState)
{
    int e1=0,x1=0,r1=0, e2=0,x2=0,r2=0;
    gm.PushState(std::make_unique<TrackingState>(e1,x1,r1,
                 GameStateType::MainMenu, "Menu"));
    gm.PushState(std::make_unique<TrackingState>(e2,x2,r2,
                 GameStateType::Playing, "Play"));
    EXPECT_EQ(gm.GetCurrentState()->GetName(), "Play");
}

TEST_F(GameManagerTest, ChangeStateExitsOldAndEntersNew)
{
    int e1=0,x1=0,r1=0, e2=0,x2=0,r2=0;
    gm.PushState(std::make_unique<TrackingState>(e1,x1,r1,
                 GameStateType::MainMenu, "Menu"));
    EXPECT_EQ(e1, 1);

    gm.ChangeState(std::make_unique<TrackingState>(e2,x2,r2,
                   GameStateType::Playing, "Play"));
    EXPECT_EQ(x1, 1);   // old state exited
    EXPECT_EQ(e2, 1);   // new state entered
    ASSERT_NE(gm.GetCurrentState(), nullptr);
    EXPECT_EQ(gm.GetCurrentState()->GetName(), "Play");
}

TEST_F(GameManagerTest, NullPushIsIgnored)
{
    gm.PushState(nullptr);
    EXPECT_EQ(gm.GetCurrentState(), nullptr);
}

TEST_F(GameManagerTest, UpdateDelegatesToCurrentState)
{
    // Use a minimal stub that counts Update calls
    class CountingState : public GameState {
    public:
        int count = 0;
        void OnEnter() override {}
        void OnExit()  override {}
        void Update(float) override { ++count; }
        void Render()      override {}
        GameStateType GetType() const override { return GameStateType::Playing; }
        std::string   GetName() const override { return "Counting"; }
    };
    auto* raw = new CountingState;
    gm.PushState(std::unique_ptr<CountingState>(raw));
    gm.Update(0.016f);
    gm.Update(0.016f);
    EXPECT_EQ(raw->count, 2);
}

TEST_F(GameManagerTest, RenderDelegatesToCurrentState)
{
    class RenderCountState : public GameState {
    public:
        int count = 0;
        void OnEnter() override {}
        void OnExit()  override {}
        void Update(float) override {}
        void Render()      override { ++count; }
        GameStateType GetType() const override { return GameStateType::Playing; }
        std::string   GetName() const override { return "RenderCount"; }
    };
    auto* raw = new RenderCountState;
    gm.PushState(std::unique_ptr<RenderCountState>(raw));
    gm.Render();
    EXPECT_EQ(raw->count, 1);
}

TEST_F(GameManagerTest, UpdateOnEmptyStackDoesNotCrash)
{
    EXPECT_NO_THROW(gm.Update(0.016f));
}

TEST_F(GameManagerTest, RenderOnEmptyStackDoesNotCrash)
{
    EXPECT_NO_THROW(gm.Render());
}
