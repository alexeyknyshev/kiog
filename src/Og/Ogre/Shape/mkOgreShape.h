//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGRESHAPE_H_INCLUDED
#define MK_OGRESHAPE_H_INCLUDED

#include <OGRE/OgreColourValue.h>

namespace Ogre
{
	class ManualObject;
}

namespace mk
{
	class MK_OG_EXPORT ManualShape
	{
	public:
		ManualShape(const Ogre::ColourValue& colour = Ogre::ColourValue(1.f, 1.f, 1.f, 1.f), bool plain = true, const Ogre::ColourValue& fillColour = Ogre::ColourValue(1.f, 1.f, 1.f, 0.5f));

		void setColour(Ogre::ColourValue colour, Ogre::ColourValue fillColour) { mColour = colour; mFillColour = fillColour; }
		void setPlain(bool plain) { mPlain = plain; }

		virtual int outlineVertexCount() = 0;
		virtual int outlineIndexCount() = 0;

		virtual void drawOutline(Ogre::ManualObject* manualObject, int lastIndex) = 0;

		virtual int plainVertexCount() = 0;
		virtual int plainIndexCount() = 0;

		virtual void drawPlain(Ogre::ManualObject* manualObject, int lastIndex) = 0;

	protected:
		Ogre::ColourValue mColour;
		bool mPlain;
		Ogre::ColourValue mFillColour;
	};
}

#endif
