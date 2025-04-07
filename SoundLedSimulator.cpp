#include <iostream>
#include <portaudio.h>
#include <cmath>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

// 입력 콜백 함수
static int audioCallback(const void* inputBuffer, void*, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*) {
    const float* in = static_cast<const float*>(inputBuffer);
    float sum = 0;

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        sum += std::abs(in[i]);
    }

    float average = sum / framesPerBuffer;
    std::cout << "🎤 Mic Input Volume: " << average << std::endl;

    return paContinue;
}

int main() {
    Pa_Initialize();

    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, nullptr);
    Pa_StartStream(stream);

    std::cout << "▶ Listening... (press Enter to quit)\n";
    std::cin.get();

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
}
