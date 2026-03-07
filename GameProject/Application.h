#pragma once
#include <chrono>

class Application
{
public:
    Application();
    virtual ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

protected:
    virtual void Update(float deltaTime);
    virtual void Render();

private:
    bool  m_Running;
    float m_DeltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
};
