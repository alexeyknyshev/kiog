//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/mkOgTypes.h>
#include <Ui/mkUiTypes.h>

#include <Ui/mkUi.h>

#include <Og/mkOgModule.h>

int main(int argc, char *argv[])
{
	std::unique_ptr<mk::OgModule> ogModule = std::make_unique<mk::OgModule>();
	ogModule->initAuto();

	mk::OgWindow* ogWindow = ogModule->createWindow("kiog v0.1", 1200, 800, false);

	mk::UiWindow* uiwindow = ogWindow->uiWindow();
	mk::Form* root = uiwindow->rootForm();
	createTestWindow(root);

	bool pursue = true;
	while(pursue)
		pursue = ogModule->nextFrame();
}