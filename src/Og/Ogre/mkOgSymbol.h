//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGRESYMBOL_H_INCLUDED
#define MK_OGRESYMBOL_H_INCLUDED

/* mk */
#include <Object/mkTyped.h>
#include <Og/mkOgForward.h>

#include <OgrePrerequisites.h>

namespace mk
{
	class MK_OG_EXPORT OgSymbol : public Object, public Typed<OgSymbol>
	{
	public:
		OgSymbol(Object* object, Ogre::SceneNode* sceneNode);
		~OgSymbol();

		void addShape(ManualShape* shape);
		void removeShape(ManualShape* shape);

		void setMask(unsigned int mask);

		void redraw();

	private:
		Ogre::SceneNode* mSceneNode;
		Ogre::ManualObject* mManualObject;
		std::vector<ManualShape*> mShapes;
	};
}

#endif // MK_OGRESYMBOL_H_INCLUDED
