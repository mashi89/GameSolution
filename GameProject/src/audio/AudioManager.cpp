#include "include/audio/AudioManager.h"

AudioManager& AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::Initialize()
{
    return true;
}

void AudioManager::Shutdown()
{
    m_SoundPaths.clear();
}

void AudioManager::LoadSound(const std::string& id, const std::string& filepath)
{
    m_SoundPaths[id] = filepath;
}

void AudioManager::PlaySound(const std::string& id, float volume, bool loop)
{
}

void AudioManager::StopSound(const std::string& id)
{
}

void AudioManager::StopAll()
{
}

void AudioManager::SetMasterVolume(float v)
{
    m_MasterVolume = v;
}
