//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/mkOgTypes.h>
#include <Ui/mkUiTypes.h>

#include <Ui/mkUiLayout.h>
#include <Ui/mkUi.h>

#include <Og/mkOgModule.h>
#include <OgreRoot.h>
#include <OgreManualObject.h>

#include <float.h>

namespace mk
{
	Ogre::ManualObject* createCubeMesh(Ogre::SceneManager* sceneManager, Ogre::String matName = "Gorilla2D21")
	{
		Ogre::ManualObject* cube = sceneManager->createManualObject();
		cube->begin(matName);

		cube->position(0.5f, -0.5f, 1.0f); cube->normal(0.408248f, -0.816497f, 0.408248f); cube->textureCoord(1, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 0.0f); cube->normal(-0.408248f, -0.816497f, -0.408248f); cube->textureCoord(0, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, -0.5f, 0.0f); cube->normal(0.666667f, -0.333333f, -0.666667f); cube->textureCoord(1, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 1.0f); cube->normal(-0.666667f, -0.333333f, 0.666667f); cube->textureCoord(0, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, 0.5f, 1.0f); cube->normal(0.666667f, 0.333333f, 0.666667f); cube->textureCoord(1, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 1.0f); cube->normal(-0.666667f, -0.333333f, 0.666667f); cube->textureCoord(0, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, -0.5f, 1.0f); cube->normal(0.408248f, -0.816497f, 0.408248f); cube->textureCoord(1, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, 0.5f, 1.0f); cube->normal(-0.408248f, 0.816497f, 0.408248f); cube->textureCoord(0, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, 0.5f, 0.0f); cube->normal(-0.666667f, 0.333333f, -0.666667f); cube->textureCoord(0, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 0.0f); cube->normal(-0.408248f, -0.816497f, -0.408248f); cube->textureCoord(1, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 1.0f); cube->normal(-0.666667f, -0.333333f, 0.666667f); cube->textureCoord(1, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, -0.5f, 0.0f); cube->normal(0.666667f, -0.333333f, -0.666667f); cube->textureCoord(0, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, 0.5f, 0.0f); cube->normal(0.408248f, 0.816497f, -0.408248f); cube->textureCoord(1, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, -0.5f, 1.0f); cube->normal(0.408248f, -0.816497f, 0.408248f); cube->textureCoord(0, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, -0.5f, 0.0f); cube->normal(0.666667f, -0.333333f, -0.666667f); cube->textureCoord(1, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, -0.5f, 0.0f); cube->normal(-0.408248f, -0.816497f, -0.408248f); cube->textureCoord(0, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, 0.5f, 1.0f); cube->normal(-0.408248f, 0.816497f, 0.408248f); cube->textureCoord(1, 0); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, 0.5f, 0.0f); cube->normal(0.408248f, 0.816497f, -0.408248f); cube->textureCoord(0, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(-0.5f, 0.5f, 0.0f); cube->normal(-0.666667f, 0.333333f, -0.666667f); cube->textureCoord(1, 1); cube->colour(Ogre::ColourValue::White);
		cube->position(0.5f, 0.5f, 1.0f); cube->normal(0.666667f, 0.333333f, 0.666667f); cube->textureCoord(0, 0); cube->colour(Ogre::ColourValue::White);

		cube->triangle(0, 1, 2);      cube->triangle(3, 1, 0);
		cube->triangle(4, 5, 6);      cube->triangle(4, 7, 5);
		cube->triangle(8, 9, 10);      cube->triangle(10, 7, 8);
		cube->triangle(4, 11, 12);   cube->triangle(4, 13, 11);
		cube->triangle(14, 8, 12);   cube->triangle(14, 15, 8);
		cube->triangle(16, 17, 18);   cube->triangle(16, 19, 17);
		cube->end();

		return cube;
	}

	Ogre::Camera* prepareOgreScene()
	{
		Ogre::Root* ogreRoot = Ogre::Root::getSingletonPtr();
		Ogre::SceneManager* sceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD);
		Ogre::SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_DYNAMIC);
		Ogre::Camera* camera = sceneManager->createCamera("OgCamera");
		camera->detachFromParent();
		camera->setNearClipDistance(0.1f);
		camera->setFarClipDistance(1000.f);

		cameraNode->setPosition(0.f, 0.f, 8.f);
		cameraNode->attachObject(camera);

		return camera;
	}

	void createOgTestViewport(Sheet& sheet)
	{
		Ogre::Camera* camera = prepareOgreScene();
		Ogre::SceneManager* sceneManager = camera->getParentSceneNode()->getCreator();

		Ogre::SceneNode* cubeNode = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_DYNAMIC);
		cubeNode->setPosition(0.f, 0.f, 0.f);
		cubeNode->attachObject(createCubeMesh(sceneManager));

		sheet.emplace<SpaceViewport>(sheet.uiWindow(), camera);
	}

	void createOgTest3DSheet(Sheet& sheet)
	{
		Ogre::Camera* camera = prepareOgreScene();

		SpaceViewport& viewport = sheet.emplace<SpaceViewport>(sheet.uiWindow(), camera);

		SpaceSheet& sheet3D = viewport.emplace<SpaceSheet>(viewport);
		sheet3D.frame().setSize(480.f, 350.f);

		Window& window = createUiTestWindow(sheet3D);
		window.toggleMovable();
	}

	void pickOgSample(Sheet& sheet, Widget& selected)
	{
		const string name = selected.label();
		sheet.clear();

		if(name == "Viewport")
			createOgTestViewport(sheet);
		else if(name == "3D Sheet")
			createOgTest3DSheet(sheet);

		pickUiSample(sheet, selected);
	}

	void createOgTestUi(Form& root)
	{
		//SpaceViewport::cls().layout().d_opacity = OPAQUE;
		//SpaceSheet::cls().layout().d_opacity = OPAQUE;

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR > 0
		StringVector samples({ "Dockspace", "Window", "Text Editor", "Tabs", "Table", "Tree", "Controls", "File Browser", "File Tree", "Progress Dialog" });
#else
		StringVector samples({ "Dockspace", "Window", "Text Editor", "Tabs", "Table", "Tree", "Controls", "File Browser", "File Tree", "Progress Dialog", "Viewport", "3D Sheet" });
#endif

		Header& demoheader = root.sheet().emplace<Header>();
		Board& demobody = root.sheet().emplace<Board>();
		demoheader.emplace<Label>("Pick a demo sample : ");
		demoheader.emplace<Dropdown>(std::bind(&pickOgSample, std::ref(demobody), std::placeholders::_1), samples);
		demoheader.emplace<Label>("Switch theme : ");
		demoheader.emplace<Dropdown>(std::bind(&switchUiTheme, std::ref(demobody), std::placeholders::_1), StringVector({ "Blendish", "Blendish Dark", "TurboBadger", "MyGui", "Photoshop", "Default" }));
	}
}
