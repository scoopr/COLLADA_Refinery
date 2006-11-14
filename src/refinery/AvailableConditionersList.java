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
		final String LIB_EXTENSION = "dll";
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
			File file = new File(directory + "\\" + libList[i]);
			if (file.isFile() && file.getName().matches("[a-zA-Z][a-zA-Z0-9\\-\\_]*\\." + LIB_EXTENSION))
			{
				try
				{
					System.load(file.getCanonicalPath());
				}
				catch (Exception e)
				{
					System.err.println(e.getMessage());
				}
			}
		}

		for (int i = 0; i < ll.getNumAvailableConditioners(); i++)
		{
			ConditionerTemplate cb = ll.getConditioner( i );
			if (cb != null)
			{
				add(cb);
				//Refinery.instance.addDebugMessage(cb.getIDName() + " added");
			}
			else
			{
				System.err.println("load lib call back null");
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
