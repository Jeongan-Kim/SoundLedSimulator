#include <iostream>    // 콘솔 출력용 (std::cout)
#include <portaudio.h> // 마이크 오디오 처리 라이브러리
#include <cmath>       // abs() 같은 수학 함수

#define SAMPLE_RATE 44100           // 샘플링 주기 (1초당 44100개 샘플)
//#define FRAMES_PER_BUFFER 256       // 256개 프레임 단위로 콜백 호출됨
//#define FRAMES_PER_BUFFER 512       // 512개 프레임 단위로 콜백 호출됨
#define FRAMES_PER_BUFFER 10000       // 2048개 프레임 단위로 콜백 호출됨

using namespace std;

// 입력 콜백 함수(portaudio가 오디오 데이터를 받으면 이 함수를 자동으로 호출함) -> 한번에 256개의 float 샘플이 들어옴

// 볼륨 평균 값을 출력
static int audioCallback_volumeValue(const void* inputBuffer, void*, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*) 
{
	const float* in = static_cast<const float*>(inputBuffer); // inputBuffer를 float 포인터로 변환
    float sum = 0; //평균 볼륨 계산

    for (unsigned long i = 0; i < framesPerBuffer; ++i) 
    {
		sum += abs(in[i]); // 절대값을 더함 (소리의 크기)
    }

    float average = sum / framesPerBuffer;
    cout << "🎤 Mic Input Volume: " << average << endl;

    return paContinue;
}

// 볼륨을 바 형태로 출력
static int audioCallback_volumeBar(const void* inputBuffer, void*, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*)
{
    const float* in = static_cast<const float*>(inputBuffer); // inputBuffer를 float 포인터로 변환
    float sum = 0; //평균 볼륨 계산

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        sum += abs(in[i]); // 절대값을 더함 (소리의 크기)
    }

    float average = sum / framesPerBuffer;

	float barOne = 0.01f; // 바 하나의 길이 (0.01)
	int barNum = average / 0.01f; // 바의 개수

	// 바 출력
    cout << "🎤 Mic Input Volume: ";

	for (int i = 0; i < barNum; ++i)
	{
		cout << "@"; // 바 출력
	}
    cout << endl;

    return paContinue;
}

// 볼륨을 바 형태로 출력(최대 볼륨을 제한, 볼륨바는 진한 네모, 아닌 부분은 연한 네모)
static int audioCallback_volumeBarMaxExist(const void* inputBuffer, void*, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*)
{
    const float* in = static_cast<const float*>(inputBuffer); // inputBuffer를 float 포인터로 변환
    float sum = 0; //평균 볼륨 계산

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        sum += abs(in[i]); // 절대값을 더함 (소리의 크기)
    }

    float average = sum / framesPerBuffer;

	int barMax = 10; // 최대 바 개수
    float barOne = 0.05f; // 바 하나의 길이 (0.05)
    int barNum = average / barOne; // 바의 개수(색칠 네모)

    // 바 출력
    cout << "🎤 Mic Input Volume: ";

	for (int i = 0; i < barMax; ++i)
	{
		if (i < barNum) // 볼륨만큼
		{
            cout << "●"; // 색칠된 동그라미 출력
		}
		else // 나머지 볼륨
		{
            cout << "○"; // 안색칠된 동그라미 출력
		}
	}

    cout << endl;

    return paContinue;
}

// 볼륨이 일정 크기 이상이면 경고메세지 출력
static int audioCallback_volumeWarning(const void* inputBuffer, void*, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*)
{
    const float* in = static_cast<const float*>(inputBuffer); // inputBuffer를 float 포인터로 변환
    float sum = 0; //평균 볼륨 계산

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        sum += abs(in[i]); // 절대값을 더함 (소리의 크기)
    }

    float average = sum / framesPerBuffer;

	float volumeMax = 0.4f; // 최대 볼륨 (2.0f)

    // 볼륨 체크
	if (average > volumeMax) // 볼륨이 최대 볼륨보다 크면
	{
		cout << "⚠️ Warning: Mic Input Volume is too high! ⚠️" << endl; // 경고메세지 출력
	}
	else
	{
		cout << "✅ Mic Input Volume is normal." << endl; // 정상 메세지 출력
	}

    return paContinue;
}

int main() {
	Pa_Initialize(); // PortAudio 초기화

    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback_volumeWarning, nullptr); // 기본 마이크 입력 열기(콜백함수 등록)
    Pa_StartStream(stream); // 마이크 입력 시작

    cout << "▶ Listening... (press Enter to quit)\n";
	cin.get(); // Enter 키 입력 대기

	Pa_StopStream(stream); // 마이크 입력 중지
	Pa_CloseStream(stream); // 스트림 닫기
	Pa_Terminate(); // PortAudio 종료
    return 0;
}
