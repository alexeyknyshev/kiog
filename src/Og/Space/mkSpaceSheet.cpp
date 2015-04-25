#include <Og/mkOgConfig.h>
#include <Og/Space/mkSpaceSheet.h>

#include <Ui/Frame/mkFrame.h>
#include <Ui/Frame/mkInk.h>
#include <Ui/mkUiWindow.h>

#include <Ui/Form/mkRootForm.h>

#include <Ui/Frame/mkLayer.h>

#include <Og/Space/mkSpaceViewport.h>
#include <Og/Gorilla/mkGorillaWindow.h>
#include <Og/Gorilla/Gorilla.h>

namespace mk
{
	void quadXY(Ogre::ManualObject* object, float left, float bottom, float width, float height, size_t index)
	{
		object->position(left, bottom, 0.f);
		object->normal(0.f, 0.f, -1.f);
		object->textureCoord(0.f, 0.f);

		object->position(left + width, bottom, 0.f);
		object->normal(0.f, 0.f, -1.f);
		object->textureCoord(1.f, 0.f);

		object->position(left + width, bottom + height, 0.f);
		object->normal(0.f, 0.f, -1.f);
		object->textureCoord(1.f, 1.f);

		object->position(left, bottom + height, 0.f);
		object->normal(0.f, 0.f, -1.f);
		object->textureCoord(0.f, 1.f);

		object->quad(index + 0, index + 1, index + 2, index + 3);
		object->quad(index + 3, index + 2, index + 1, index + 0);
	}

	Ogre::Vector3 plane_segment_intersection(const Ogre::Vector3& P1, const Ogre::Vector3& P2, const Ogre::Vector3& P3, const Ogre::Vector3& L1, const Ogre::Vector3& L2)
	{
		Ogre::Vector3 T(P2 - P1);
		Ogre::Vector3 N = T.crossProduct(P3 - P1);
		Ogre::Vector3 result = L1 + (N.dotProduct(P1 - L1) / N.dotProduct(L2 - L1)) * (L2 - L1);
		return result;
	}

	SpaceSheet::SpaceSheet(SpaceViewport& viewport)
		: RootSheet(viewport.uiWindow(), nullptr, false)
		, mViewport(viewport)
		, mTarget(uiWindow().inkWindow().as<GorillaWindow>().spaceTarget(mViewport.ogreCamera()->getParentSceneNode()->getCreator(), 0, 0).as<GorillaSpaceTarget>())
	{
		mStyle = &cls();

		mFrame = make_unique<Layer3D>(*this, 0, &mTarget);
		
		mOgreCamera = mViewport.ogreCamera();
		mSceneNode = mTarget.spaceScreen().getParentSceneNode();
		mSceneNode->getUserObjectBindings().setUserAny(Ogre::Any(static_cast<Object*>(this)));
		mDummyRect = mSceneNode->getCreator()->createManualObject();
		mSceneNode->attachObject(mDummyRect);

		mDummyRect->setQueryFlags(SPACESHEET_OGRE_MASK);

		mDummyRect->begin("debug_invisible");
		mDummyRect->position(Ogre::Vector3::ZERO);
		mDummyRect->normal(Ogre::Vector3::ZERO);
		mDummyRect->textureCoord(Ogre::Vector2::ZERO);
		mDummyRect->index(0);
		mDummyRect->end();

		mTarget.spaceScreen().ratio() = 0.01f;

		//this->updateSize();
	}

	SpaceSheet::~SpaceSheet()
	{
		Sheet::clear(); // @kludge : destroy the contents now because layer() is a virtual function and is needed in the destructor
		this->cleanup();
		mFrame.reset();
		this->uiWindow().inkWindow().as<GorillaWindow>().releaseTarget(mTarget);
	}

	void SpaceSheet::nextFrame(size_t tick, size_t delta)
	{
		bool dirty = mFrame->dirty();

		RootSheet::nextFrame(tick, delta);

		if(dirty)
			this->updateSize();

		mSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(0.2f * float(delta)), Ogre::Vector3::UNIT_Y));
	}

	void SpaceSheet::updateSize()
	{
		mTarget.spaceScreen().maxSize().x = mFrame->dsize(DIM_X);
		mTarget.spaceScreen().maxSize().y = mFrame->dsize(DIM_Y);

		mDummyRect->beginUpdate(0);
		float x = mFrame->dsize(DIM_X) * mTarget.spaceScreen().ratio();
		float y = mFrame->dsize(DIM_Y) * mTarget.spaceScreen().ratio();
		quadXY(mDummyRect, -x/2.f, -y/2.f, x, y, 0U);
		mDummyRect->end();
	}

	void SpaceSheet::transformCoordinates(float& x, float &y)
	{
		x -= mViewport.frame().dabsolute(DIM_X);
		y -= mViewport.frame().dabsolute(DIM_Y);

		Ogre::Vector3 rayOrigin;
		Ogre::Vector3 rayDir;

		mViewport.viewportRay(x, y, &rayOrigin[0], &rayDir[0]);

		Ogre::Vector3 p0 = mSceneNode->getPosition();
		Ogre::Vector3 p1 = mSceneNode->getOrientation().xAxis();
		Ogre::Vector3 p2 = mSceneNode->getOrientation().yAxis();

		Ogre::Vector3 pos = plane_segment_intersection(p0, p1, p2, rayOrigin, rayOrigin + rayDir * 1000.f);

		Ogre::Vector3 rel = mSceneNode->getOrientation().Inverse() * (pos - mSceneNode->getPosition());

		x = rel.x / mTarget.spaceScreen().ratio() + mFrame->dsize(DIM_X) / 2.f;
		y = -rel.y / mTarget.spaceScreen().ratio() + mFrame->dsize(DIM_Y) / 2.f;
	}

	InputReceiver* SpaceSheet::propagateMouse(float x, float y)
	{
		return mParent->parent(); // we skip the SpaceViewport
	}

	Widget* SpaceSheet::pinpoint(float x, float y, bool modal)
	{
		if(modal)
			return this;
		return Widget::pinpoint(x, y, modal);
	}
}