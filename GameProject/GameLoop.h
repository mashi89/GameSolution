#pragma once
#include <chrono>

class GameLoop
{
public:
    GameLoop();
    void  Start();
    void  Stop();
    bool  IsRunning()    const { return m_Running; }
    float GetDeltaTime() const { return m_DeltaTime; }
    float GetFPS()       const { return m_DeltaTime > 0 ? 1.0f / m_DeltaTime : 0.0f; }

private:
    bool  m_Running;
    float m_DeltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
};
