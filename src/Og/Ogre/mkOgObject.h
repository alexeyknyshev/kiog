//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGOBJECT_H_INCLUDED
#define MK_OGOBJECT_H_INCLUDED

/* mk headers */
#include <Object/mkStem.h>
#include <Object/Threading/mkScheduler.h>
#include <Og/mkOgForward.h>

namespace mk
{
	class MK_OG_EXPORT OgObject : public Object
    {
    public:
		OgObject(Ogre::SceneManager* sceneMgr);
        ~OgObject();

		Ogre::SceneNode* sceneNode() { return mOgreSceneNode; }

    private:
        Ogre::SceneNode* mOgreSceneNode;
    };
}

#endif // MK_OGOBJECT_H_INCLUDED
