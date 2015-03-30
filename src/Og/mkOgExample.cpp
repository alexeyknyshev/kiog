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

#include <float.h>

#include <OgreManualObject.h>

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

	void createOgTestViewport(Form* root)
	{
		Ogre::Camera* camera = prepareOgreScene();
		Ogre::SceneManager* sceneManager = camera->getParentSceneNode()->getCreator();

		Ogre::SceneNode* cubeNode = sceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_DYNAMIC);
		cubeNode->setPosition(0.f, 0.f, 0.f);
		cubeNode->attachObject(createCubeMesh(sceneManager));

		Form* viewport = root->makeappend<Form>(nullptr, "", [root, camera](){ return make_unique<SpaceViewport>(camera); });
	}

	void createOgTest3DSheet(Form* root)
	{
		Ogre::Camera* camera = prepareOgreScene();

		Form* viewport = root->makeappend<Form>(nullptr, "", [root, camera](){ return make_unique<SpaceViewport>(camera); });

		SpaceForm* sheet3D = viewport->makeappend<SpaceForm>(viewport->widget()->as<SpaceViewport>());
		sheet3D->widget()->frame()->setSize(480.f, 350.f);

		Window* window = createUiTestWindow(sheet3D);
		window->widget()->as<WWindow>()->toggleMovable();
		//createUiTestMyGuiImageSkin(sheet3D);
	}

	void pickSample(Form* root, const string& name)
	{
		root->clear();

		if(name == "Viewport")
			createOgTestViewport(root);
		else if(name == "3D Sheet")
			createOgTest3DSheet(root);
		else if(name == "Dockspace")
			createUiTestDockspace(root);
		else if(name == "Window")
			createUiTestWindow(root);
		else if(name == "Tabs")
			createUiTestTabs(root);
		else if(name == "Table")
			createUiTestTable(root);
		else if(name == "Tree")
			createUiTestTree(root);
		else if(name == "Controls")
			createUiTestControls(root);
		else if(name == "Skinned Window (Cegui)")
			createUiTestCeguiImageSkin(root);
		else if(name == "Skinned Window (MyGui)")
			createUiTestMyGuiImageSkin(root);
		else if(name == "File Browser")
			createUiTestFileBrowser(root);
		else if(name == "File Tree")
			createUiTestFileTree(root);
		else if(name == "Progress Dialog")
			createUiTestProgressDialog(root);
	}

	void createOgTestUi(Form* root)
	{
		WOgViewport::styleCls()->layout()->d_sizing = DimSizing(EXPAND, EXPAND);
		SpaceViewport::styleCls()->layout()->d_sizing = DimSizing(EXPAND, EXPAND);
		SpaceViewport::styleCls()->layout()->d_opacity = _OPAQUE;
		SpaceSheet::styleCls()->layout()->d_sizing = DimSizing(FIXED, FIXED);
		SpaceSheet::styleCls()->layout()->d_opacity = _OPAQUE;

		Form* demoheader = root->makeappend<Header>();
		Form* demobody = root->makeappend<PartitionX>();
		demoheader->makeappend<Label>("Pick a demo sample : ");
#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR > 0
		StringVector samples({ "Dockspace", "Window", "Skinned Window (MyGui)", "Tabs", "Table", "Tree", "Controls", "File Browser", "File Tree", "Progress Dialog" });
#else
		StringVector samples({ "Dockspace", "Window", "Skinned Window (MyGui)", "Tabs", "Table", "Tree", "Controls", "File Browser", "File Tree", "Progress Dialog", "Viewport", "3D Sheet" });		
#endif
		demoheader->makeappend<Dropdown>(std::bind(&pickSample, demobody, std::placeholders::_1), samples);
	}
}
