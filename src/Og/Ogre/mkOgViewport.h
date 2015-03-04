//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGVIEWPORT_H_INCLUDED
#define MK_OGVIEWPORT_H_INCLUDED

/* mk Headers */
#include <Og/mkOgForward.h>
#include <Og/Ogre/mkOgObject.h>

/* Ogre */
#include <OgreMaterialManager.h>

namespace Ogre
{
	class Viewport;
}

namespace MOC
{
	class CollisionTools;
}

namespace mk
{
	enum OgreMask
	{
		WORLDPAGE_OGRE_MASK = 1 << 0,
		SHADEABLE_OGRE_MASK = 1 << 1,
		SELECTABLE_OGRE_MASK = 1 << 2
	};

	class MK_OG_EXPORT OgViewport
	{
	public:
		OgViewport(Ogre::Camera* camera, MOC::CollisionTools* collisionTools);
		~OgViewport();

		float left() { return mLeft; }
		float top() { return mTop; }
		float width() { return mWidth; }
		float height() { return mHeight; }

		Ogre::Camera* ogreCamera() { return mCamera; }
		Ogre::SceneManager* sceneManager() { return mSceneManager; }

		void setCamera(Ogre::Camera* camera);

		void update();
		void resize();
	
		Object* pickObject(float x, float y, uint32_t queryMask = 0xFFFFFFFF);
		Ogre::Vector3 pickLocation(float x, float y);

	protected:
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneManager;

		float mLeft;
		float mTop;
		float mWidth;
		float mHeight;

		MOC::CollisionTools* mCollisionTools;
	};
}

#endif // MK_OGVIEWPORT_H_INCLUDED
