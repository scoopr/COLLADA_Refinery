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
#include "reversewinding.h"
#include <dae.h>
#include <dae/daeDatabase.h>

unsigned int getMaxOffset( domInputLocalOffset_Array &input_array );
/*{
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}
*/

int OnChangeWindingOrder(domTriangles * triangles)
{
	unsigned int max_offset = getMaxOffset(triangles->getInput_array()) + 1;

	domUint count = triangles->getCount();

	domP * p = triangles->getP();
	if (p == NULL) return -1;									// Error check
	domListOfUInts & list_of_uint = p->getValue();
	
	domUint * temp_data = new domUint[max_offset];	// make temp data for temp vertex

	for (int i=0; i<count; i++) // for each triangle			// change winding order from v1,v2,v3 to v3,v2,v1
	{
		int v1_offset = i * 3 * max_offset;
//		int v2_offset = v1_offset + max_offset;					// v2 is unchanged
		int v3_offset = v1_offset + 2 * max_offset;

		for(unsigned int j=0; j<max_offset; j++)							// backup v1 to temp
			temp_data[j] = list_of_uint[v1_offset + j];

		for(unsigned int j=0; j<max_offset; j++)							// copy v3 to v1
			list_of_uint[v1_offset + j] = list_of_uint[v3_offset + j];				

		for(unsigned int j=0; j<max_offset; j++)							// copy temp to v3
			list_of_uint[v3_offset + j] = temp_data[j];				
	}

	return 0;
}

int ChangeWindingOrder(DAE * _dae)
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

		if (thisMesh == NULL) return -1;	// Error Check

		// Loop over all the polygon elements
		for(int i = 0; i < (int)(thisMesh->getTriangles_array().getCount()); i++)
		{
			domTriangles * triangles = thisMesh->getTriangles_array()[i];
			error = OnChangeWindingOrder(triangles);
			if (error != 0) return -1;		// Error check
		}
	}

	return 0;
}


bool ReverseWinding::init()
{
	return true;
}

int ReverseWinding::execute()
{
	int error = 0;

	ChangeWindingOrder(_dae);

	return 0;
}


Conditioner::Register< ReverseWindingProxy > ReverseWindingProxy;

