//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDFILEBUFFER_H_INCLUDED
#define MK_SOUNDFILEBUFFER_H_INCLUDED

/* mk */
#include <Object/String/mkString.h>
#include <Sound/mkSoundForward.h>

namespace mk
{
	class MK_SOUND_EXPORT SoundFileBuffer
	{
	public:
		virtual void open(const string& fileName) = 0;
		virtual void close() = 0;
		virtual void reopen() = 0;

		virtual void seekTime(float time) = 0;
		virtual void seekBytes(long pos) = 0;

		virtual void fill(ALuint buffer) = 0;
		virtual bool fillChunk(ALuint buffer) = 0;

		virtual ALenum getFormat() = 0;
		virtual bool isSeekable() = 0;
		virtual bool isMono() = 0;
		virtual const string& getFileName() = 0;
		virtual ALfloat getDuration() = 0;
	};
}

#endif // MK_SOUNDFILEBUFFER_H_INCLUDED
