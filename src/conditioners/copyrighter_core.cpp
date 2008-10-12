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
#include "copyrighter.h"
#include "dom/domConstants.h"

bool Copyrighter::init()
{
	addStringOption( "copyright", "Copyright Options", "This is copyright text.", "");
	addBoolOption( "overwrite", "Overwrite Options", "This is overwrite option.", false );
	addStringOption( "file", "Copyright file Options", "This is the file to copyright", "", true);
	return true;
}

int Copyrighter::execute()
{
	int error;

	bool overwrite;
	getBoolOption( "overwrite", overwrite );
	std::string copyright;
	getStringOption( "copyright", copyright );
	std::string copyright_file;
	getStringOption( "file", copyright_file);
	if (copyright_file != std::string("")) {

		FILE * pFile;
		long lSize;
		char * buffer;
		pFile = fopen ( copyright_file.c_str() , "rb" );
		if (pFile==NULL)
		{
			char temp[1024];
			sprintf(temp, "File %s is not found\n", copyright_file.c_str()); 
			printDebugMessage(temp);
			return (-1);
		}

		// obtain file size.
		fseek (pFile , 0 , SEEK_END);
		lSize = ftell (pFile);
		rewind (pFile);

		// allocate memory to contain the whole file.
		buffer = (char*) malloc (lSize + 1);
		if (buffer == NULL) 
		{
			char temp[1024];
			sprintf(temp, "File %s is too big, not enough memory\n", copyright_file.c_str()); 
			printDebugMessage(temp);
			return (-2);
		}
		buffer[lSize] = '\0';
		// copy the file into the buffer.
		fread (buffer,1,lSize,pFile);

		copyright = buffer;

		// terminate
		fclose (pFile);
		free (buffer);
	}


	unsigned int assetCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_ASSET, getInput(0).c_str());
	domAsset * thisAsset;
	for(unsigned int i=0; i<assetCount; i++)
	{
		// Get the next asset
		error = _dae->getDatabase()->getElement((daeElement**)&thisAsset,i, NULL, COLLADA_TYPE_ASSET, getInput(0).c_str());
		if(error != DAE_OK)
			continue;

		domAsset::domContributor_Array & contributorArray = thisAsset->getContributor_array();
		unsigned int contributorCount = (int) contributorArray.getCount();

		if (contributorCount == 0){
			// need to add contributor and copyright
			domAsset::domContributor *newContributor;
			newContributor = (domAsset::domContributor *)thisAsset->createAndPlace(COLLADA_TYPE_CONTRIBUTOR);
			domAsset::domContributor::domCopyright *newCopyright;
			newCopyright = (domAsset::domContributor::domCopyright *)newContributor->createAndPlace(COLLADA_TYPE_COPYRIGHT);
			newCopyright->setValue(copyright.c_str());			
		} else {
			// find the copyright
			for (unsigned int j=0; j< contributorCount; j++)
			{
				domAsset::domContributor::domCopyright * pCopyright;
				pCopyright = contributorArray[j]->getCopyright();
				if (pCopyright == 0) {
					// need to add copright
					domAsset::domContributor::domCopyright *newCopyright;
					newCopyright = (domAsset::domContributor::domCopyright *)contributorArray[j]->createAndPlace(COLLADA_TYPE_COPYRIGHT);
					newCopyright->setValue(copyright.c_str());
				} else {
					// may overwrite the copyright value
                    if (overwrite) 
						pCopyright->setValue(copyright.c_str());
				}
			} //for
		} //else
	} //for
	return 0;
}

Conditioner::Register< CopyrighterProxy > copyrighterProxy;
