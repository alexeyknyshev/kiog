//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/mkOgSymbol.h>

#include <Og/Ogre/Shape/mkOgreShape.h>

#include <Object/String/mkString.h>

#include <OgreSceneManager.h>
#include <OgreManualObject.h>

namespace mk
{
	OgSymbol::OgSymbol(Object* object, Ogre::SceneNode* sceneNode)
		: mSceneNode(sceneNode)
		, mManualObject(sceneNode->getCreator()->createManualObject())
		, mShapes()
	{
		sceneNode->attachObject(mManualObject);
		mManualObject->getUserObjectBindings().setUserAny(Ogre::Any(object));

		mManualObject->setDynamic(true);

		mManualObject->begin("debug_draw", Ogre::RenderOperation::OT_LINE_LIST);
		mManualObject->position(Ogre::Vector3::ZERO);
		mManualObject->colour(Ogre::ColourValue::ZERO);
		mManualObject->index(0);
		mManualObject->end();
		mManualObject->begin("debug_draw", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		mManualObject->position(Ogre::Vector3::ZERO);
		mManualObject->colour(Ogre::ColourValue::ZERO);
		mManualObject->textureCoord(Ogre::Vector2::ZERO);
		mManualObject->index(0);
		mManualObject->end();
	}

	OgSymbol::~OgSymbol()
	{
		mSceneNode->getCreator()->destroyManualObject(mManualObject);
	}

	void OgSymbol::setMask(unsigned int mask)
	{
		mManualObject->setQueryFlags(mask);
	}

	void OgSymbol::addShape(ManualShape* shape)
	{
		mShapes.push_back(shape);
		redraw();
	}

	void OgSymbol::removeShape(ManualShape* shape)
	{
		mShapes.erase(std::remove(mShapes.begin(), mShapes.end(), shape), mShapes.end());
		redraw();
	}

	void OgSymbol::redraw()
	{
		int vertexCount = 0;
		int indexCount = 0;
		int lastIndex = 0;

		mManualObject->beginUpdate(0);
		for(auto it = mShapes.begin(); it != mShapes.end(); ++it)
		{
			vertexCount += (*it)->outlineVertexCount();
			indexCount += (*it)->outlineIndexCount();
		}

		mManualObject->estimateVertexCount(vertexCount);
		mManualObject->estimateIndexCount(indexCount);

		for(auto it = mShapes.begin(); it != mShapes.end(); ++it)
		{
			(*it)->drawOutline(mManualObject, lastIndex);
			lastIndex += (*it)->outlineVertexCount();
		}

		mManualObject->end();


		vertexCount = 0;
		indexCount = 0;
		lastIndex = 0;

		mManualObject->beginUpdate(1);
		for(auto it = mShapes.begin(); it != mShapes.end(); ++it)
		{
			vertexCount += (*it)->plainVertexCount();
			indexCount += (*it)->plainIndexCount();
		}

		mManualObject->estimateVertexCount(vertexCount);
		mManualObject->estimateIndexCount(indexCount);

		for(auto it = mShapes.begin(); it != mShapes.end(); ++it)
		{
			(*it)->drawPlain(mManualObject, lastIndex);
			lastIndex += (*it)->plainVertexCount();
		}

		mManualObject->end();
	}
}
