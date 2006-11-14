/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
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
