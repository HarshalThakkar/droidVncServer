/*
	 droid vnc server - Android VNC server
	 Copyright (C) 2009 Jose Pereira <onaips@gmail.com>

	 This library is free software; you can redistribute it and/or
	 modify it under the terms of the GNU Lesser General Public
	 License as published by the Free Software Foundation; either
	 version 3 of the License, or (at your option) any later version.

	 This library is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	 Lesser General Public License for more details.

	 You should have received a copy of the GNU Lesser General Public
	 License along with this library; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
	 */

#include "flinger.h"
#include "screenFormat.h"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <binder/IMemory.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>

using namespace android;

ScreenshotClient *screenshotClient=NULL;

extern "C" screenFormat getscreenformat_flinger()
{
  //get format on PixelFormat struct
//	PixelFormat f=screenshotClient->getFormat();

//	PixelFormatInfo pf;
//	getPixelFormatInfo(f,&pf);

	screenFormat format;

	format.bitsPerPixel = 32;//pf.bitsPerPixel;
	format.width = screenshotClient->getWidth();
	format.height =  screenshotClient->getHeight();
	format.size = format.bitsPerPixel*format.width*format.height/CHAR_BIT;
	format.redShift = 0;
	format.redMax = 8;
	format.greenShift = 8;
	format.greenMax = 8;
	format.blueShift = 16;
	format.blueMax = 8;
	format.alphaShift = 24;
	format.alphaMax = 8;

	return format;
}


extern "C" int init_flinger()
{
	int errno;

	//L("--Initializing gingerbread access method--\n");

  screenshotClient = new ScreenshotClient();
  errno = screenshotClient->update(SurfaceComposerClient::getBuiltInDisplay(0), Rect(), true);
  if (!screenshotClient->getPixels())
    return -1;

  if (errno != NO_ERROR) {
		return -1;
	}
	return 0;
}

extern "C" unsigned int *readfb_flinger()
{
	screenshotClient->update(SurfaceComposerClient::getBuiltInDisplay(0), Rect(), true);
	return (unsigned int*)screenshotClient->getPixels();
}

extern "C" void close_flinger()
{
  free(screenshotClient);
}
