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
#include "polylists2triangles.h"
#include <dae.h>
#include <dae/daeDatabase.h>


bool Polylist2Triangles::init()
{
	return true;
}

int Polylist2Triangles::execute()
{
	int error = 0;

	std::string doc = getInput( 0 );

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry", doc.c_str() ));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", doc.c_str() );

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh == 0) continue;

		//if(verbose)	cerr<<"There are "<<polylistElementCount<<" polylist elements in this file\n"; 
		for(size_t currentPolylist = 0; currentPolylist < thisMesh->getPolylist_array().getCount(); currentPolylist++)
		{
			// Get the polylist out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolylist *thisPolylist = thisMesh->getPolylist_array()[currentPolylist];  
			createTrianglesFromPolylist( thisMesh, thisPolylist );
			// Remove the polylist from the mesh
			thisMesh->removeChildElement(thisPolylist);
		}
		while (thisMesh->getPolylist_array().getCount() > 0)
		{
			domPolylist *thisPolylist = thisMesh->getPolylist_array()[0];  
			thisMesh->removeChildElement(thisPolylist);
		}
	}
	return 0;
}

void Polylist2Triangles::createTrianglesFromPolylist( domMesh *thisMesh, domPolylist *thisPolylist )
{
	// Create a new <triangles> inside the mesh that has the same material as the <polylist>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	//thisTriangles->setCount( 0 );
	unsigned int triangles = 0;
	thisTriangles->setMaterial(thisPolylist->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <_dae> and <parameters> as the old <polylist>
	for(int i=0; i<(int)(thisPolylist->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolylist->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolylist->getInput_array()) + 1;
	int numberOfPrimitives = (int)(thisPolylist->getVcount()->getValue().getCount());

	unsigned int offset = 0;

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{	
		int triangleCount = (int)thisPolylist->getVcount()->getValue()[j] -2;
		// Write out the primitives as triangles, just fan using the first element as the base
		int idx = numberOfInputs;
		for(int k = 0; k < triangleCount; k++)
		{
			// First vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + l]);
			}
			// Second vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			// Third vertex
			idx += numberOfInputs;
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			//thisTriangles->setCount(thisTriangles->getCount()+1);
			triangles++;
		}
		offset += (unsigned int)thisPolylist->getVcount()->getValue()[j] * numberOfInputs;
	}
	thisTriangles->setCount( triangles );
}

#if 0
void Polylist2Triangles::createTrianglesFromPolygons( domMesh *thisMesh, domPolygons *thisPolygons )
{
	// Create a new <triangles> inside the mesh that has the same material as the <polygons>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	thisTriangles->setCount( 0 );
	thisTriangles->setMaterial(thisPolygons->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <_dae> and <parameters> as the old <polygons>
	for(int i=0; i<(int)(thisPolygons->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolygons->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolygons->getInput_array()) +1;
	int numberOfPrimitives = (int)(thisPolygons->getP_array().getCount());

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{
		// Check the polygons for consistancy (some exported files have had the wrong number of indices)
		domP * thisPrimitive = thisPolygons->getP_array()[j];
		int elementCount = (int)(thisPrimitive->getValue().getCount());
		if((elementCount%numberOfInputs) != 0)
		{
			//cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
		}
		else
		{
			int triangleCount = (elementCount/numberOfInputs)-2;
			// Write out the primitives as triangles, just fan using the first element as the base
			int idx = numberOfInputs;
			for(int k = 0; k < triangleCount; k++)
			{
				// First vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Second vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				// Third vertex
				idx += numberOfInputs;
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				thisTriangles->setCount(thisTriangles->getCount()+1);
			}
		}
	}
}
#endif

unsigned int Polylist2Triangles::getMaxOffset( domInputLocalOffset_Array &input_array )
{
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

Conditioner::Register< Polylist2TrianglesProxy > polylist2TrianglesProxy;

