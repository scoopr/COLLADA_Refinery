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
	
		//TODO: Add Behavior for Linux
	
		//Check for Mac-platform
		if (System.getProperty("os.name").contains("Mac OS")) {
			
			//At first check if Refinery is called by Xcode Debugger, in this case the 
			//following env-var would have been set and we would load the Conditioner-framework
			//directly out of the build-directory
			
			String debugFramework = System.getenv("REFINERY_XCODE_DEBUG_FRAMEWORK");
			if (debugFramework != null) {
				
				try {
					//Load the Dynamic lib from Build directory
					System.load(debugFramework+"/ColladaRefineryConditioner.framework/ColladaRefineryConditioner");					
				} catch (UnsatisfiedLinkError ule) {
					System.err.println("Could not load Framework from build directory " + debugFramework);
					ule.printStackTrace();
				}
				
			} else {
				//Try to load the Conditioner-framework from normal install location on Mac
				try {
					System.load("/Library/Frameworks/ColladaRefineryConditioner.framework/ColladaRefineryConditioner");					
				} catch (UnsatisfiedLinkError ule) {
					System.err.println("Could not load Framework from default installation-dir /Library/Frameworks/ColladaRefineryConditioner.framework/ColladaRefineryConditioner. You might have a broken installation.");
					ule.printStackTrace();
				}
			}
		} else {
		
			//For Windows load it as usual
			System.loadLibrary("libloader");
			
		}

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
