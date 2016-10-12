#ifndef __opengl_sample__Audio__
#define __opengl_sample__Audio__
#include <iostream>

//#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
//#include <unistd.h>
#include <stdbool.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <iostream>
#include <iomanip>

#define LIBALURE
#define BUFFER_SIZE   32768     // 32 KB buffers
#ifdef LIBALURE
#include <AL/alure.h>
#define BACKEND	"ALURE"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif

class Audio
{

	bool enabled = false;
	void loadOGG(char *fileName, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
public:
	Audio();
	void initialize();
	void play(const char* filename);
    void update();
	static void list_audio_devices(const ALCchar *devices);
	static inline ALenum Audio::to_al_format(short channels, short samples);
	~Audio();
};

#endif /* defined(__opengl_sample__Audio__) */
