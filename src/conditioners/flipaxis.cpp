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
#include "flipaxis.h"
#include <dae.h>
#include <dae/daeDatabase.h>

enum REFLECT_PLANE
{
	XY_PLANE,
	YZ_PLANE,
	XZ_PLANE
};

enum REFLECT_PLANE reflect_plane;

int reflectaxisOnListOfFloats(domListOfFloats & list_of_floats)
{
	domUint count = list_of_floats.getCount();

	for(int i=0; i<count; i+=3)
	{
		switch(reflect_plane)
		{
		case XY_PLANE:
			list_of_floats[i+2] = -1 * list_of_floats[i+2];
			break;
		case YZ_PLANE:
			list_of_floats[i] = -1 * list_of_floats[i];
			break;
		case XZ_PLANE:
			list_of_floats[i+1] = -1 * list_of_floats[i+1];
			break;
		default:
			break;
		}
	}
	return 0;
}

int reflectaxisOnSource(domSource * source)
{
	domSource::domTechnique_common * technique_common = source->getTechnique_common();
	if (technique_common == NULL) return -1; // Error check

	domAccessor * accessor = technique_common->getAccessor();
	if (accessor == NULL) return -1; //Error check

	domUint stride = accessor->getStride();

	if (stride != 3) return -1; // If stride is not 3, skip this source. eg. TexCoords

	domFloat_array * float_array = source->getFloat_array();

	domListOfFloats & list_of_floats = float_array->getValue();

	reflectaxisOnListOfFloats(list_of_floats);

	return 0;
}

int reflectaxis(DAE * _dae)
{
	int error = 0;

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry" ));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
//		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry");
		daeElement * element = 0;
		error = _dae->getDatabase()->getElement(&element,currentGeometry, NULL, "geometry");
		thisGeometry = (domGeometry *) element;

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh == NULL)
			continue;

		// Loop over all the polygon elements
		for(int currentSource = 0; currentSource < (int)(thisMesh->getSource_array().getCount()); currentSource++)
		{
			domSource *source = thisMesh->getSource_array()[currentSource];  
			reflectaxisOnSource(source);
		}
	}
	return 0;
}


bool FlipAxis::init()
{
	addStringOption( "flipaxis", "flipaxis", "Axis to be flipped", "Z");

	return true;
}

int FlipAxis::execute()
{
	std::string axis;
	getStringOption( "flipaxis", axis );

	if (axis == "Y")
	{
		reflect_plane = XZ_PLANE;
	} else if (axis == "X")
	{
		reflect_plane = YZ_PLANE;
	} else // axis == "Z" // or default
	{
		reflect_plane = XY_PLANE;
	}

	reflectaxis(_dae);

	return 0;
}


Conditioner::Register< FlipAxisProxy > FlipAxisProxy;

