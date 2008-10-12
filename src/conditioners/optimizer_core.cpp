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
#include "optimizer.h"

bool Optimizer::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	addFloatOption( "c", "cache_size", "cache_size", 10);
	return true;
}

int Optimizer::execute()
{
	float cache_size;
	bool verbose = false;
	getBoolOption("v", verbose);
	getFloatOption("c", cache_size);

	int error = 0;

	// How many geometry elements are there?
	unsigned int geometryElementCount = _dae->getDatabase()->getElementCount(NULL, "geometry", getInput(0).c_str());
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	// Iterate over all the geometry elements, each one is optimized individually
	for(unsigned int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Get the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", getInput(0).c_str());
		
		// Get the geometry element's mesh
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh==NULL) continue; // if there is no mesh in geometry, skip to the next one

		// Loop over all the triangle elements in this mesh
		int trianglesElementCount = (int)(thisMesh->getTriangles_array().getCount());
//		if(verbose)	cerr<<"There are "<<trianglesElementCount<<" triangle elements in this geometry\n";
		for(int currentTriangles = 0; currentTriangles < trianglesElementCount; currentTriangles++)
		{
			// Get the next triangles out of the mesh
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(currentTriangles);  
			
			// Get count of inputs and primitives
			// Remember that for triangles a <p> tag can contain multiple triangles
			
			// find the max input offset, 0 start
			unsigned int  inputCount			= (unsigned int)(thisTriangles->getInput_array().getCount());
			unsigned int  maxinputoffset		= 0;
			for (unsigned int i=0; i < inputCount; i++)
			{
				unsigned int thisinputoffset = (unsigned int ) thisTriangles->getInput_array()[i]->getOffset();
				if (maxinputoffset < thisinputoffset) maxinputoffset = thisinputoffset;
			}
			maxinputoffset++; // make it 1 start

			unsigned int  numberOfPrimitives	= (unsigned int )(thisTriangles->getCount());

			// Make a new vertex cache
			vertexCache beforeCache((int) cache_size);
			unsigned int  totalVertices = 0;
			list<simpleTriangle> inputTriangles;
			list<simpleTriangle> outputTriangles;

			// Iterate over all the primitives and save them in "inputTriangles"
			for(unsigned int  currentPrimitive = 0; currentPrimitive < numberOfPrimitives; currentPrimitive++)
			{

				unsigned int  currentTriangleIndex = currentPrimitive * 3 * maxinputoffset;
				// Simulate operation of a vertex cache, the file has already been deindexed
				// so we only look at the first index of each vertex, all the others will be the same
				simpleTriangle aTriangle;
				for(unsigned int  currentIndex = 0; currentIndex < 3; currentIndex++)
				{
					domPRef P = thisTriangles->getP();
					domListOfUInts Plist = P->getValue();
//					unsigned int  count = (unsigned int ) Plist.getCount();
					unsigned int  Pindex = currentTriangleIndex + currentIndex * maxinputoffset;
					unsigned int  Pvalue = (unsigned int ) Plist[Pindex];
					aTriangle.indices[currentIndex] = Pvalue;
					beforeCache.addVertex(Pvalue);
//					aTriangle.indices[currentIndex] = thisTriangles->getP()->getValue()[currentTriangleIndex + currentIndex*inputCount];
//					beforeCache.addVertex(thisTriangles->getP()->getValue()[currentTriangleIndex + currentIndex*inputCount]);
					totalVertices++;
				}
				inputTriangles.push_back(aTriangle);
			}
//			if(verbose) cerr<<"Triangles "<<currentTriangles<< " had "<<beforeCache.cacheHits<<" cache hits and "<<beforeCache.cacheMisses<<" misses "<<((float)beforeCache.cacheHits/(float)totalVertices)*100.0<<"% hit rate\n";

			// This is a simple cache optimizer, it finds the triangle with the lowest cache cost and moves to the 
			// output triangles array, updating the cache in the process.
			vertexCache afterCache((int) cache_size);
			// Move the first triangle over to the output array to "seed" the cache
			outputTriangles.push_back(*(inputTriangles.begin()));
			inputTriangles.pop_front();
			afterCache.addTriangle(*(outputTriangles.begin()));
			// Repeatedly scan the input array for the lowest cost triangle and move it
			// Worst case this is an n-squared loop but there are some short cuts for 
			// triangles that are totally in-cache.
			while(inputTriangles.size() > 0)
			{
				int lowestCost = 999;
				int cost;
				list<simpleTriangle>::iterator lowestCostTri;
				// Scan the remaining input triangles to find the lowest cost one
				for(list<simpleTriangle>::iterator tri = inputTriangles.begin(); tri != inputTriangles.end(); )
				{
					cost=afterCache.costTriangle(*tri);
					if(cost < lowestCost)
					{
						if(cost == 0)
						{
							// Triangles with zero cost should be moved to output immediately as they don't change the state of the
							// cache or the cost of any unprocessed triangles.
							afterCache.addTriangle(*tri);
							outputTriangles.push_back(*tri);
							tri = inputTriangles.erase(tri);
							continue;
						}
						lowestCostTri = tri;
						lowestCost = cost;
					}
					tri++;
				}
				// Move the lowest cost triangle over to the output array.
				// If there were multiple triangles of cost 1, it might result in better optimization to find
				// what vertex appears the most and add those triangles first.  This could be done by tracking
				// the most frequently missed vertex and adding that vertex to the cache, on the next pass through
				// this loop all the triangles containing that vertex would have a cost of 0 and be added all at once.
				if(lowestCost != 999)
				{
					afterCache.addTriangle(*lowestCostTri);
					outputTriangles.push_back(*lowestCostTri);
					inputTriangles.erase(lowestCostTri);
					}
								}
//			if(verbose) cerr<<"Triangles "<<currentTriangles<< " had "<<afterCache.cacheHits<<" cache hits and "<<afterCache.cacheMisses<<" misses "<<((float)afterCache.cacheHits/(float)totalVertices)*100.0<<"% hit rate\n";
			// Rewrite the new order to the output file, this updates it in place
			for(unsigned int currentPrimitive = 0; currentPrimitive < numberOfPrimitives; currentPrimitive++)
			{
				unsigned int currentTriangleIndex = currentPrimitive * 3 * maxinputoffset;
				for(unsigned int currentIndex = 0; currentIndex < 3; currentIndex++)
				{
					unsigned int anIndex = (*(outputTriangles.begin())).indices[currentIndex];
					for(unsigned int iCount = 0; iCount < maxinputoffset; iCount++)
					{
						thisTriangles->getP()->getValue()[currentTriangleIndex + ((currentIndex*maxinputoffset)+iCount)] = anIndex;
					}
				}
				outputTriangles.pop_front();
			}
		}
	}
	return 0;
}

Conditioner::Register<OptimizerProxy> opimizerProxy;
