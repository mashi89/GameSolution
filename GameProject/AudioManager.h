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

// inline implementations
inline AudioManager& AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

inline bool AudioManager::Initialize()
{
    // stub initialization
    return true;
}

inline void AudioManager::Shutdown()
{
    m_SoundPaths.clear();
}

inline void AudioManager::LoadSound(const std::string& id, const std::string& filepath)
{
    m_SoundPaths[id] = filepath;
}

inline void AudioManager::PlaySound(const std::string& id, float volume, bool loop)
{
    // no-op stub
    (void)id; (void)volume; (void)loop;
}

inline void AudioManager::StopSound(const std::string& id)
{
    (void)id;
}

inline void AudioManager::StopAll()
{
    // no-op
}

inline void AudioManager::SetMasterVolume(float v)
{
    m_MasterVolume = v;
}

