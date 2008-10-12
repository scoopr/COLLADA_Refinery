/*
The MIT License

Copyright 2006 Sony Computer Entertainment Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */
#include "imageConv.h"

#include <IL/il.h>

bool Imageconv::init()
{
	addStringOption( "ext", "extension", "image file extension", "" );
	return true;
}

int Imageconv::execute()
{
	int error = 0;

	ILboolean ret;
	ILuint image;
	ilInit();
	ilGenImages(1, &image);
	ilBindImage(image);

	std::string extension;
	getStringOption( "ext", extension );

	// How many geometry elements are there?
	int imageElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "image", getInput(0).c_str()));

	for(int currentImg = 0; currentImg < imageElementCount; currentImg++)
	{
		// Find the next geometry element
		domImage *thisImage;
		error = _dae->getDatabase()->getElement((daeElement**)&thisImage,currentImg, NULL, "image", getInput(0).c_str());

		/*thisImage->getInit_from()->getValue().validate();
		daeString origPath = thisImage->getInit_from()->getValue().getURI();
		daeString origIF = thisImage->getInit_from()->getValue().getOriginalURI();

		//make current image element point to new image
		char *origExt = const_cast<char*>(strrchr( origIF, '.' )); //look for the extension
		if ( origExt == NULL )
		{
			continue;
		}
		origExt++;
		unsigned int origExtLen = (unsigned int)strlen( origExt );
		std::string extension;
		getStringOption( "ext", extension );
		char ext[32];
		strcpy(ext, extension.c_str());
		unsigned int extLen = (unsigned int)strlen( ext );

		char *newPath = new char[ strlen( origPath ) + 1 + ( extLen - origExtLen ) ];
		strncpy( newPath, origPath, strlen( origPath ) + ( extLen - origExtLen ) );
		origExt = strrchr( newPath, '.' );
		origExt++;
		strcpy( origExt, ext );


		FILE *file = fopen( newPath + 8, "r" ); //accomodate for file:///
		if ( file != NULL ) 
		{
			//file exists don't recreate it.
			fclose( file );
			delete []newPath;
			continue;
		}

		char *newIF = new char[ strlen( origIF ) + 1 + ( extLen - origExtLen ) ];
		strncpy( newIF, origIF, strlen( origIF ) + ( extLen - origExtLen ) );
		origExt = strrchr( newIF, '.' );
		origExt++;
		strcpy( origExt, ext );

		thisImage->getInit_from()->getValue().setURI( newIF );*/

		daeURI &origURI = thisImage->getInit_from()->getValue();
		origURI.validate();
		if ( strcmp( origURI.getProtocol(), "file" ) != 0 )
			continue;

		char *origPath = new char[ strlen( origURI.getFilepath() ) + strlen( origURI.getFile() ) +1 ];

		strcpy( origPath, origURI.getFilepath() );
		strcat( origPath, origURI.getFile() );
		//convert image using dev il
		ret = ilLoadImage((const ILstring)origPath+1);
		
		if (ret == IL_FALSE || ilIsImage(image) == IL_FALSE )
		{
			delete[] origPath;
			continue;
		}

		ILenum type;
		if (strcmp(extension.c_str(), "bmp") == 0) 
			type = IL_BMP;
		else if ((strcmp(extension.c_str(), "jpg") == 0) || (strcmp(extension.c_str(), "jpeg") == 0))
			type = IL_JPG;
		else if (strcmp(extension.c_str(), "png") == 0) 
			type = IL_PNG;
		else if (strcmp(extension.c_str(), "tga") == 0)
			type = IL_TGA;
		else if ((strcmp(extension.c_str(), "tif") == 0) || (strcmp(extension.c_str(), "tiff") == 0))
			type = IL_TIF;
		else if (strcmp(extension.c_str(), "raw") == 0)
			type = IL_RAW;
		else if (strcmp(extension.c_str(), "dds") == 0) 
			type = IL_DDS;

		char *newPath = new char[ strlen( origPath ) + extension.size() - strlen( origURI.getExtension() ) + 1 ];
		strcpy( newPath, origPath );
		char *newExt = strrchr( newPath, '.' );
		newExt++;
		strcpy( newExt, extension.c_str() );

		FILE *file = fopen( newPath+1, "r" );
		if ( file != NULL ) 
		{
			//file exists don't recreate it.
			fclose( file );
			delete[] origPath;
			delete[] newPath;
			continue;
		}

        ret = ilSave(type, newPath+1);
		if (ret == IL_FALSE) {
			delete[] origPath;
			delete[] newPath;
			continue;
		}
		
		thisImage->getInit_from()->setValue( newPath );
		delete[] origPath;
		delete[] newPath;
	}

	ilShutDown();
	return 0;
}

Conditioner::Register< ImageconvProxy > imageconvProxy;
