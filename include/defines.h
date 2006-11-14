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
#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef WIN32
	#ifdef REFINERY
		#define REF_EXPORT __declspec(dllexport)
	#else
		#define REF_EXPORT __declspec(dllimport)
	#endif
#endif

#endif //__DEFINES_H__
