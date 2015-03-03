//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDFORWARD_H_INCLUDED
#define MK_SOUNDFORWARD_H_INCLUDED

/* mk */
#include <Object/mkObjectForward.h>


typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

typedef char ALboolean;
typedef char ALchar;
typedef char ALbyte;
typedef unsigned char ALubyte;
typedef short ALshort;
typedef unsigned short ALushort;
typedef int ALint;
typedef unsigned int ALuint;
typedef int ALsizei;
typedef int ALenum;
typedef float ALfloat;
typedef double ALdouble;
typedef void ALvoid;

class btVector3;
class btQuaternion;

namespace mk
{
	class Sound;
	class SoundManager;
	class SoundCallback;
	class SoundImplementer;
	class SoundListener;

	class SharedBuffer;
}

#endif // MK_SOUNDFORWARD_H_INCLUDED
