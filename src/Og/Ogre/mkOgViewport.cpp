//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/mkOgViewport.h>

/* MOC picking header */
#include <Og/Ogre/CollisionTools/CollisionTools.h>

#include <iostream>

namespace mk
{
	OgViewport::OgViewport(Ogre::Camera* camera, MOC::CollisionTools* collisionTools)
		: mCamera(camera)
		, mSceneManager(camera->getSceneManager())
		, mWidth(0.f)
		, mHeight(0.f)
		, mCollisionTools(collisionTools)
	{}

	OgViewport::~OgViewport()
	{}

	void OgViewport::update()
	{}

    void OgViewport::resize()
    {
		if(mWidth != 0.f && mHeight != 0.f)
		{
			//std::cerr << "Resize OgViewport to " << mWidth << " , " << mHeight << std::endl;
			Ogre::Real aspectRatio(Ogre::Real(mWidth) / Ogre::Real(mHeight));
			mCamera->setAspectRatio(aspectRatio);
		}
    }

	Object* OgViewport::pickObject(float tx, float ty, uint32_t queryMask)
	{
		Ogre::Ray ray = mCamera->getCameraToViewportRay(tx, ty);
		
		Ogre::Vector3 result;
		Ogre::MovableObject* target;

		float f = 0.f;
		if (mCollisionTools->raycast(ray, result, target, f, queryMask))
		{
			Ogre::SceneNode* node = target->getParentSceneNode();
			const Ogre::Any& any = node->getUserObjectBindings().getUserAny();
			
			// std::cerr << " picked at " << result.x << ", " << result.y << ", " << result.z << std::endl;
			if (!any.isEmpty())
				return Ogre::any_cast<Object*> (any);
		}

		return nullptr;
	}

	Ogre::Vector3 OgViewport::pickLocation(float tx, float ty)
	{
		Ogre::Ray ray = mCamera->getCameraToViewportRay(tx, ty);
		
		Ogre::Vector3 result;
		Ogre::MovableObject* target;
		
		float f = 0.f;
		if(mCollisionTools->raycast(ray, result, target, f, WORLDPAGE_OGRE_MASK))
			return result;

		return Ogre::Vector3(0, 0, 0);
	}
}
