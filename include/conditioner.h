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
#ifndef _CONDITIONER_H_
#define _CONDITIONER_H_

#include <string>
#include <vector>
#include <map>

#ifdef REFINERY
  #include "libloader.h"
#endif

class Conditioner;
class DAE;

/**
 * ConditionerCreator is an interface to enable creation of Conditioner objects.
 * The user of this class must inherit from this class and implement it's functions.
 */
class ConditionerCreator
{
public:
	
	/**
	 * Constructor.
	 */
	ConditionerCreator() {}

	/**
	 * Destructor.
	 */
	virtual ~ConditionerCreator() {}

	/**
	 * Gets the name to identify Conditioners that can be created by this class.
	 * @return Returns the identifying name for the Conditioner object this class knows to create.
	 */
	virtual	std::string getBaseName() const	= 0;
	/**
	 * Gets the description of Conditioners that can be created by this class.
	 * @return Returns the description for the Conditioner object this class knows to create.
	 */
	virtual	std::string getDescription() const	= 0;
	/**
	 * Gets the category for Conditioners that can be created by this class.
	 * @return Returns the category name for the Conditioner object this class knows to create.
	 */
	virtual std::string getCategory() const { return ""; }
	/**
	 * Creates a new Conditioner object for the type this class knows to create.
	 * @return Returns a pointer to a new Conditioner object.
	 */
	virtual Conditioner *create() const = 0;
};

/**
 * Conditioner is an interface for conditioners to work with the COLLADA_REFINERY. You must implement 
 * this interface with your desired functionality for your conditioner. The CondtionerBase class is a 
 * default implementation that can be inherited from to ease the Conditioner creation process.
 * Conditioner inherits from ConditionerCreator therefore a Condtioner can know how to create itself.
 */
class Conditioner : public ConditionerCreator
{
public:

	/**
	 * A templated struct used for defining Conditioner options.
	 */
	template <typename T>
	struct TypedOption
	{
		/**
		 * The full name for this option.
		 */
		std::string fullName;

		/**
		 * The description of this option.
		 */
		std::string description;

		/**
		 * Storage to hold the value of this option.
		 */
		T value;

		/**
		 * Flag to allow special processing of this option.
		 * Currently this is only used for Strings to enable a file dialog box.
		 */
		bool flag;
	};

	/**
	 * Boolean option type.
	 */
	typedef TypedOption<bool> BoolOption;

	/**
	 * String option type.
	 */
	typedef TypedOption<std::string> StringOption;

	/**
	 * Floating point number option type.
	 */
	typedef TypedOption<float> FloatOption;

	/**
	 * A map for Boolean options. The key is the option's name. The value is a BoolOption.
	 */
	typedef std::map< std::string, BoolOption > BoolOptionMap;

	/**
	 * A map for String options. The key is the option's name. The value is a StringOption.
	 */
	typedef std::map< std::string, StringOption > StringOptionMap;

	/**
	 * A map for Float options. The key is the option's name. The value is a FloatOption.
	 */
	typedef std::map< std::string, FloatOption > FloatOptionMap;

	/**
	 * Register is a class used for registering and unregistering Conditoners with the 
	 * COLLADA Refinery.
	 * To register a conditioner create a global object of type Conditioner::Register with
	 * a template argument of the ConditionerCreator implementation that can create your
	 * Conditioner. Here is an example:
	 * (in header)
	 * class MyConditioner;
	 * class MyConditionerCreator : public ConditionerCreator
	 * {
	 *   ...
	 *   Conditioner *create() { return new MyConditioner(); }
	 * };
	 * (in .cpp)
	 * Conditioner::Register<MyConditionerCreator> registerObject();
	 */
	template <class T>
	class Register : public T
	{
	public:
		Register()
		{
#ifdef REFINERY
			LibLoader::registerConditioner( this );
#endif
		}

		~Register()
		{
#ifdef REFINERY
			LibLoader::unregisterConditioner( this );
#endif
		}
	private:
		Register( const Register & ) {}
		Register &operator=( const Register & ) { return *this; }
	};

public:

	/**
	 * Constructor.
	 */
	Conditioner() {}

	/**
	 * Destructor.
	 */
	virtual ~Conditioner() {}

	virtual Conditioner *create() const = 0;

	/**
	 * Initializes this Conditioner object. This function is called after a Conditioner object is 
	 * created. All Conditioner options must be declared before the end of execution of this function.
	 */
	virtual bool init()	= 0;

	/**
	 * Executes this Conditioner object. The implementation is responsible for checking the inputs,
	 * outputs, and any options that it requires to run correctly.
	 */
	virtual int execute() = 0;

	virtual	std::string getBaseName() const		= 0;
	virtual	std::string getDescription() const	= 0;
	virtual std::string getCategory() const { return ""; }

	/**
	 * For the best interopability, Condtioners should use the COLLADA DOM, but they are not required 
	 * to. The Refinery will manage the DAE object for running the pipeline and will call this function
	 * passing in a pointer to that object after a Conditioner is created. The ConditionerBase 
	 * implementation will store this pointer for the Conditioner to use later but if your Conditioner 
	 * does not use the COLLADA DOM you are responsible for getting and updating the documents needed
	 * from this DAE pointer.
	 * @param dae A pointer to the DAE object that the Refinery pipeline is operating on.
	 */
	virtual bool setDAE( DAE *dae ) = 0;

	/**
	 * Sets the name of this Conditioner object. Each Conditioner object is identified by it's
	 * unique name.
	 * @param nm The name to set for this Conditioner.
	 * @note This function is called internally by the Refinery. It is not recommended for client 
	 * code to call this function.
	 */
	virtual void setName( const std::string &nm ) = 0;

	/**
	 * Sets the number of inputs for this Conditioner. This function should be called during 
	 * initialization to specify the number of inputs required by this Conditioner and whether
	 * or not this Conditioner allows for a variable number of inputs.
	 * @param num The number of inputs for this Conditioner.
	 * @param variable Set to true if this Conditioner can have a variable number of inputs.
	 * @note If you set variable to true make sure to check the number of inputs during
	 * execution. If variable, this function will be called before Conditioner execution to
	 * set the number of inputs.
	 */
	virtual void setNumInputs( unsigned int num, bool variable = false ) = 0;

	/**
	 * Sets the document associated with an input number.
	 * @param num The input number to set.
	 * @param input The document assigned to this input from the Refinery.
	 * @note This function is called from the Refinery upon execution. It is not recommended to call
	 * this function in client code.
	 */
	virtual bool setInput( unsigned int num, const std::string &input ) = 0;

	/**
	 * Sets the number of outputs for this Conditioner. This function should be called during 
	 * initialization to specify the number of output documents generated by this Conditioner.
	 * @param num The number of outputs for this Conditioner.
	 */
	virtual void setNumOutputs( unsigned int num ) = 0;

	/**
	 * Sets the document name associated with an output.
	 * @param num The output number to set.
	 * @param output The document assigned to this output.
	 * @return Returns true upon success. False otherwise.
	 * @note Depending on the properties associated with this output this function may be called
	 * by the Refinery prior to execution. If an output is not linked and not editable it must be
	 * set during initialization.
	 */
	virtual bool setOutput( unsigned int num, const std::string &output ) = 0;

	/**
	 * Sets the properties of an output.
	 * @param num The output number to set.
	 * @param linked If linked is true the output has the same value as the input of the same number.
	 * @param editable If editable is true the user can set the output document as a property in the
	 * Refinery GUI.
	 * @return Returns true upon success. False otherwise.
	 * @note This function will fail if both linked and editable are true or if linked is true and the
	 * number of inputs is less than num.
	 */
	virtual bool setOutputProperties( unsigned int num, bool linked, bool editable = false ) = 0;

	/**
	 * Registers a Boolean option for this Conditioner object.
	 * @param option The name of this option. This will also be the command line switch to set this option.
	 * @param fullName The fullName of this option.
	 * @param description The description of this option.
	 * @param def The initial value for this option.
	 */
	virtual void addBoolOption( const std::string &option, const std::string &fullName, 
		const std::string &description, bool def = false ) = 0;

	/**
	 * Registers a String option for this Conditioner object.
	 * @param option The name of this option. This will also be the command line switch to set this option.
	 * @param fullName The fullName of this option.
	 * @param description The description of this option.
	 * @param def The initial value for this option.
	 */
	virtual void addStringOption( const std::string &option, const std::string &fullName, 
		const std::string &description, const std::string &def = "", bool fileMenu = false) = 0;

	/**
	 * Registers a Float option for this Conditioner object.
	 * @param option The name of this option. This will also be the command line switch to set this option.
	 * @param fullName The fullName of this option.
	 * @param description The description of this option.
	 * @param def The initial value for this option.
	 */
	virtual void addFloatOption( const std::string &option, const std::string &fullName, 
		const std::string &description, float def = 0.0f ) = 0;

	/**
	 * Gets the name of this Conditioner object.
	 * @return Returns the unique name assigned to this Conditioner object.
	 */
	virtual const std::string &getName() const = 0;

	/**
	 * Gets the number of inputs for this Conditioner object.
	 * @returns Returns the number of inputs for this Conditioner object.
	 * @note This function is especially inportant to call if a Conditioner allows for a variable number
	 * of input documents.
	 */
	virtual int getNumInputs() const = 0;

	/**
	 * Gets the name of the document assigned to an input. This function should be called during 
	 * execution to identify which document to process by the Conditioner.
	 * @param num The number for the input to get.
	 * @return Returns the name of the document assigned to the input.
	 */
	virtual const std::string &getInput( unsigned int num ) const = 0;

	/**
	 * Checks if this Conditioner has a variable number of inputs.
	 * @return Returns true if the Conditioner has a variable number of inputs. False otherwise.
	 */
	virtual bool hasVariableNumInputs() const = 0;

	/**
	 * Gets the number of outputs for this Conditioner.
	 * @return Returns the number of outputs for this Conditioner.
	 */
	virtual int getNumOutputs() const = 0;

	/**
	 * Gets the name of the document assigned to an output.
	 * @param num The number for the output to get.
	 * @return Returns the name of the document assigned to the output.
	 * @note This is especially important to call during execution for any output that is editable 
	 * by the user.
	 */
	virtual const std::string &getOutput( unsigned int num ) const = 0;

	/**
	 * Checks if an output is linked to an input.
	 * @param num The output number to check.
	 * @return Returns true if the output is linked to the input with the same num.
	 */
	virtual bool isOutputLinked( unsigned int num ) const = 0;

	/**
	 * Checks if an output is editable by the user.
	 * @param num The output number to check.
	 * @return Returns true if the output is editable.
	 */
	virtual bool isOutputEditable( unsigned int num ) const = 0;

	/**
	 * Gets the value assigned to a Boolean option.
	 * @param option The identifier of the option to get.
	 * @param out The return value.
	 * @return Returns true upon success. False if the option does not exist.
	 */
	virtual bool getBoolOption( const std::string &option, bool &out ) const = 0;

	/**
	 * Gets the value assigned to a String option.
	 * @param option The identifier of the option to get.
	 * @param out The return value.
	 * @return Returns true upon success. False if the option does not exist.
	 */
	virtual bool getStringOption( const std::string &option, std::string &out ) const = 0;

	/**
	 * Gets the value assigned to a Float option.
	 * @param option The identifier of the option to get.
	 * @param out The return value.
	 * @return Returns true upon success. False if the option does not exist.
	 */
	virtual bool getFloatOption( const std::string &option, float &out ) const = 0;

	/**
	 * Sets the value for a Boolean option.
	 * @param option The identifier of option the to set.
	 * @param val The value to set the option to.
	 * @return Returns true upon success. False if the option does not exist.
	 * @note This function will be called by the Refinery for each Boolean option before execution
	 * regardless of whether or not the option was actually changed.
	 */
	virtual bool setBoolOption( const std::string &option, bool val ) = 0;

	/**
	 * Sets the value for a String option.
	 * @param option The identifier of option the to set.
	 * @param val The value to set the option to.
	 * @return Returns true upon success. False if the option does not exist.
	 * @note This function will be called by the Refinery for each Boolean option before execution
	 * regardless of whether or not the option was actually changed.
	 */
	virtual bool setStringOption( const std::string &option, const std::string &val ) = 0;

	/**
	 * Sets the value for a Float option.
	 * @param option The identifier of option the to set.
	 * @param val The value to set the option to.
	 * @return Returns true upon success. False if the option does not exist.
	 * @note This function will be called by the Refinery for each Boolean option before execution
	 * regardless of whether or not the option was actually changed.
	 */
	virtual bool setFloatOption( const std::string &option, float val ) = 0;

	/**
	 * Gets the map of Boolean options.
	 * @return Returns a map of Boolean options for this conditioner.
	 */
	virtual const BoolOptionMap &getBoolOptionsMap() const = 0;

	/**
	 * Gets the map of String options.
	 * @return Returns a map of Boolean options for this conditioner.
	 */
	virtual const StringOptionMap &getStringOptionsMap() const = 0;

	/**
	 * Gets the map of Float options.
	 * @return Returns a map of Boolean options for this conditioner.
	 */
	virtual const FloatOptionMap &getFloatOptionsMap() const = 0;

};

#endif //_CONDITIONER_H_
