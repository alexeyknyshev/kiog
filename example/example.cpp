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
	mk::OgModule ogModule("plugins.cfg", "../../Data/");
	ogModule.initAuto();

	mk::OgWindow& ogWindow = ogModule.createWindow("kiog v0.1", 1200, 800, false);
	ogWindow.uiWindow().init();

	mk::Form& root = ogWindow.uiWindow().rootForm();
	createOgTestUi(root);

	bool pursue = true;
	while(pursue)
		pursue = ogModule.nextFrame();
}