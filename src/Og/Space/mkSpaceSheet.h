#ifndef MK_SPACESHEET_H_INCLUDED
#define MK_SPACESHEET_H_INCLUDED

/* mk Headers */
#include <Og/mkOgForward.h>
#include <Ui/Form/mkForm.h>
#include <Ui/Widget/mkRootSheet.h>
#include <Og/Gorilla/mkGorillaWindow.h>

#ifdef GORILLA_V21
namespace Ogre
{
	typedef v1::ManualObject ManualObject;
}
#endif

namespace mk
{
	class MK_OG_EXPORT SpaceSheet : public RootSheet, public Typed<SpaceSheet>, public Styled<SpaceSheet>
	{
	public:
		SpaceSheet(SpaceViewport* viewport, Form* form);
		~SpaceSheet();

		void build();

		FrameType frameType() { return LAYER3D; }
		InkTarget* inkTarget() { return mTarget; }

		void nextFrame(size_t tick, size_t delta);

		GorillaSpaceTarget* spaceTarget() { return mTarget; }

		void updateSize();

		void transformCoordinates(float& x, float& y);

		InputReceiver* propagateMouse(float x, float y);

		Widget* pinpoint(float x, float y, bool modal = false);

		using Typed<SpaceSheet>::cls;
		using Styled<SpaceSheet>::styleCls;

	private:
		SpaceViewport* mViewport;
		GorillaSpaceTarget* mTarget;
		Ogre::Camera* mOgreCamera;
		Ogre::SceneNode* mSceneNode;
		Ogre::ManualObject* mDummyRect;
		GorillaWindow* mGorillaWindow;
		
		float mMouseX;
		float mMouseY;
	};

	class MK_OG_EXPORT SpaceForm : public Form
	{
	public:
		SpaceForm(SpaceViewport* viewport);
	};
}

#endif // MK_SPACESHEET_H_INCLUDED