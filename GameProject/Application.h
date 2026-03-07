#pragma once

class Application
{
public:
    Application();
    virtual ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

protected:
    virtual void Update();
    virtual void Render();

private:
    bool m_Running;
};

// Inline implementations
inline Application::Application()
    : m_Running(false)
{
}

inline Application::~Application()
{
    Shutdown();
}

inline bool Application::Initialize()
{
    // basic initialization logic
    m_Running = true;
    return true;
}

inline void Application::Run()
{
    while (m_Running)
    {
        Update();
        Render();
        // for this stub break immediately
        m_Running = false;
    }
}

inline void Application::Shutdown()
{
    m_Running = false;
}

inline void Application::Update() {}
inline void Application::Render() {}

