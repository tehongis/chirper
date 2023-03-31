#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

volatile sig_atomic_t running;

void inthandler(int signum) {
    fflush(stdout);
    printf("Break by %d!\n",signum);
    exit(0);
}

int main()
{

    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }


    // Set up SDL audio
    SDL_AudioSpec audio_spec;
    audio_spec.freq = 22050;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 256;
    audio_spec.callback = NULL;
    audio_spec.userdata = NULL;

    // Open the default audio device
    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    if (audio_device == 0) {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
        return 1;
    }
    
    signal(SIGINT, inthandler);

    char buffer[65536];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            SDL_PauseAudioDevice(audio_device, 0);
            // repeating same buffer few times to get longer audio from short strings
            SDL_QueueAudio(audio_device, buffer, strlen(buffer));
            SDL_QueueAudio(audio_device, buffer, strlen(buffer));
            SDL_QueueAudio(audio_device, buffer, strlen(buffer));
            SDL_QueueAudio(audio_device, buffer, strlen(buffer));
            printf("%s",buffer);
            while (SDL_GetQueuedAudioSize(audio_device) > 0) {
                SDL_Delay(10);
            }
        }
    SDL_CloseAudioDevice(audio_device);

    return 0;
}
