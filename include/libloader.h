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

