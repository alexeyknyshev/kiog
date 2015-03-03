//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/mkOgObject.h>

/* Ogre headers */
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>

namespace mk
{
    OgObject::OgObject(Ogre::SceneManager* sceneManager)
		: mOgreSceneNode(sceneManager->getRootSceneNode()->createChildSceneNode())
    {
		mOgreSceneNode->getUserObjectBindings().setUserAny(Ogre::Any(this));
	}

    OgObject::~OgObject()
    {
		mOgreSceneNode->getParentSceneNode()->removeChild(mOgreSceneNode);
		mOgreSceneNode->getCreator()->destroySceneNode(mOgreSceneNode);
    }
}
