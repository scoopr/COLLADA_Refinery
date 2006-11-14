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
#ifndef _CONDITIONERBASE_H_
#define _CONDITIONERBASE_H_

#include <string>
#include <vector>
#include <map>

#include "defines.h"
#include "conditioner.h"

#ifdef REFINERY
  #include "libloader.h"
#endif

class Conditioner;
class DAE;

class ConditionerBase : public Conditioner
{
public:
	REF_EXPORT ConditionerBase();
	REF_EXPORT ConditionerBase( const ConditionerBase &toCopy );
	REF_EXPORT virtual ~ConditionerBase();

	REF_EXPORT virtual Conditioner *create() const;

	REF_EXPORT virtual bool init() = 0;
	REF_EXPORT virtual int execute() = 0;

	REF_EXPORT virtual	std::string getBaseName() const = 0;
	REF_EXPORT virtual	std::string getDescription() const = 0;
	REF_EXPORT virtual std::string getCategory() const;

	REF_EXPORT bool setDAE( DAE *dae );

	REF_EXPORT void setName( const std::string &nm );
	REF_EXPORT void setNumInputs( unsigned int num, bool variable = false );
	REF_EXPORT bool setInput( unsigned int num, const std::string &input );
	REF_EXPORT void setNumOutputs( unsigned int num );
	REF_EXPORT bool setOutput( unsigned int num, const std::string &output );
	REF_EXPORT bool setOutputProperties( unsigned int num, bool linked, bool editable = false );

	REF_EXPORT void addBoolOption( const std::string &option, const std::string &fullName, 
		const std::string &description, bool def = false );

	REF_EXPORT void addStringOption( const std::string &option, const std::string &fullName, 
		const std::string &description, const std::string &def = "", bool fileMenu = false);

	REF_EXPORT void addFloatOption( const std::string &option, const std::string &fullName, 
		const std::string &description, float def = 0.0f );

	
	REF_EXPORT const std::string &getName() const;
	REF_EXPORT int getNumInputs() const;
	REF_EXPORT const std::string &getInput( unsigned int num ) const;
	REF_EXPORT bool hasVariableNumInputs() const;
	REF_EXPORT int getNumOutputs() const;
	REF_EXPORT const std::string &getOutput( unsigned int num ) const;
	REF_EXPORT bool isOutputLinked( unsigned int num ) const;
	REF_EXPORT bool isOutputEditable( unsigned int num ) const;
	

	REF_EXPORT bool getBoolOption( const std::string &option, bool &out ) const;
	REF_EXPORT bool getStringOption( const std::string &option, std::string &out ) const;
	REF_EXPORT bool getFloatOption( const std::string &option, float &out ) const;

	REF_EXPORT bool setBoolOption( const std::string &option, bool val );
	REF_EXPORT bool setStringOption( const std::string &option, const std::string &val );
	REF_EXPORT bool setFloatOption( const std::string &option, float val );

	REF_EXPORT const BoolOptionMap &getBoolOptionsMap() const;
	REF_EXPORT const StringOptionMap &getStringOptionsMap() const;
	REF_EXPORT const FloatOptionMap &getFloatOptionsMap() const;

public:
	REF_EXPORT static void printDebugMessage( const std::string &message );
	REF_EXPORT static void printExecutionMessage( const std::string &message );
	REF_EXPORT static void printErrorMessage( const std::string &message );

protected:

	struct OutputInfo 
	{
		std::string _output;
		bool _linked;
		bool _editable;
	};

	std::string _name;
	DAE *_dae;

private:
	unsigned int _numInputs;
	unsigned int _numOutputs;
	bool _variableInputs;
	
	std::vector< std::string > _inputs;
	std::vector< OutputInfo > _outputs;

	BoolOptionMap _boolOptions;
	StringOptionMap _stringOptions;
	FloatOptionMap _floatOptions;

#if 0
	/****** Chart methods *****/
	// Start a new chart. The following methods can be used to fill it with data
	jobject startNewChart(string name);

	// Start a new profile in the current chart, displayed in a new tab
	void startProfile(jobject chart, string profileName);

	// Start a new group in the current profile
	void startGroup(jobject chart, string groupName);

	// Add data for <size> different entities in the current chart, current profile, current group.
	void addDataToChart(jobject chart, string elementName, jint *array, int size);

	// Display the current chart
	void displayChart(jobject chart);

#endif
	
};

#endif //_CONDITIONERBASE_H_
