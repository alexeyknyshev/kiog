#include <Og/mkOgConfig.h>
#include <Og/Space/mkSpaceViewport.h>

#include <Og/Space/mkSpaceSheet.h>

namespace mk
{
	SpaceViewport::SpaceViewport(UiWindow& uiWindow, Ogre::Camera* camera)
		: WOgViewport(uiWindow, camera)
	{}

	InputReceiver* SpaceViewport::propagateMouse(float x, float y)
	{
		x -= mFrame->dabsolute(DIM_X);
		y -= mFrame->dabsolute(DIM_Y);

		Object* object = pickObject(x, y, SPACESHEET_OGRE_MASK);
		if(!object)
			return mParent;

		return &object->as<SpaceSheet>();
	}
}