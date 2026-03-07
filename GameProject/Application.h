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
