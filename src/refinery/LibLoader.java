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
package refinery;

import refinery.Option.OptionType;
import refinery.document.ConditionerWrapper;
import refinery.output.Chart;

import java.io.File;

public class LibLoader
{
	private AvailableConditionersList list;
	private Refinery main;
	private String errorMessage;

	/**
	 * Constructor
	 * @param _list AvailableConditionersList where to add new Conditioner Bases
	 * @param _main Reference to the main class
	 */
	public LibLoader(AvailableConditionersList _list, Refinery _main)
	{
		list = _list;
		main = _main;
		errorMessage = "";
		init();
	}

	/*************** Interface Java -> conditioners **************************/
	/**
	 * Inits the LibLoader native code. This basically just sets the JNI environment.
	 */
	public /*static*/ native void init();

	/**
	 * Gets the number of Conditioners that are available in the LibLoader library.
	 * @return Returns the number of available conditioners.
	 */
	public native int getNumAvailableConditioners();

	/**
	 * Gets the conditioner given the index.
	 * @param idx The idx into the available conditioner list to get.
	 * @return Returns a new ConditionerTemplate representing the conditioner.
	 */
	public native ConditionerTemplate getConditioner(int idx);

	/**
	 * Creates a new instance of a conditioner in native code.
	 * @param baseName The name of the conditioner to create.
	 * @param name The unique name to identify this instance.
	 * @param cs The ConditionerWrapper that represents this conditioner.
	 * @return Returns true if the conditioner is created and initialized correctly.
	 */
	public native boolean loadConditioner(String baseName, String name, ConditionerWrapper cw );
	public native boolean removeConditioner(String name);

	public native boolean loadBoolOptions(String name, ConditionerWrapper cw);
	public native boolean loadStringOptions(String name, ConditionerWrapper cw);
	public native boolean loadFloatOptions(String name, ConditionerWrapper cw);
	
	/**
	 * Changes the name of a conditioner instance.
	 * @param oldName The current name of the conditioner.
	 * @param newName The new name for the conditioner.
	 */
	public native boolean changeConditionerName(String oldName, String newName);

	public static native void clearDOM();

	/**
	 * Execute a plugin based on the settings in the ConditionerWrapper
	 * @param libName Library to execute
	 * @param cw ConditionerWrapper containing the settings
	 */
	public native int execute(String name);

	/**
	 * Loads a document natively.
	 * @param doc The filepath of the document to load.
	 * @return Returns true on success. False otherwise.
	 */
	public static native boolean loadDocument(String doc);

	/**
	 * Saves a document natively.
	 * @param doc The document to save.
	 * @param toFile The filepath to save the document as.
	 * @return Returns true on success. False otherwise.
	 */
	public static native boolean saveDocument(String doc, String toFile);

	public native void setInputNumber(String name, int num);
	public native void setInput(String name, int num, String doc);
	public native void setOutput(String name, int num, String doc);

	public native void setBoolOption(String cName, String oName, boolean val);
	public native void setStringOption(String cName, String oName, String val);
	public native void setFloatOption(String cName, String oName, float val);

	public static native void cloneDocument(String fromDoc, String toDoc);
	
	/**
	 * load the native code loading libraries
	 */
	static
	{
		//Possibly make this variable so we can have different conditioner libraries.
		//System.loadLibrary("libloader");
		System.load(Refinery.RefineryDirectory + File.separator + "libloader.dll");
		//init();
	}

	/*************** Interface plugins -> Java  **************************/
	/******* Methods to create and set Condtioner Bases */
	/**
	 * Create a new ConditionerTemplate
	 * @param name Name of the new Conditioner
	 * @param description The description of the new Conditioner
	 * @return The newly created ConditionerTemplate
	 */
	public ConditionerTemplate newConditionerBase(String name, String description, String category)
	{
		ConditionerTemplate cb = new ConditionerTemplate(name, description, category);
		return cb;
	}

	/******* Methods to return results back the GUI */
	/**
	 * Add a debug message to the Console
	 */
	private void debugMessage(String result)
	{
		main.addDebugMessage(result);
	}

	private void executionMessage(String result)
	{
		main.addNormalMessage(result);
	}

	private void setErrorMessage(String result)
	{
		errorMessage = result;
	}

	public String getErrorMessage()
	{
		return errorMessage;
	}

	/* !!!ACL - I Have No Clue What these Chart Functions Do - Or If They Are Useful
	private Chart startNewChart(String name){
		return new Chart(name);
	}
	
	private void startChartProfile(Chart chart, String profileName){
		chart.startProfile(profileName);
	}
	
	private void startChartGroup(Chart chart, String groupName){
		chart.startGroup(groupName);
	}
	
	private void addChartElement(Chart chart, String name, int [] values){
		chart.addElement(name, values);
	}
	*/

		/**
	 * Display a Chart once all values are set.
	 * @param chart
	 */
		/*private void displayChart(Chart chart){
		chart.display();
	}*/
}
