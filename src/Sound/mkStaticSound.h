//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_STATICSOUND_H_INCLUDED
#define MK_STATICSOUND_H_INCLUDED

/* mk headers */
#include <Sound/mkSound.h>
#include <Sound/mkSharedBuffer.h>

namespace mk
{
	class StaticSound : public Sound
	{
	public:
		StaticSound(SoundImplementer* manager, SoundCallback* callback = 0);
		~StaticSound();

		void openShared(SharedBuffer* buffer);
		void release();

		void updateBuffers();
		void fillBuffers();
		void clearBuffers();

		void rewind();
		void updatePlayCursor();
		ALfloat getPlayCursor();

		void setLoop(bool loop);

	private:
		SharedBuffer* mBuffer;
	};
}

#endif // MK_STATICSOUND_H_INCLUDED
