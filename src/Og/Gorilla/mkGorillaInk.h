//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_GORILLAINK_H
#define MK_GORILLAINK_H

/* mk */
#include <Og/mkOgForward.h>
#include <Ui/Frame/mkInk.h>
#include <Og/Gorilla/Gorilla.h>

namespace mk
{
	class GorillaImage
	{
	public:
		GorillaImage(GorillaInk* inkbox);
		~GorillaImage();

		void prepare();

		void show();
		void hide();

		void updateFrame();
		void updateImage();

		Gorilla::Rectangle* createImage(const string& image);

	protected:
		GorillaInk* mInkbox;
		Frame* mFrame;
		std::vector<Gorilla::Rectangle*> mRects;
	};

	class GorillaInk : public Inkbox
	{
	public:
		GorillaInk(Frame* frame, GorillaLayer* layer);
		~GorillaInk();
    
		GorillaLayer* layer() { return mLayer; }

		void updateContent();
		void updateFrame();
		void updateStyle();

		void show();
		void hide();

		string image();

		float contentSize(Dimension dim);
		size_t caretIndex(float x, float y);
		void caretCoords(size_t index, float& caretX, float& caretY, float& caretHeight);

		void updateImage();
		void updateCaption();

		Gorilla::Rectangle* lastRect();
		Gorilla::Rectangle* createRectangle(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height);

	protected:
		GorillaLayer* mLayer;
		Gorilla::Rectangle* mRect;
		Gorilla::Rectangle* mImage; // Optional
		Gorilla::Caption* mCaption; // Optional
		Gorilla::Rectangle* mLastRect; // Pointer to last created rect
		unique_ptr<GorillaImage> mImageSkin;
	};

}

#endif
