//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_STREAMSOUND_H_INCLUDED
#define MK_STREAMSOUND_H_INCLUDED

/* mk headers */
#include <Sound/mkSound.h>
#include <Sound/mkSoundFileBuffer.h>

/* Standard */
#include <vector>

namespace mk
{
	class StreamSound : public Sound
	{
	public:
		StreamSound(SoundImplementer* manager, SoundCallback* callback = 0);
		~StreamSound();

		void open(const string& fileName);

		void setup();
		void release();

		void updateBuffers();
		void fillBuffers();
		void clearBuffers();

		void rewind();
		void updatePlayCursor();
		ALfloat getPlayCursor();

	private:
		// Streaming sounds have local buffers
		int mNumBuffers;
		std::vector<ALuint> mALBuffers;
		SoundFileBuffer* mBuffer;

		ALfloat mLastOffset;
	};
}

#endif // MK_STREAMSOUND_H_INCLUDED
