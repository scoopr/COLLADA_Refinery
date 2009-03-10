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
#include "filename.h"

#include <dom/domConstants.h>
#include <dom/domFx_include_common.h>

bool Filename::init()
{
	addStringOption( "prefix", "Prefix Option", "This is prefix string.", "" );
	addBoolOption( "image", "Modify Image elements?", "Set to true to modify image elements. False will modify include elements.", false );
	return true;
}

int Filename::execute()
{
	int error;
	std::string prefix;
	getStringOption( "prefix", prefix );
	bool image;
	getBoolOption( "image", image );

	if ( !image )
	{
	    // How many include elements are there?
	    unsigned int includeTagCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_FX_INCLUDE_COMMON, getInput(0).c_str());

	    // Process the URI/filename strings on each include element
	    domFx_include_common *thisInclude;  // Pointer to the include element we are currently working on
	    for(unsigned int i=0; i<includeTagCount; i++)
		{
		    // Get the next include element
		    error = _dae->getDatabase()->getElement((daeElement**)&thisInclude,i, NULL, COLLADA_TYPE_FX_INCLUDE_COMMON, getInput(0).c_str());
		    if(error != DAE_OK)
			{
			   return -1;
			}
		    // If the protocol is something other than "file" don't touch the element
			thisInclude->getUrl().validate();
		    if(strcmp("file", thisInclude->getUrl().getProtocol()) != 0)
			{
			    continue;
			}
//			char *_URIfile = (char *)thisInclude->getUrl().getFile();
			char *_URIfile = (char *)thisInclude->getUrl().getPath();

		    // Build the new relative file name and write it back into the DOM
		    // This uses the filename in domInclude.source as parsed by the API
			std::string finalname = prefix + _URIfile;
		    if(finalname.length() != 0)
			{
			    thisInclude->getUrl().setURI(finalname.c_str());
			}
		    else
			{
				return -1;
			}
		}
	}
    else if (image)
 	{
	    // How many image elements are there?
	    unsigned int imageTagCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_IMAGE, getInput(0).c_str());

	    // Process the URI/filename strings on each image element
		domImage *thisImage;  // Pointer to the image element we are currently working on
	    for(unsigned int i=0; i<imageTagCount; i++)
		{
		    // Get the next image element
		    error = _dae->getDatabase()->getElement((daeElement**)&thisImage,i, NULL, COLLADA_TYPE_IMAGE, getInput(0).c_str());
		    if(error != DAE_OK)
			{
			    return(-1);
			}
		    // If the protocol is something other than "file" don't touch the element
			thisImage->getInit_from()->getValue().validate();
		    if(strcmp("file", thisImage->getInit_from()->getValue().getProtocol()) != 0)
			{
			    continue;
			}
		    // Build the new relative file name and write it back into the DOM
		    // This uses the filename in domImage.source as parsed by the API
//			std::string finalname = prefix + thisImage->getInit_from()->getValue().getFile();
			std::string finalname = prefix + thisImage->getInit_from()->getValue().getPath();

		    if(finalname.length() != 0)
			{
			    thisImage->getInit_from()->getValue().setURI(finalname.c_str());
			}
		    else
			{
			    return(-1);
			}
		}

	}
	return 0;
}

Conditioner::Register< FilenameProxy > filenameProxy;
