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
#include "vertexcache.h"
vertexCache::vertexCache(int size)
{
	cacheSize = size;
	cacheMisses = 0;
	cacheHits = 0;
}
vertexCache::~vertexCache()
{
}
// Figure the cost of a vertex, 1 indicates a cache miss, 0 a hit
int vertexCache::costVertex(int vertexNumber)
{
	if(cacheState.find(vertexNumber) != cacheState.end())
	{
		return(0); // Vertex was already in the cache
	}
	else
	{
		return(1);
	}
}
// Try adding a vertex to the cache, return the cost.  1 indicates there was a cache miss.
int vertexCache::addVertex(int vertexNumber)
{
	if(costVertex(vertexNumber) == 0)
	{
		// Already in the cache
		cacheHits ++;
		return(0);
	}
	else
	{
		// FIFO cache behavior, add new vertex
		cacheState.insert(make_pair(vertexNumber,cacheMisses));
		// Remove oldest vertex if cache is full
		if(cacheState.size() > cacheSize)
		{
			map<int,int>::iterator findme = cacheState.begin();
			map<int,int>::iterator removeme = findme;
			findme++;
			while(findme != cacheState.end())
			{
				if(findme->second < removeme->second)
				{
					removeme = findme;
				}
				findme++;
			}
			cacheState.erase(removeme);
		}
		cacheMisses++;
		return(1);
	}
}
// Add a whole triangle to the cache and return the cost
int vertexCache::addTriangle(simpleTriangle &tri)
{
	return(addVertex(tri.indices[0]) + addVertex(tri.indices[1]) + addVertex(tri.indices[2]));
}
// Figure the cost of adding a triangle to the cache
int vertexCache::costTriangle(simpleTriangle &tri)
{
	return(costVertex(tri.indices[0]) + costVertex(tri.indices[1]) + costVertex(tri.indices[2]));
}
