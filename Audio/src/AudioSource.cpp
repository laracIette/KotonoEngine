#include "AudioSource.h"

#include "al_utils.h"
#include "AudioDataWAV.h"
#include <AL/al.h>

UAudioSource::UAudioSource(UPath const& path)
{
    if (path.Extension() == ".wav" || path.Extension() == ".WAV")
    {
        UAudioDataWAV const data{ path };

        ALenum const format{ (data.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16 };

        AL_CHECK_THROW(
            alGenBuffers(1, &buffer_),
            "couldn't generate buffer"
        );
        AL_CHECK_THROW(
            alBufferData(buffer_, format, data.pSampleData, static_cast<ALsizei>(data.dataSize), data.sampleRate),
            "couldn't set the buffer's data"
        );
    }

    AL_CHECK_THROW(
        alGenSources(1, &source_),
        "couldn't generate source"
    );
    AL_CHECK_THROW(
        alSourcei(source_, AL_BUFFER, buffer_),
        "couldn't set the source's buffer"
    );

    SetVolume(1.0f);
}

UAudioSource::~UAudioSource()
{
    AL_CHECK_THROW(
        alDeleteSources(1, &source_),
        "couldn't delete source"
    );
    AL_CHECK_THROW(
        alDeleteBuffers(1, &buffer_),
        "couldn't delete buffer"
    );
}

auto UAudioSource::GetState() const -> EAudioSourceState
{
    ALint state; 
    AL_CHECK_THROW(
        alGetSourcei(source_, AL_SOURCE_STATE, &state),
        "couldn't get the source's state"
    );
    switch (state)
    {
    case AL_INITIAL: return EAudioSourceState::Initial;
    case AL_PLAYING: return EAudioSourceState::Playing;
    case AL_PAUSED: return EAudioSourceState::Paused;
    case AL_STOPPED: return EAudioSourceState::Stopped;
    default: throw std::runtime_error{ "unsupported source state" };
    }
}

void UAudioSource::SetState(EAudioSourceState state) const
{
    switch (state)
    {
    case EAudioSourceState::Initial: return Rewind();
    case EAudioSourceState::Playing: return Play();
    case EAudioSourceState::Paused: return Pause();
    case EAudioSourceState::Stopped: return Stop();
    default: throw std::runtime_error{ "unsupported source state" };
    }
}

auto UAudioSource::GetVolume() const -> f32
{
    f32 gain; 
    AL_CHECK_THROW(
        alGetSourcef(source_, AL_GAIN, &gain),
        "couldn't get the source's gain"
    );
    return gain;
}

void UAudioSource::SetVolume(f32 volume) const
{
    AL_CHECK_THROW(
        alSourcef(source_, AL_GAIN, volume),
        "couldn't set the source's gain"
    );
}

void UAudioSource::Play() const
{
    AL_CHECK_THROW(
        alSourcePlay(source_),
        "couldn't play the source"
    );
}

void UAudioSource::Pause() const
{
    AL_CHECK_THROW(
        alSourcePause(source_),
        "couldn't pause the source"
    );
}

void UAudioSource::Rewind() const
{
    AL_CHECK_THROW(
        alSourceRewind(source_),
        "couldn't rewind the source"
    );
}

void UAudioSource::Stop() const
{
    AL_CHECK_THROW(
        alSourceStop(source_),
        "couldn't stop the source"
    );
}

void UAudioSource::SetIsLooping(b8 isLooping) const
{
    AL_CHECK_THROW(
         alSourcei(source_, AL_LOOPING, isLooping),
         "couldn't set the source's looping value"
    );
}
