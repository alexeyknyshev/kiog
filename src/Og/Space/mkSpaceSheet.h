#ifndef MK_SPACESHEET_H_INCLUDED
#define MK_SPACESHEET_H_INCLUDED

/* mk Headers */
#include <Og/mkOgForward.h>
#include <Ui/Form/mkForm.h>
#include <Ui/Widget/mkRootSheet.h>
#include <Og/Gorilla/mkGorillaWindow.h>

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR > 0
namespace Ogre
{
	typedef v1::ManualObject ManualObject;
}
#endif

namespace mk
{
	class MK_OG_EXPORT SpaceSheet : public RootSheet
	{
	public:
		SpaceSheet(SpaceViewport& viewport);
		~SpaceSheet();

		FrameType frameType() { return LAYER3D; }

		void nextFrame(size_t tick, size_t delta);

		GorillaSpaceTarget& spaceTarget() { return mTarget; }

		void updateSize();

		void transformCoordinates(float& x, float& y);

		InputReceiver* propagateMouse(float x, float y);

		Widget* pinpoint(float x, float y, bool modal = false);

		static StyleType& cls() { static StyleType ty(RootSheet::cls()); return ty; }

	private:
		SpaceViewport& mViewport;
		GorillaSpaceTarget& mTarget;
		Ogre::Camera* mOgreCamera;
		Ogre::SceneNode* mSceneNode;
		Ogre::ManualObject* mDummyRect;
		GorillaWindow* mGorillaWindow;
		
		float mMouseX;
		float mMouseY;
	};
}

#endif // MK_SPACESHEET_H_INCLUDED