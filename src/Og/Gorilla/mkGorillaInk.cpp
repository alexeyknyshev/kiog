//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Gorilla/mkGorillaInk.h>

#include <Object/Util/mkColour.h>
#include <Ui/Frame/mkFrame.h>
#include <Ui/Frame/mkStripe.h>
#include <Ui/Widget/mkWidget.h>

#include <Og/Gorilla/mkGorillaWindow.h>

#include <OgreColourValue.h>

#include <iostream>

namespace mk
{
	GorillaImage::GorillaImage(Frame* frame, GorillaLayer* layer)
		: mFrame(frame)
	{
		ImageSkin& skin = mFrame->inkstyle()->mImageSkin;

		mTop = createImage(layer, skin.d_top);
		mRight = createImage(layer, skin.d_right);
		mBottom = createImage(layer, skin.d_bottom);
		mLeft = createImage(layer, skin.d_left);

		mTopLeft = createImage(layer, skin.d_topLeft);
		mTopRight = createImage(layer, skin.d_topRight);
		mBottomRight = createImage(layer, skin.d_bottomRight);
		mBottomLeft = createImage(layer, skin.d_bottomLeft);

		mFill = createImage(layer, skin.d_fill);

		if(!skin.d_size)
		{
			skin.d_topIn = mTop->height();
			skin.d_bottomIn = mBottom->height();
			skin.d_leftIn = mLeft->width();
			skin.d_rightIn = mRight->width();

			skin.d_topOut = mTopLeft->height() - mTop->height();
			skin.d_leftOut = mTopLeft->width() - mLeft->width();
			skin.d_bottomOut = mBottomRight->height() - mBottom->height();
			skin.d_rightOut = mBottomRight->width() - mRight->width();
		}
	}

	Gorilla::Rectangle* GorillaImage::createImage(GorillaLayer* layer, const string& image)
	{
		Gorilla::Sprite* sprite = layer->layer()->_getSprite(image);
		float width = sprite->spriteWidth;
		float height = sprite->spriteHeight;

		Gorilla::Rectangle* rect = layer->layer()->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), width, height);
		rect->background_image(image);

		return rect;
	}

	void GorillaImage::updateFrame()
	{
		FrameSkin fskin(mFrame, &mFrame->inkstyle()->mImageSkin);

		// Borders
		mTop->position(fskin.d_inleft, fskin.d_top);
		mTop->width(fskin.d_inwidth);

		mRight->position(fskin.d_inright, fskin.d_intop);
		mRight->height(fskin.d_inheight);

		mBottom->position(fskin.d_inleft, fskin.d_inbottom);
		mBottom->width(fskin.d_inwidth);

		mLeft->position(fskin.d_left, fskin.d_intop);
		mLeft->height(fskin.d_inheight);

		// Corners
		mTopLeft->position(fskin.d_outleft, fskin.d_outtop);
		mTopRight->position(fskin.d_inright, fskin.d_outtop);
		mBottomRight->position(fskin.d_inright, fskin.d_inbottom);
		mBottomLeft->position(fskin.d_outleft, fskin.d_inbottom);

		// Fill
		mFill->position(fskin.d_inleft, fskin.d_intop);
		mFill->width(fskin.d_inwidth);
		mFill->height(fskin.d_inheight);
	}

	Ogre::ColourValue ogreColour(const Colour& colour)
	{
		return Ogre::ColourValue(colour.r(), colour.g(), colour.b(), colour.a());
	}

	GorillaInk::GorillaInk(Frame* frame, GorillaLayer* layer)
		: Inkbox(frame)
		, mLayer(layer)
		, mRect(nullptr)
		, mImage(nullptr)
		, mCaption(nullptr)
		, mImageSkin(nullptr)
	{}

	GorillaInk::~GorillaInk()
	{
		this->hide();
	}

	void GorillaInk::show()
	{
		if(!mFrame->inkstyle()->mEmpty)
		{
			mRect = mLayer->layer()->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y));

			if(!mFrame->inkstyle()->mImageSkin.null())
				mImageSkin = make_unique<GorillaImage>(mFrame, mLayer);
		}

		mVisible = true;

		this->updateStyle();
		this->updateContent();
	}

	void GorillaInk::hide()
	{
		if(mRect)
		{
			mLayer->layer()->destroyRectangle(mRect);
			mRect = nullptr;
		}
		if(mImage)
		{
			mLayer->layer()->destroyRectangle(mImage);
			mImage = nullptr;
		}
		if(mCaption)
		{
			mLayer->layer()->destroyCaption(mCaption);
			mCaption = nullptr;
		}

		mVisible = false;
	}

	float GorillaInk::contentSize(Dimension dim)
	{
		if(mImage)
		{
			Gorilla::Sprite* sprite = mLayer->layer()->_getSprite(mFrame->widget()->image());
			return dim == DIM_X ? sprite->spriteWidth : sprite->spriteHeight;
		}
		if(mCaption)
		{
			Ogre::Vector2 size;
			mCaption->_calculateDrawSize(size);
			float xoffset = mFrame->inkstyle()->mPadding[DIM_X] * 2.f;
			float yoffset = mFrame->inkstyle()->mPadding[DIM_Y] * 2.f;

			return dim == DIM_X ? size.x + xoffset : size.y + yoffset;
		}
		
		return 0.f;
	}

	void GorillaInk::updateContent()
	{
		this->updateImage();
		this->updateCaption();
	}

	void GorillaInk::updateStyle()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mFrame->inkstyle()->mBackgroundColour.a() == 0.f)
			mRect->no_background();
		else
			mRect->background_colour(ogreColour(mFrame->inkstyle()->mBackgroundColour));

		if(mFrame->inkstyle()->mBorderWidth.x())
			mRect->border(mFrame->inkstyle()->mBorderWidth.x(), ogreColour(mFrame->inkstyle()->mBorderColour));

		if(!mFrame->inkstyle()->mImageSkin.null() && !mImageSkin /* @kludge because we are updating Style way more than we should : that's the problem */) 
			mImageSkin = make_unique<GorillaImage>(mFrame, mLayer);

		// @todo add gradients
	}

	void GorillaInk::updateFrame()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible || mFrame->dsize(DIM_X) == 0.f || mFrame->dsize(DIM_X) == 0.f)
			return;

		if(mCaption)
		{
			mCaption->left(mFrame->dabsolute(DIM_X) + mFrame->inkstyle()->mPadding[DIM_X]);
			mCaption->top(mFrame->dabsolute(DIM_Y) + mFrame->inkstyle()->mPadding[DIM_Y]);

			mCaption->size(mFrame->dclipsize(DIM_X), mFrame->dclipsize(DIM_Y));
		}
		
		float left = mFrame->dabsolute(DIM_X) + mFrame->dclippos(DIM_X) + mFrame->inkstyle()->mMargin[DIM_X] + mFrame->inkstyle()->mBorderWidth.x();
		float top = mFrame->dabsolute(DIM_Y) + mFrame->dclippos(DIM_Y) + mFrame->inkstyle()->mMargin[DIM_Y] + mFrame->inkstyle()->mBorderWidth.x();

		float width = mFrame->dclipsize(DIM_X) - mFrame->inkstyle()->mMargin[DIM_X] - mFrame->inkstyle()->mMargin[DIM_X + 2] - mFrame->inkstyle()->mBorderWidth[DIM_X] - mFrame->inkstyle()->mBorderWidth[DIM_X + 2];
		float height = mFrame->dclipsize(DIM_Y) - mFrame->inkstyle()->mMargin[DIM_Y] - mFrame->inkstyle()->mMargin[DIM_Y + 2] - mFrame->inkstyle()->mBorderWidth[DIM_Y] - mFrame->inkstyle()->mBorderWidth[DIM_Y + 2];

		mRect->left(left);
		mRect->top(top);

		mRect->width(width);
		mRect->height(height);

		if(mImage)
		{
			mImage->left(left);
			mImage->top(top);
			mImage->width(width);
			mImage->height(height);
		}

		if(mImageSkin)
			mImageSkin->updateFrame();

		//if(mFrame->style()->name() != "cursor")
		//	std::cerr << "GorillaInk :: draw " << mFrame->style()->name() << " at " << mRect->left() << " , " << mRect->top() << " size " << mRect->width() << " , " << mRect->height() << std::endl;
	}

	void GorillaInk::updateImage()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mImage && mFrame->widget()->image().empty())
		{
			mLayer->layer()->destroyRectangle(mImage);
			mImage = nullptr;
			return;
		}
		else if(!mImage && !mFrame->widget()->image().empty())
		{
			mImage = mLayer->layer()->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y));
		}

		if(!mFrame->widget()->image().empty())
		{
			mImage->background_image(Ogre::String(mFrame->widget()->image()));
		}
	}

	void GorillaInk::updateCaption()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mCaption && mFrame->widget()->label().empty())
		{
			mLayer->layer()->destroyCaption(mCaption);
			mCaption = nullptr;
			return;
		}
		else if(!mCaption && !mFrame->widget()->label().empty())
		{
			mCaption = mLayer->layer()->createCaption(9, mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->widget()->label());

			//std::cerr << "Caption at : " << mFrame->dabsolute(DIM_X) << " , " << mFrame->dabsolute(DIM_Y) << std::endl;
		}

		if(!mFrame->widget()->label().empty())
		{
			mCaption->text(mFrame->widget()->label());
			mCaption->size(mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y));
			mCaption->align(Gorilla::TextAlign_Left);
			mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
			mCaption->no_background();
			mCaption->colour(ogreColour(mFrame->inkstyle()->mTextColour));
		}
	}
}
