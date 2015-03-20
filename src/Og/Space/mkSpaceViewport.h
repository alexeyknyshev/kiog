#ifndef MK_SPACEVIEWPORT_H_INCLUDED
#define MK_SPACEVIEWPORT_H_INCLUDED

/* mk Headers */
#include <Og/mkOgForward.h>
#include <Og/Widget/mkWOgViewport.h>
#include <Ui/Controller/mkController.h>

namespace mk
{
	class MK_OG_EXPORT SpaceViewport : public WOgViewport
	{
	public:
		SpaceViewport(Ogre::Camera* camera);

		void leftPicked(Object* object, bool shift, float x, float y) {}
		void rightPicked(Object* object, bool shift, float x, float y) {}

		InputReceiver* propagateMouse(float x, float y);

		bool mouseMoved(float xPos, float yPos, float xDif, float yDif) { UNUSED(xPos); UNUSED(yPos); UNUSED(xDif); UNUSED(yDif); return false; };
		bool mouseWheel(float xPos, float yPos, float amount) { UNUSED(xPos); UNUSED(yPos); UNUSED(amount); return false; };

		bool mousePressed(float xPos, float yPos, MouseButton button) { UNUSED(xPos); UNUSED(yPos); UNUSED(button); return false; };
		bool mouseReleased(float xPos, float yPos, MouseButton button) { UNUSED(xPos); UNUSED(yPos); UNUSED(button); return false; };

	protected:
		float mMouseX;
		float mMouseY;
	};
}

#endif // MK_SPACEVIEWPORT_H_INCLUDED
