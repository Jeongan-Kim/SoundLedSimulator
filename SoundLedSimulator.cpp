#include <iostream>    // 콘솔 출력용 (std::cout)
#include <portaudio.h> // 마이크 오디오 처리 라이브러리
#include <cmath>       // abs() 같은 수학 함수

#define SAMPLE_RATE 44100           // 샘플링 주기 (1초당 44100개 샘플)
#define FRAMES_PER_BUFFER 256       // 256개 프레임 단위로 콜백 호출됨


// 입력 콜백 함수(portaudio가 오디오 데이터를 받으면 이 함수를 자동으로 호출함) -> 한번에 256개의 float 샘플이 들어옴
static int audioCallback(const void* inputBuffer, void*, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*) 
{
	const float* in = static_cast<const float*>(inputBuffer); // inputBuffer를 float 포인터로 변환
    float sum = 0; //평균 볼륨 계산

    for (unsigned long i = 0; i < framesPerBuffer; ++i) 
    {
		sum += std::abs(in[i]); // 절대값을 더함 (소리의 크기)
    }

    float average = sum / framesPerBuffer;
    std::cout << "🎤 Mic Input Volume: " << average << std::endl;

    return paContinue;
}

int main() {
	Pa_Initialize(); // PortAudio 초기화

    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, nullptr); // 기본 마이크 입력 열기(콜백함수 등록)
    Pa_StartStream(stream); // 마이크 입력 시작

    std::cout << "▶ Listening... (press Enter to quit)\n";
	std::cin.get(); // Enter 키 입력 대기

	Pa_StopStream(stream); // 마이크 입력 중지
	Pa_CloseStream(stream); // 스트림 닫기
	Pa_Terminate(); // PortAudio 종료
    return 0;
}
