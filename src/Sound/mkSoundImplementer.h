//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDIMPLEMENTER_H_INCLUDED
#define MK_SOUNDIMPLEMENTER_H_INCLUDED

/* mk headers */
#include <Sound/mkSoundForward.h>

namespace mk
{
	class SoundImplementer
	{
	public:
		virtual void playSound(Sound* sound) = 0;
		virtual void pauseSound(Sound* sound) = 0;
		virtual void stopSound(Sound* sound) = 0;
		virtual void destroySound(Sound* sound) = 0;

		virtual void updatePosition(Sound* sound, const btVector3& position) = 0;
	};
}

#endif // MK_SOUNDIMPLEMENTER_H_INCLUDED
