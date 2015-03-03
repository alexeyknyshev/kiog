//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDCONFIG_H_INCLUDED
#define MK_SOUNDCONFIG_H_INCLUDED

#include <Object/mkObjectConfig.h>

#if defined SOUND_EXPORT
#define MK_SOUND_EXPORT __declspec(dllexport)
#else
#define MK_SOUND_EXPORT __declspec(dllimport) 
#endif



#endif // MK_SOUNDCONFIG_H_INCLUDED
