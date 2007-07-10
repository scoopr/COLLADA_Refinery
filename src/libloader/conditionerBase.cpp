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

//#include <windows.h>

#include <dae.h>
#include "conditionerBase.h"

static std::string NULLString = "";

ConditionerBase::ConditionerBase()
{
	_numInputs = 1;
	_numOutputs = 1;
	_inputs.resize( 1 );
	_outputs.resize( 1 );
	_variableInputs = false;
	_outputs[0]._linked = true;
	_outputs[0]._editable = false;
	
}

ConditionerBase::ConditionerBase(const ConditionerBase &toCopy)
{
	printf("*** Copy constructor !!!");
}

ConditionerBase::~ConditionerBase()
{
}

bool ConditionerBase::setDAE( DAE *dae )
{
	_dae = dae;
	return true;
}

std::string ConditionerBase::getCategory() const
{
	return "";
}

void ConditionerBase::setName( const std::string &nm )
{
	_name = nm;
}

void ConditionerBase::setNumInputs( unsigned int num, bool variable )
{
	_numInputs = num;
	_inputs.resize( _numInputs );
	_variableInputs = variable;
}

bool ConditionerBase::setInput( unsigned int num, const std::string &input )
{
	if ( num >= _numInputs )
		return false;

	_inputs[num] = input;
	return true;
}

void ConditionerBase::setNumOutputs( unsigned int num )
{
	_numOutputs = num;
	_outputs.resize( _numOutputs );
}

bool ConditionerBase::setOutput( unsigned int num, const std::string &output )
{
	if ( num >= _numOutputs )
		return false;

	if ( _outputs[num]._linked )
		return false;

	_outputs[num]._output = output;
	return true;
}

bool ConditionerBase::setOutputProperties( unsigned int num, bool linked, bool editable )
{
	if ( num >= _numOutputs )
		return false;
	
	if ( linked && editable )
		return false;

	if ( linked && num >= _numInputs )
		return false;

	_outputs[num]._editable = editable;
	_outputs[num]._linked = linked;

	return true;
}

void ConditionerBase::addBoolOption( const std::string &option, const std::string &fullName, 
	const std::string &description, bool def )
{
	BoolOption bo;
	bo.fullName = fullName;
	bo.description = description;
	bo.value = def;
	bo.flag = false;

	_boolOptions.insert( std::make_pair( option, bo ) );
}

void ConditionerBase::addStringOption( const std::string &option, const std::string &fullName, 
	const std::string &description, const std::string &def, bool fileMenu )
{
	StringOption so;
	so.fullName = fullName;
	so.description = description;
	so.value = def;
	so.flag = fileMenu;

	_stringOptions.insert( std::make_pair( option, so ) );
}

void ConditionerBase::addFloatOption( const std::string &option, const std::string &fullName, 
	const std::string &description, float def )
{
	FloatOption fo;
	fo.fullName = fullName;
	fo.description = description;
	fo.value = def;
	fo.flag = false;

	_floatOptions.insert( std::make_pair( option, fo ) );
}

const std::string &ConditionerBase::getName() const
{
	return _name;
}

int ConditionerBase::getNumInputs() const
{
	return _numInputs;
}

const std::string &ConditionerBase::getInput( unsigned int num ) const
{
	if ( num >= _numInputs )
		return NULLString;

	return _inputs[num];
}

bool ConditionerBase::hasVariableNumInputs() const
{
	return _variableInputs;
}

int ConditionerBase::getNumOutputs() const
{
	return _numOutputs;
}

const std::string &ConditionerBase::getOutput( unsigned int num ) const
{
	if ( num >= _numOutputs )
		return NULLString;

	if ( _outputs[num]._linked )
		return _inputs[num];

	return _outputs[num]._output;
}

bool ConditionerBase::isOutputLinked( unsigned int num ) const
{
	if ( num >= _numOutputs )
	{
		return false;
	}
	return _outputs[num]._linked;
}

bool ConditionerBase::isOutputEditable( unsigned int num ) const
{
	if ( num >= _numOutputs )
	{
		return false;
	}
	return _outputs[num]._editable;
}

bool ConditionerBase::getBoolOption( const std::string &option, bool &out ) const
{
	BoolOptionMap::const_iterator iter = _boolOptions.find( option );
	if ( iter == _boolOptions.end() )
		return false;

	out = iter->second.value;
	return true;
}

bool ConditionerBase::getStringOption( const std::string &option, std::string &out ) const
{
	StringOptionMap::const_iterator iter = _stringOptions.find( option );
	if ( iter == _stringOptions.end() )
		return false;

	out = iter->second.value;
	return true;
}

bool ConditionerBase::getFloatOption( const std::string &option, float &out ) const
{
	FloatOptionMap::const_iterator iter = _floatOptions.find( option );
	if ( iter == _floatOptions.end() )
		return false;

	out = iter->second.value;
	return true;
}

bool ConditionerBase::setBoolOption( const std::string &option, bool val )
{
	BoolOptionMap::iterator iter = _boolOptions.find( option );
	if ( iter == _boolOptions.end() )
		return false;

	iter->second.value = val;
	return true;
}

bool ConditionerBase::setStringOption( const std::string &option, const std::string &val )
{
	StringOptionMap::iterator iter = _stringOptions.find( option );
	if ( iter == _stringOptions.end() )
		return false;

	iter->second.value = val;
	return true;
}

bool ConditionerBase::setFloatOption( const std::string &option, float val )
{
	FloatOptionMap::iterator iter = _floatOptions.find( option );
	if ( iter == _floatOptions.end() )
		return false;

	iter->second.value = val;
	return true;
}

const ConditionerBase::BoolOptionMap &ConditionerBase::getBoolOptionsMap() const
{
	return _boolOptions;
}

const ConditionerBase::StringOptionMap &ConditionerBase::getStringOptionsMap() const
{
	return _stringOptions;
}

const ConditionerBase::FloatOptionMap &ConditionerBase::getFloatOptionsMap() const
{
	return _floatOptions;
}

void ConditionerBase::printDebugMessage( const std::string &message )
{
#ifdef _DEBUG
  #ifdef REFINERY
	LibLoader::printDebugMessage( message );
  #else
	printf( message.c_str() );
  #endif
#endif
}

void ConditionerBase::printExecutionMessage( const std::string &message )
{
#ifdef REFINERY
	LibLoader::printExecutionMessage( message );
  #else
	printf( message.c_str() );
  #endif
}

void ConditionerBase::printErrorMessage( const std::string &message )
{
#ifdef REFINERY
	LibLoader::printErrorMessage( message );
  #else
	fprintf( stderr, message.c_str() );
  #endif
}

Conditioner *ConditionerBase::create() const
{
	//If you are going to use your derived Conditioner class as the creation
	//object you MUST overwrite this function to return an instance of your
	//derived class.
	//If you don't want an extra instance of your class to be used as the 
	//creator then you can write a class that derives from ConditionerCreator
	//and have it's create() function returns an instance of your derived class.
	assert( 0 );

	return NULL;
}

#if 0
jobject Conditioner::startNewChart(string name){
	jstring nameJString = env->NewStringUTF(name.c_str());
	
	jmethodID mid = env->GetMethodID(cls, "startNewChart", "(Ljava/lang/String;)Lrefinery/output/Chart;");
	if (mid == 0) {
		printf("startNewChart not found !!\n");
	    return false;
	}
	jvalue	args[2];
	args[0].l = nameJString;
	return env->CallObjectMethodA(obj, mid, args);
}

void Conditioner::startProfile(jobject chart, string profileName){
	jstring profileNameJString = env->NewStringUTF(profileName.c_str());
	
	jmethodID mid = env->GetMethodID(cls, "startChartProfile", "(Lrefinery/output/Chart;Ljava/lang/String;)V");
	if (mid == 0) {
		printf("startChartProfile not found !!\n");
	    return;
	}
	jvalue	args[2];
	args[0].l = chart;
	args[1].l = profileNameJString;
	env->CallVoidMethodA(obj, mid, args);
}

void Conditioner::startGroup(jobject chart, string groupName){
	jstring groupNameJString = env->NewStringUTF(groupName.c_str());
	
	jmethodID mid = env->GetMethodID(cls, "startChartGroup", "(Lrefinery/output/Chart;Ljava/lang/String;)V");
	if (mid == 0) {
		printf("startChartGroup not found !!\n");
	    return;
	}
	jvalue	args[2];
	args[0].l = chart;
	args[1].l = groupNameJString;
	env->CallVoidMethodA(obj, mid, args);
}

void Conditioner::addDataToChart(jobject chart, string elementName,  jint *array, int _size){
	jsize size = _size;
	jintArray  values = env->NewIntArray(size);
	
	env->SetIntArrayRegion(values, 0, size, array);
	
	jstring elementNameJString = env->NewStringUTF(elementName.c_str());
	
	jmethodID mid = env->GetMethodID(cls, "addChartElement", "(Lrefinery/output/Chart;Ljava/lang/String;[I)V");
	if (mid == 0) {
		printf("addChartElement not found !!\n");
	    return;
	}
	jvalue	args[3];
	args[0].l = chart;
	args[1].l = elementNameJString;
	args[2].l = values;
	env->CallVoidMethodA(obj, mid, args);
}

void Conditioner::displayChart(jobject chart){
	jmethodID mid = env->GetMethodID(cls, "displayChart", "(Lrefinery/output/Chart;)V");
	if (mid == 0) {
		printf("displayChart not found !!\n");
	    return;
	}
	jvalue	args[1];
	args[0].l = chart;
	env->CallVoidMethodA(obj, mid, args);
}
#endif
