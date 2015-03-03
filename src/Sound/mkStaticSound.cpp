//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkStaticSound.h>

#include <iostream>

namespace mk
{
	StaticSound::StaticSound(SoundImplementer* manager, SoundCallback* callback)
		: Sound(manager, callback)
		, mBuffer(0)
	{
		mStream = false;
	}

	StaticSound::~StaticSound()
	{}

	void StaticSound::openShared(SharedBuffer* buffer)
	{
		mBuffer = buffer;
		mBuffer->use();
		mDuration = mBuffer->getDuration();
		mSeekable = mBuffer->isSeekable();
		mMono = mBuffer->isMono();
	}

	void StaticSound::release()
	{
		mBuffer->release();
	}

	void StaticSound::updateBuffers()
	{}

	void StaticSound::fillBuffers()
	{
		alSourcei(mSource, AL_BUFFER, mBuffer->getALBuffer());
	}

	void StaticSound::clearBuffers()
	{
		alSourcei(mSource, AL_BUFFER, 0);
	}

	void StaticSound::rewind()
	{
		alSourceRewind(mSource);
	}

	void StaticSound::updatePlayCursor()
	{
		alSourcef(mSource, AL_SEC_OFFSET, mCursor);

		mUpdateCursor = false;
	}

	ALfloat StaticSound::getPlayCursor()
	{
		ALfloat cursor;
		alGetSourcef(mSource, AL_SEC_OFFSET, &cursor);
		return cursor;
	}

	void StaticSound::setLoop(bool loop)
	{
		Sound::setLoop(loop);

		if(mActive)
			alSourcei(mSource, AL_LOOPING, loop);
	}
}
