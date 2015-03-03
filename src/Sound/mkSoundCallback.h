//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDCALLBACK_H_INCLUDED
#define MK_SOUNDCALLBACK_H_INCLUDED

/* mk headers */
#include <Sound/mkSoundForward.h>

namespace mk
{
	class SoundCallback
	{
	public:
		virtual void soundDestroyed(Sound* sound) = 0;
	};
}

#endif // MK_SOUNDCALLBACK_H_INCLUDED
