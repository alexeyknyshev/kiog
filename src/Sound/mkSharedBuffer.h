//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SHAREDBUFFER_H_INCLUDED
#define MK_SHAREDBUFFER_H_INCLUDED

/* mk headers */
#include <Object/String/mkString.h>
#include <Sound/mkSoundFileBuffer.h>
#include <Sound/mkOggFileBuffer.h>

/* Std headers */
#include <functional>

namespace mk
{
	class SharedBuffer
	{
	public:
		typedef std::function<void (SharedBuffer*)> ReleaseCallback;

	public:
		SharedBuffer(const string& fileName, const ReleaseCallback& onRelease);
		~SharedBuffer();

		bool isSeekable() { return mSoundFileBuffer->isSeekable(); }
		ALfloat getDuration() { return mSoundFileBuffer->getDuration(); }
		bool isMono() { return mSoundFileBuffer->isMono(); }
		const string& getFileName() const { return mSoundFileBuffer->getFileName(); }

		ALuint getALBuffer() { return mALBuffer; }

		void use();
		void release();

	private:
		ALuint mALBuffer;
		SoundFileBuffer* mSoundFileBuffer;

		int mNumUsers;
		ReleaseCallback mOnRelease;
	};
}

#endif // MK_SHAREDBUFFER_H_INCLUDED
