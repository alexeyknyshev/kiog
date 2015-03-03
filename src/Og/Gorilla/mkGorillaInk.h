//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_GORILLAINK_H
#define MK_GORILLAINK_H

/* mk */
#include <Og/mkOgForward.h>
#include <Ui/Frame/mkInk.h>

/* Gorilla */
#include <Og/Gorilla/Gorilla.h>

namespace mk
{
	class GorillaImage
	{
	public:
		GorillaImage(Frame* frame, GorillaLayer* layer);

		void updateFrame();

		Gorilla::Rectangle* createImage(GorillaLayer* layer, const string& image);

	protected:
		Frame* mFrame;
		Gorilla::Rectangle* mTop; Gorilla::Rectangle* mRight; Gorilla::Rectangle* mBottom; Gorilla::Rectangle* mLeft;
		Gorilla::Rectangle* mTopLeft; Gorilla::Rectangle* mTopRight; Gorilla::Rectangle* mBottomRight; Gorilla::Rectangle* mBottomLeft;
		Gorilla::Rectangle* mFill;
	};

	class GorillaInk : public Inkbox
	{
	public:
		GorillaInk(Frame* frame, GorillaLayer* layer);
		~GorillaInk();
    
		void updateContent();
		void updateFrame();
		void updateStyle();

		void show();
		void hide();

		string image();

		float contentSize(Dimension dim);

		void updateImage();
		void updateCaption();

	protected:
		GorillaLayer* mLayer;
		Gorilla::Rectangle* mRect;
		Gorilla::Rectangle* mImage; // Optional
		Gorilla::Caption* mCaption; // Optional
		unique_ptr<GorillaImage> mImageSkin;
		bool mVisible;
	};

}

#endif
