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

#ifndef _LIBLOADER_H_
#define _LIBLOADER_H_

#include "defines.h"

#include <map>

#include <jni.h>

class ConditionerCreator;
class Conditioner;

class LibLoader
{
public:
	REF_EXPORT static void init( JNIEnv *env, jobject obj );
	REF_EXPORT static void cleanup();

	REF_EXPORT static void registerConditioner( const ConditionerCreator *cc );
	REF_EXPORT static void unregisterConditioner( const ConditionerCreator *cc );
	REF_EXPORT static unsigned int getNumAvailableConditioners();
	REF_EXPORT static const ConditionerCreator *getAvailableConditioner( unsigned int idx );
	REF_EXPORT static const ConditionerCreator *getAvailableConditioner( const std::string &key );

	REF_EXPORT static bool addConditioner( const std::string &name, Conditioner *cond );
	REF_EXPORT static Conditioner *getConditioner( const std::string &name );
	REF_EXPORT static bool removeConditioner( const std::string &name );

	REF_EXPORT static void printDebugMessage( const std::string &message );
	REF_EXPORT static void printExecutionMessage( const std::string &message );
	REF_EXPORT static void printErrorMessage( const std::string &message );

	REF_EXPORT static bool loadDocument( const std::string &doc );
	REF_EXPORT static bool saveDocument( const std::string &doc, const std::string &toFile );
	REF_EXPORT static bool cloneDocument( const std::string &from, const std::string &to );

	REF_EXPORT static void clearDOM();

private:
	//maps baseName to conditionerCreator
	static std::map< std::string, const ConditionerCreator* > _availableConditioners;
	//maps specific name to Conditioner instance
	static std::map< std::string, Conditioner* > _conditioners;

	//static JNIEnv *_env;
	//static jobject _obj;
	static JavaVM *_jvm;
	static jint _envVer;

};

#endif

