//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Widget/mkWOgViewport.h>

#include <Og/mkOgModule.h>
#include <Og/Ogre/mkOgreModule.h>

#include <Ui/Frame/mkFrame.h>

#include <Ui/mkUiWindow.h>

#include <Og/Gorilla/mkGorillaWindow.h>

/* Ogre headers */
#include <OgreSceneManager.h>
#include <OgreTexture.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreMaterial.h>
#include <OgreTextureUnitState.h>

#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/OgreCompositorManager2.h>

/* MOC picking header */
#include <Og/Ogre/CollisionTools/CollisionTools.h>

namespace mk
{
	WOgViewport::WOgViewport(Form* form, Ogre::Camera* camera, MOC::CollisionTools* collisionTools)
		: Sheet("", form)
		, OgViewport(camera, collisionTools)
		, mRenderTexture(nullptr)
		, mBlank(true)
	{}

	WOgViewport::~WOgViewport()
	{}

	void WOgViewport::build()
	{
		mOgreWindow = static_cast<OgreWindow*>(this->uiWindow()->renderWindow());
		mGorillaWindow = static_cast<GorillaWindow*>(this->uiWindow()->inkWindow());
		mRenderWindow = mOgreWindow->context();
		mRoot = mOgreWindow->ogreModule()->ogreRoot();
	}

	void WOgViewport::initViewport()
	{
		mBlank = false;

		mMaterialName = mForm->fullIndex() + "ViewportRTT";

		mMaterial = Ogre::MaterialManager::getSingleton().create(mMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();

		this->initTexture();

		mGorillaViewport = static_cast<Gorilla::Screen*>(mGorillaWindow->gorillaScreenTarget()->screen())->addViewport(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y), mMaterial);

		OgViewport::resize();

		mController->take(this);
	}

	void WOgViewport::initTexture()
	{
		mTexture = mRoot->getTextureManager()->createManual(mMaterialName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			size_t(mWidth),
			size_t(mHeight),
			0,
			Ogre::PF_R8G8B8,
			Ogre::TU_RENDERTARGET);

		mRenderTexture = mTexture->getBuffer()->getRenderTarget();

		mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(mTexture);
		
		mWorkspace = mRoot->getCompositorManager2()->addWorkspace(mSceneManager, mRenderTexture, mCamera, "Basic Workspace", true);
	}

	void WOgViewport::resizeTexture()
	{
		/*mRoot->getTextureManager()->remove(mTexture->getHandle());
		mRoot->getCompositorManager2()->removeWorkspace(mWorkspace);

		this->initTexture();*/

		mGorillaViewport->resize(mLeft, mTop, mWidth, mHeight);
	}

	void WOgViewport::nextFrame(size_t tick, size_t delta)
	{
		if(mFrame->dirty())
			resize();

		Widget::nextFrame(tick, delta);
	}

    void WOgViewport::resize()
    {
		mLeft = mFrame->dabsolute(DIM_X);
		mTop = mFrame->dabsolute(DIM_Y);
		mWidth = mFrame->dsize(DIM_X);
		mHeight = mFrame->dsize(DIM_Y);

		if(mWidth && mHeight)
		{
			if(mBlank)
				this->initViewport();
			else
				this->resizeTexture();
		}

		OgViewport::resize();
    }

	void WOgViewport::viewportRay(float x, float y, float* from, float* to)
	{
		float tx = x / mWidth;
		float ty = y / mHeight;

		Ogre::Ray ray = mCamera->getCameraToViewportRay(tx, ty);

		from[0] = ray.getOrigin()[0]; from[1] = ray.getOrigin()[1]; from[2] = ray.getOrigin()[2];
		from[0] = ray.getDirection()[0]; from[1] = ray.getDirection()[1]; from[2] = ray.getDirection()[2];

		//to *= mCamera->getFarClipDistance();
		//to += from;

		//mCamera->viewportRay(tx, ty, from, to);
	}

	bool WOgViewport::leftClick(float x, float y)
	{
		x -= mFrame->dabsolute(DIM_X);
		y -= mFrame->dabsolute(DIM_Y);

		float tx = x / mWidth;
		float ty = y / mHeight;

		Object* object = this->pickObject(tx, ty, SELECTABLE_OGRE_MASK);
		leftPicked(object, this->uiWindow()->shiftPressed(), tx, ty);
		return true;
	}

	bool WOgViewport::rightClick(float x, float y)
	{
		x -= mFrame->dabsolute(DIM_X);
		y -= mFrame->dabsolute(DIM_Y);

		float tx = x / mWidth;
		float ty = y / mHeight;

		Object* object = this->pickObject(tx, ty, SELECTABLE_OGRE_MASK | WORLDPAGE_OGRE_MASK);
		rightPicked(object, this->uiWindow()->shiftPressed(), tx, ty);
		return true;
	}
}
