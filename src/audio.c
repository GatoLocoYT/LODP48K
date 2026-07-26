#include "audio.h"

#include <SDL2/SDL.h>
#include <stdint.h>

#define AUDIO_FREQUENCY 8000
#define AUDIO_BUFFER_SAMPLES 256
#define AUDIO_VOLUME_PERCENT 18

#define AUDIO_SHOT_DURATION 1600
#define AUDIO_SHOT_VOICES 2

#define AUDIO_DEATH_DURATION 8000
#define AUDIO_VICTORY_DURATION 16000
#define AUDIO_MUSIC_DURATION 524280

static SDL_AudioDeviceID gAudioDevice = 0;

static bool gMusicPlaying = false;
static uint32_t gMusicTime = 0;

static uint32_t
    gShotTimes[AUDIO_SHOT_VOICES];
static uint32_t
    gDeathTime = AUDIO_DEATH_DURATION;
static uint32_t
    gVictoryTime = AUDIO_VICTORY_DURATION;

static int Audio_ToSigned(uint8_t value)
{
    if (value < 128)
    {
        return value;
    }

    return (int)value - 256;
}

static uint8_t Audio_GetShotSample(
    uint32_t t)
{
    /*
     * Con el final pedido en t = 1600,
     * t >> 17 siempre es cero. Esta es
     * la rama activa de la formula dada.
     */
    if ((37649u &
         (1u << ((t >> 10) & 15u))) == 0)
    {
        return 0;
    }

    uint32_t value =
        ((t + 314u) * 128u /
         (((t >> 5) & 31u) + 1u)) &
        255u;

    return (uint8_t)(
        (value / 85u) * 85u);
}

static uint8_t Audio_GetMusicSample(
    uint32_t t)
{
    uint32_t value = 0;

    if (t > 0 && t < 65535u)
    {
        value =
            (t % 32u) * 10000u > t
                ? t >> 4
                : t >> 6;
    }
    else if (t > 65535u &&
             t < 131070u)
    {
        value =
            t % 32u > t % 43u
                ? t >> 4
                : t >> 6;
    }
    else if (t > 131070u &&
             t < 196605u)
    {
        value =
            t % 36u > t % 43u
                ? t >> 4
                : t >> 6;
    }
    else if (t > 196605u &&
             t < 262140u)
    {
        value =
            t % 64u > t % 43u
                ? t >> 4
                : t >> 6;
    }
    else if (t > 262140u &&
             t < 327675u)
    {
        value =
            5u < t % 43u
                ? t << 3
                : t >> 6;
    }
    else if (t > 327675u &&
             t < 393210u)
    {
        value =
            5u < t % 27u
                ? t << 3
                : t >> 6;
    }
    else if (t > 393210u &&
             t < AUDIO_MUSIC_DURATION)
    {
        value =
            5u < t % 63u
                ? t << 3
                : t >> 6;
    }

    return (uint8_t)(
        value & (t >> 4));
}

static uint8_t Audio_GetDeathSample(
    uint32_t t)
{
    return (uint8_t)(
        (t * (60u -
              ((t >> 8) & 63u)) >>
         3) &
        (255u - (t >> 5)));
}

static uint8_t Audio_GetVictorySample(
    uint32_t t)
{
    uint32_t note =
        (0xA8656543u >>
         (((t >> 11) & 7u) * 4u)) &
        15u;

    return (uint8_t)(
        (((t * note) & 128u) *
         (255u - ((t >> 3) & 255u))) >>
        8);
}

static void Audio_Callback(
    void* userData,
    Uint8* stream,
    int length)
{
    (void)userData;

    Sint16* output = (Sint16*)stream;
    int sampleCount =
        length / (int)sizeof(Sint16);

    for (int i = 0; i < sampleCount; i++)
    {
        int mixedSample = 0;

        if (gMusicPlaying)
        {
            mixedSample +=
                Audio_ToSigned(
                    Audio_GetMusicSample(
                        gMusicTime));

            gMusicTime++;

            if (gMusicTime >=
                AUDIO_MUSIC_DURATION)
            {
                gMusicTime = 0;
            }
        }

        for (int voice = 0;
             voice < AUDIO_SHOT_VOICES;
             voice++)
        {
            if (gShotTimes[voice] >=
                AUDIO_SHOT_DURATION)
            {
                continue;
            }

            mixedSample +=
                Audio_ToSigned(
                    Audio_GetShotSample(
                        gShotTimes[voice]));

            gShotTimes[voice]++;
        }

        if (gDeathTime <
            AUDIO_DEATH_DURATION)
        {
            mixedSample +=
                Audio_ToSigned(
                    Audio_GetDeathSample(
                        gDeathTime));

            gDeathTime++;
        }

        if (gVictoryTime <
            AUDIO_VICTORY_DURATION)
        {
            mixedSample +=
                Audio_ToSigned(
                    Audio_GetVictorySample(
                        gVictoryTime));

            gVictoryTime++;
        }

        mixedSample =
            mixedSample *
            AUDIO_VOLUME_PERCENT / 100;

        output[i] =
            (Sint16)(mixedSample * 256);
    }
}

bool Audio_Init(void)
{
    if (SDL_InitSubSystem(
            SDL_INIT_AUDIO) != 0)
    {
        return false;
    }

    SDL_AudioSpec desired;
    SDL_AudioSpec obtained;

    SDL_zero(desired);
    SDL_zero(obtained);

    desired.freq = AUDIO_FREQUENCY;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples =
        AUDIO_BUFFER_SAMPLES;
    desired.callback = Audio_Callback;

    gAudioDevice =
        SDL_OpenAudioDevice(
            NULL,
            0,
            &desired,
            &obtained,
            0);

    if (gAudioDevice == 0 ||
        obtained.freq != AUDIO_FREQUENCY ||
        obtained.format != AUDIO_S16SYS ||
        obtained.channels != 1)
    {
        if (gAudioDevice != 0)
        {
            SDL_CloseAudioDevice(
                gAudioDevice);
            gAudioDevice = 0;
        }

        SDL_QuitSubSystem(
            SDL_INIT_AUDIO);
        return false;
    }

    Audio_StopAll();

    SDL_PauseAudioDevice(
        gAudioDevice,
        0);

    return true;
}

void Audio_StartGame(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_LockAudioDevice(gAudioDevice);

    gMusicTime = 0;
    gMusicPlaying = true;
    gDeathTime = AUDIO_DEATH_DURATION;
    gVictoryTime =
        AUDIO_VICTORY_DURATION;

    for (int i = 0;
         i < AUDIO_SHOT_VOICES;
         i++)
    {
        gShotTimes[i] =
            AUDIO_SHOT_DURATION;
    }

    SDL_UnlockAudioDevice(gAudioDevice);
}

void Audio_PlayShot(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_LockAudioDevice(gAudioDevice);

    int selectedVoice = 0;

    for (int i = 0;
         i < AUDIO_SHOT_VOICES;
         i++)
    {
        if (gShotTimes[i] >=
            AUDIO_SHOT_DURATION)
        {
            selectedVoice = i;
            break;
        }

        if (gShotTimes[i] >
            gShotTimes[selectedVoice])
        {
            selectedVoice = i;
        }
    }

    gShotTimes[selectedVoice] = 0;

    SDL_UnlockAudioDevice(gAudioDevice);
}

void Audio_PlayPlayerDeath(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_LockAudioDevice(gAudioDevice);

    gMusicPlaying = false;
    gDeathTime = 0;
    gVictoryTime =
        AUDIO_VICTORY_DURATION;

    for (int i = 0;
         i < AUDIO_SHOT_VOICES;
         i++)
    {
        gShotTimes[i] =
            AUDIO_SHOT_DURATION;
    }

    SDL_UnlockAudioDevice(gAudioDevice);
}

void Audio_PlayVictory(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_LockAudioDevice(gAudioDevice);

    gMusicPlaying = false;
    gDeathTime = AUDIO_DEATH_DURATION;
    gVictoryTime = 0;

    for (int i = 0;
         i < AUDIO_SHOT_VOICES;
         i++)
    {
        gShotTimes[i] =
            AUDIO_SHOT_DURATION;
    }

    SDL_UnlockAudioDevice(gAudioDevice);
}

void Audio_StopAll(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_LockAudioDevice(gAudioDevice);

    gMusicPlaying = false;
    gDeathTime = AUDIO_DEATH_DURATION;
    gVictoryTime =
        AUDIO_VICTORY_DURATION;

    for (int i = 0;
         i < AUDIO_SHOT_VOICES;
         i++)
    {
        gShotTimes[i] =
            AUDIO_SHOT_DURATION;
    }

    SDL_UnlockAudioDevice(gAudioDevice);
}

void Audio_Quit(void)
{
    if (gAudioDevice == 0)
    {
        return;
    }

    SDL_CloseAudioDevice(gAudioDevice);
    gAudioDevice = 0;

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
