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
