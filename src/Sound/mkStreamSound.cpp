//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkStreamSound.h>

#include <al.h>

#include <Sound/mkOggFileBuffer.h>

namespace mk
{
#define NUM_BUFFERS 3

	StreamSound::StreamSound(SoundImplementer* manager, SoundCallback* callback)
		: Sound(manager, callback)
		, mNumBuffers(NUM_BUFFERS)
		, mALBuffers(mNumBuffers)
		, mBuffer(0)
		, mLastOffset(0)
	{
		mStream = true;
	}

	void StreamSound::open(const string& fileName)
	{
		//std::cerr << "opening stream sound" << std::endl;

		for(int i=0; i < mNumBuffers; i++)
		{
			mALBuffers[i] = AL_NONE;
		}

		alGenBuffers(mNumBuffers, &mALBuffers[0]);

		if(fileName.find(".ogg") != fileName.npos || fileName.find(".OGG") != fileName.npos)
			mBuffer = new/*sound*/OggFileBuffer();

		mBuffer->open(fileName);
		mDuration = mBuffer->getDuration();
		mSeekable = mBuffer->isSeekable();
		mMono = mBuffer->isMono();
	}

	StreamSound::~StreamSound()
	{
		delete mBuffer;
	}

	void StreamSound::setup()
	{

	}

	void StreamSound::release()
	{
		for(int i = 0; i < mNumBuffers; i++)
		{
			if (mALBuffers[i] != AL_NONE)
				alDeleteBuffers(1, &mALBuffers[i]);
		}
	}

	void StreamSound::rewind()
	{
		if(mBuffer->isSeekable())
		{
			mBuffer->seekTime(0);
			mCursor = 0;
		}
		else
		{
			mBuffer->reopen();
		}

		mLastOffset = 0;
	}

	void StreamSound::updateBuffers()
	{
		int processed;

		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);

		while(processed--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(mSource, 1, &buffer);

			ALint size, bits, channels, freq;

			alGetBufferi(buffer, AL_SIZE, &size);
			alGetBufferi(buffer, AL_BITS, &bits);
			alGetBufferi(buffer, AL_CHANNELS, &channels);
			alGetBufferi(buffer, AL_FREQUENCY, &freq);    

			mLastOffset += ((ALuint)size/channels/(bits/8)) / (ALfloat)freq;

			bool ended = mBuffer->fillChunk(buffer);

			if(ended && mLoop)
				rewind();

			alSourceQueueBuffers(mSource, 1, &buffer);
		}
	}

	void StreamSound::fillBuffers()
	{
		int i = 0;
		while(i < NUM_BUFFERS)
		{
			bool active = mBuffer->fillChunk(mALBuffers[i]);

			if(active)
				alSourceQueueBuffers(mSource, 1, &mALBuffers[i++]);
			else
				break;
		}
	}

	void StreamSound::clearBuffers()
	{
		int queued;
		ALuint buffer;

		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);

		while (queued--)
		{
			alSourceUnqueueBuffers(mSource, 1, &buffer);
			checkError();
		}
	}

	void StreamSound::updatePlayCursor()
	{
		if(mBuffer->isSeekable())
			mBuffer->seekTime(mCursor);

		mUpdateCursor = false;
		mLastOffset = mCursor;
	}

	ALfloat StreamSound::getPlayCursor()
	{
		ALfloat pos;
		alGetSourcef(mSource, AL_SEC_OFFSET, &pos);

		if ((mLastOffset + pos) >= mDuration) 
			return (mLastOffset + pos) - mDuration;
		else
			return mLastOffset + pos;
	}
}
