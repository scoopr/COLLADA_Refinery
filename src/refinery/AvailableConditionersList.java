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

import java.util.Iterator;
import java.util.Vector;
import java.io.*;

/** 
 *  A <code>AvailableConditionersList</code> is a list of available ConditionerBases, created from
 *  loaded libraries.
 */
public class AvailableConditionersList {

	private Vector <ConditionerTemplate> conditionerBasesList;
	
	public AvailableConditionersList(){
		conditionerBasesList = new Vector<ConditionerTemplate>();
	}
	
	public void add(ConditionerTemplate cb){
		conditionerBasesList.add(cb);
	}
	
	public Iterator<ConditionerTemplate> iterator(){
		return conditionerBasesList.iterator();
	}
	
	/**
	 * Find a ConditionerTemplate corresponding to the provided name.
	 * @param libName name of a loaded library.
	 * @return The corresponding ConditionerTemplate if found, null otherwise.
	 */
	public ConditionerTemplate getConditionerBase(String name)
	{
		Iterator<ConditionerTemplate> cbIt = iterator();
		while(cbIt.hasNext())
		{
			ConditionerTemplate cb = cbIt.next();
			if(cb.getIDName().equals(name) )
			{
				return cb;
			}
		}		
		return null;
	}
	
	public void load(LibLoader ll, String directory )
	{
		final String LIB_EXTENSION;
		if (System.getProperty("os.name").contains("Windows"))
			LIB_EXTENSION = "dll";
		else if (System.getProperty("os.name").contains("Mac OS"))
			LIB_EXTENSION = "dylib";
		else if (System.getProperty("os.name").contains("Linux"))
			LIB_EXTENSION = "so";
		else {
			System.err.println("Could not determine OS for platform-specific LIB_EXTENSION");
			return;
		}

		File libDirectory = new File(directory);
		if (!libDirectory.exists())
		{
			//libDirectory.mkdirs();
			return;
		}
		String[] libList = libDirectory.list();

		//Refinery.instance.addDebugMessage("Lib list:");
		for (int i = 0; i < libList.length; i++)
		{
			File file = new File(directory + File.separator + libList[i]);
			if (file.isFile() && file.getName().matches("[a-zA-Z][a-zA-Z0-9\\-\\_]*\\." + LIB_EXTENSION))
			{
			
				//note: if we catch the unsatisfied link-error, the Refinery won't crash just because
				//of one broken conditioner, it simply does not load it
				try
				{
					System.load(file.getCanonicalPath());
				}
				catch (UnsatisfiedLinkError ule)
				{
					ule.printStackTrace();
				}
				catch (Exception e)
				{
					System.err.println(e.getMessage());
				}
			}
		}
	}

	public void printList(){
		System.out.println("Conditioners found :");
		Iterator<ConditionerTemplate> cIt = conditionerBasesList.iterator();
		while(cIt.hasNext()){
			System.out.println(" " + cIt.next().getIDName());
		}
	}

}
