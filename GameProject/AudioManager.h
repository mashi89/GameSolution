#pragma once
#include <string>
#include <unordered_map>

class AudioManager
{
public:
    static AudioManager& GetInstance();
    bool Initialize();
    void Shutdown();
    void LoadSound(const std::string& id, const std::string& filepath);
    void PlaySound(const std::string& id, float volume = 1.0f, bool loop = false);
    void StopSound(const std::string& id);
    void StopAll();
    void  SetMasterVolume(float v);
    float GetMasterVolume() const { return m_MasterVolume; }

private:
    AudioManager() = default;
    float m_MasterVolume = 1.0f;
    std::unordered_map<std::string, std::string> m_SoundPaths;
};
