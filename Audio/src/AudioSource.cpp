#include "AudioSource.h"

#include "al_utils.h"
#include "AudioDataWAV.h"
#include <AL/al.h>
#include <concepts>
#include <glm/ext/vector_float3.hpp>

static void _getSource(ALuint source, ALenum param, f32& value) { alGetSourcef(source, param, &value); }
static void _getSource(ALuint source, ALenum param, i32& value) { alGetSourcei(source, param, &value); }
static void _getSource(ALuint source, ALenum param, glm::vec3& value) { alGetSource3f(source, param, &value.x, &value.y, &value.z); }

static void _setSource(ALuint source, ALenum param, std::floating_point auto value) { alSourcef(source, param, static_cast<f32>(value)); }
static void _setSource(ALuint source, ALenum param, std::integral auto value) { alSourcei(source, param, static_cast<i32>(value)); }
static void _setSource(ALuint source, ALenum param, glm::vec3 const& value) { alSource3f(source, param, value.x, value.y, value.z); }

template <typename T>
static auto getSource(ALuint source, ALenum param) -> T
{
    T value;
    AL_CHECK_THROW(
        _getSource(source, param, value),
        "couldn't get the source's param!"
    );
    return value;
}

template <typename T>
static void setSource(ALuint source, ALenum param, T value)
{
    AL_CHECK_THROW(
        _setSource(source, param, value),
        "couldn't set the source's param!"
    );
}

UAudioSource::UAudioSource(UPath const& path)
{
    if (path.Extension() == ".wav" || path.Extension() == ".WAV")
    {
        UAudioDataWAV const data{ path };

        ALenum const format{ (data.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16 };

        AL_CHECK_THROW(
            alGenBuffers(1, &buffer_),
            "couldn't generate buffer!"
        );
        AL_CHECK_THROW(
            alBufferData(buffer_, format, data.pSampleData, static_cast<ALsizei>(data.dataSize), data.sampleRate),
            "couldn't set the buffer's data!"
        );
    }

    AL_CHECK_THROW(
        alGenSources(1, &source_),
        "couldn't generate source!"
    );

    setSource(source_, AL_BUFFER, buffer_);

    SetVolume(1.0f);
}

UAudioSource::~UAudioSource()
{
    AL_CHECK_THROW(
        alDeleteSources(1, &source_),
        "couldn't delete source!"
    );
    AL_CHECK_THROW(
        alDeleteBuffers(1, &buffer_),
        "couldn't delete buffer!"
    );
}

auto UAudioSource::GetState() const -> EAudioSourceState
{
    switch (getSource<i32>(source_, AL_SOURCE_STATE))
    {
    case AL_INITIAL: return EAudioSourceState::Initial;
    case AL_PLAYING: return EAudioSourceState::Playing;
    case AL_PAUSED: return EAudioSourceState::Paused;
    case AL_STOPPED: return EAudioSourceState::Stopped;
    default: throw std::runtime_error{ "unsupported source state!" };
    }
}

auto UAudioSource::GetVolume() const -> f32
{
    return getSource<f32>(source_, AL_GAIN);
}

auto UAudioSource::GetPitch() const -> f32
{
    return getSource<f32>(source_, AL_PITCH);
}

auto UAudioSource::GetIsLooping() const -> b8
{
    return getSource<i32>(source_, AL_LOOPING);
}

auto UAudioSource::GetPosition() const -> glm::vec3
{
    // Reverse Z
    return getSource<glm::vec3>(source_, AL_POSITION) * glm::vec3{ 1.0f, 1.0f, -1.0f };
}

auto UAudioSource::GetSpace() const -> EAudioSourceSpace
{
    return getSource<i32>(source_, AL_SOURCE_RELATIVE) == 0 ? EAudioSourceSpace::Scene : EAudioSourceSpace::Interface;
}

auto UAudioSource::GetAttenuationFactor() const -> f32
{
    return getSource<f32>(source_, AL_ROLLOFF_FACTOR);
}

auto UAudioSource::GetAttenuationStartDistance() const -> f32
{
    return getSource<f32>(source_, AL_REFERENCE_DISTANCE);
}

auto UAudioSource::GetAttenuationEndDistance() const -> f32
{
    return getSource<f32>(source_, AL_MAX_DISTANCE);
}

void UAudioSource::SetState(EAudioSourceState state) const
{
    switch (state)
    {
    case EAudioSourceState::Initial: return Rewind();
    case EAudioSourceState::Playing: return Play();
    case EAudioSourceState::Paused: return Pause();
    case EAudioSourceState::Stopped: return Stop();
    default: throw std::runtime_error{ "unsupported source state!" };
    }
}

void UAudioSource::SetVolume(f32 volume) const
{
    setSource(source_, AL_GAIN, volume);
}

void UAudioSource::SetPitch(f32 pitch) const
{
    setSource(source_, AL_PITCH, pitch);
}

void UAudioSource::SetIsLooping(b8 isLooping) const
{
    setSource(source_, AL_LOOPING, isLooping);
}

void UAudioSource::SetPosition(glm::vec3 const& position) const
{ 
    // Reverse Z
    setSource(source_, AL_POSITION, position * glm::vec3{ 1.0f, 1.0f, -1.0f });
}

void UAudioSource::SetSpace(EAudioSourceSpace space) const
{
    switch (space)
    {
    case EAudioSourceSpace::Scene: return setSource(source_, AL_SOURCE_RELATIVE, AL_FALSE);
    case EAudioSourceSpace::Interface: return setSource(source_, AL_SOURCE_RELATIVE, AL_TRUE);
    default: throw std::runtime_error{ "unsupported source space!" };
    }
}

void UAudioSource::SetAttenuationFactor(f32 factor) const
{
    setSource(source_, AL_ROLLOFF_FACTOR, factor);
}

void UAudioSource::SetAttenuationStartDistance(f32 distance) const
{
    setSource(source_, AL_REFERENCE_DISTANCE, distance);
}

void UAudioSource::SetAttenuationEndDistance(f32 distance) const
{
    setSource(source_, AL_MAX_DISTANCE, distance);
}

void UAudioSource::Play() const
{
    AL_CHECK_THROW(
        alSourcePlay(source_),
        "couldn't play source!"
    );
}

void UAudioSource::Pause() const
{
    AL_CHECK_THROW(
        alSourcePlay(source_),
        "couldn't pause source!"
    );
}

void UAudioSource::Rewind() const
{
    AL_CHECK_THROW(
        alSourcePlay(source_),
        "couldn't rewind source!"
    );
}

void UAudioSource::Stop() const
{
    AL_CHECK_THROW(
        alSourcePlay(source_),
        "couldn't stop source!"
    );
}
