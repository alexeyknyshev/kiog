//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGCONFIG_H_INCLUDED
#define MK_OGCONFIG_H_INCLUDED

#include <Object/mkObjectConfig.h>
#include <Sound/mkSoundConfig.h>
#include <Ui/mkUiConfig.h>

#if defined OG_EXPORT
#define MK_OG_EXPORT __declspec(dllexport)
#else
#define MK_OG_EXPORT __declspec(dllimport) 
#endif

#endif // MK_OGCONFIG_H_INCLUDED
