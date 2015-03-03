//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkSharedBuffer.h>

#include <iostream>

namespace mk
{
	SharedBuffer::SharedBuffer(const string& fileName, const ReleaseCallback& onRelease)
		: mALBuffer(AL_NONE)
		, mSoundFileBuffer(0)
		, mNumUsers(0)
		, mOnRelease(onRelease)
	{
		if(fileName.find(".ogg") != fileName.npos || fileName.find(".OGG") != fileName.npos)
			mSoundFileBuffer = new/*sound*/OggFileBuffer();

		alGenBuffers(1, &mALBuffer);

		mSoundFileBuffer->open(fileName);
		mSoundFileBuffer->fill(mALBuffer);
	}

	SharedBuffer::~SharedBuffer()
	{
		alDeleteBuffers(1, &mALBuffer);

		mSoundFileBuffer->close();
	}

	void SharedBuffer::use()
	{
		++mNumUsers;
	}

	void SharedBuffer::release()
	{
		--mNumUsers;
		if(mNumUsers == 0)
			mOnRelease(this);
	}
}
