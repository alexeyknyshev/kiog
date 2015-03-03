//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/Shape/mkOgreShape.h>

namespace mk
{
	ManualShape::ManualShape(const Ogre::ColourValue& colour, bool plain, const Ogre::ColourValue& fillColour)
		: mColour(colour)
		, mPlain(plain)
		, mFillColour(fillColour)
	{}
}
