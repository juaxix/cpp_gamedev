#include "Audio.hpp"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <errno.h>
#include <time.h>
#include <stdint.h>
inline void Sleep(uint32_t ms)
{
	struct timespec ts, rem;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
		ts = rem;
}
#endif

using namespace std;
#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		enabled = false; \
		return ;		\
		}

Audio::Audio()
{

}

void Audio::play(const char *filename)
{
	/*alure::DeviceManager *devMgr = alure::DeviceManager::get();

	alure::Device *dev = devMgr->openPlayback();
	cout << "Opened \"" << dev->getName(alure::PlaybackDevType_Basic) << "\"" << endl;

	alure::Context *ctx = dev->createContext();
	alure::Context::MakeCurrent(ctx);

	
	alure::Buffer *buffer = ctx->getBuffer(filename);
	alure::Source *source = ctx->getSource();
	source->play(buffer);
	cout << "Playing " << filename << " (" << alure::GetSampleTypeName(buffer->getSampleType()) << ", "
		<< alure::GetSampleConfigName(buffer->getSampleConfig()) << ", "
		<< buffer->getFrequency() << "hz)" << endl;

	float invfreq = 1.0f / buffer->getFrequency();
	while (source->isPlaying())
	{
		//cout << "\r " << setiosflags(ios::fixed) << setprecision(2) <<(source->getOffset()*invfreq) << " / " << (buffer->getLength()*invfreq);
		cout.flush();
		Sleep(25);
		ctx->update();
	}
	cout << endl;

	source->release();
	source = 0;
	ctx->removeBuffer(buffer);
	buffer = 0;
	

	alure::Context::MakeCurrent(0);
	ctx->destroy();
	ctx = 0;
	dev->close();
	dev = 0;*/
}

void Audio::initialize(){
	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	this->play("resources/bomb.ogg");
}

/*
void Audio::initialize()
{
	ALboolean enumeration;
	const ALCchar *devices;
	const ALCchar *defaultDeviceName = ""; // argv[1];
	int ret;
	char *bufferData;
	ALCdevice *device;
	ALvoid *data;
	ALCcontext *context;
	ALsizei size, freq;
	ALenum format;
	ALuint buffer, source;
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;
	ALint source_state;

	fprintf(stdout, "Using " BACKEND " as audio backend\n");

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	if (!defaultDeviceName)
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		enabled = false;
		return;//-1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		enabled = false;
		return; // -1;

	}
	TEST_ERROR("make default context");

	//set orientation 
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source);
	TEST_ERROR("source generation");

	alSourcef(source, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer);
	TEST_ERROR("buffer generation");

#ifdef LIBALURE
	//buffer = alureCreateBufferFromFile("resources/bomb.ogg");
	alureStream* stream = alureCreateStreamFromFile("resources/bomb.ogg", 19200, 2, &buffer);
	freq = alureGetStreamFrequency(stream);
	

	TEST_ERROR("loading ogg file");
	
	alBufferData(buffer, format, data, size, freq);
	TEST_ERROR("buffer copy");
	/* USE libaudio instead of alut (include/audio/wave.h) : */
	/*wave = WaveOpenFileForReading("resources/robot_go.wav");
	if (!wave) {
	fprintf(stderr, "failed to read wave file\n");
	return -1;
	}

	ret = WaveSeekFile(0, wave);
	if (ret) {
	fprintf(stderr, "failed to seek wave file\n");
	return -1;
	}

	bufferData = malloc(wave->dataSize);
	if (!bufferData) {
	perror("malloc");
	return -1;
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
	fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
	return -1;
	}

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
	bufferData, wave->dataSize, wave->sampleRate);
	TEST_ERROR("failed to load buffer data");
	*/
	/*
#else
	wchar_t* file = L"resources/robot_go.wav";
	ALbyte filename = *file;

	alutLoadWAVFile(&filename, &format, &data, &size, &freq, &loop);

	TEST_ERROR("loading wav file");

	alBufferData(buffer, format, data, size, freq);
	TEST_ERROR("buffer copy");

#endif
	alSourcei(source, AL_BUFFER, buffer);
	TEST_ERROR("buffer binding");

	alSourcePlay(source);
	TEST_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	TEST_ERROR("source state get");
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
	}

	// exit context 
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	enabled = true;

}
*/

void Audio::update()
{
    
}

void Audio::list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

ALenum Audio::to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}

Audio::~Audio()
{

}