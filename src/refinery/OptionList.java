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
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class OptionList{
	private Vector<Option> optionList;
	
	public OptionList(){
		optionList = new Vector<Option>();
	}
	
	public boolean addOption(Option option){
		if (getOption(option.name) != null) {return false;}
		
		return optionList.add(option);
	}
	
	public Option getOption(String name){
		Iterator<Option> optIt = optionList.iterator();
		while(optIt.hasNext()){
			Option option = optIt.next();
			if (option.name.equals(name)) { return option; } 
		}
		return null;
	}
	
	public Iterator<Option> getOptions(){
		return optionList.iterator();
	}
	
	public boolean isSet(String optionName){
		Option o = getOption(optionName);
		if (o == null) return false;
		return o.isSet;
	}
	
	public String getValue(String optionName){
		Option o = getOption(optionName);
		if ( o != null )	
			return String.valueOf(o.getValue());
		return null;
	}
	
	public boolean setValue(String name, String value){
		Option o = getOption(name);
		if (o == null )
			return false;
		if (o.type == Option.OptionType.BOOL)
		{
			if (value.equals("true"))
				o.setValue(true);
			else
				o.setValue(false);
			return true;
		}
		if (o.type == Option.OptionType.FLOAT)
		{
			try
			{
				o.setValue(Float.parseFloat(value));
				return true;
			}
			catch (Exception e)
			{
				o.unset();
				return false;
			}
		}
		if (o.type == Option.OptionType.STRING)
		{
			o.setValue(value);
			return true;
		}
		return false;
	}
	
	public void parse(String []args) throws ParsingErrorException{
		for(int i = 0; i< args.length; i++){
			if (args[i].startsWith("-")){
				String s = args[i].substring(1);
				if (s.equals("i") || s.equals("o"))
				{
					i++;
					continue;
				}
				Option o = getOption( s );
				if (o == null) throw new ParsingErrorException( s +" is not a recognized option");
				if(i+1 <args.length){
					if (o.type == Option.OptionType.BOOL)
					{
						if (args[i+1].equals("true"))
							o.setValue(true);
						else
							o.setValue(false);
					}
					if (o.type == Option.OptionType.FLOAT)
					{
						try
						{
							o.setValue(Float.parseFloat(args[i+1]));
						}
						catch (Exception e)
						{
							throw new ParsingErrorException("Value for " + s +" is not a valid floating point number");
						}
					}
					if (o.type == Option.OptionType.STRING)
					{
						o.setValue(args[i+1]);
					}
					i++;
				} 
				else 
				{
					throw new ParsingErrorException("You must give a value for " + s );
				}
			} 
			else 
			{
				// This does not does with - or --, abort parsing
				throw new ParsingErrorException(args[i]+" is not a recognized option");
			}
		}
	}
	
	public String toString(){
		String result = "";
		
		Iterator<Option> optIt = optionList.iterator();
		while(optIt.hasNext()){
			result += optIt.next().toString() + "\n";
		}
		
		if (result.equals("")){
			result = "No option\n";
		}
		
		return result;
	}
	
	public void toXML(XMLOutput output){
		//String result = "";
		Iterator<Option> optIt = getOptions();
		while(optIt.hasNext()){
			Option o = optIt.next();
			output.startEntity("option");
			output.addAttribute("name", o.name);
			switch(o.type){
			case BOOL:
				output.addAttribute("type", "bool");
				break;
			case STRING:
				output.addAttribute("type", "string");
				break;
			case FLOAT:
				output.addAttribute("type", "float");
				break;
			}
			output.addBodyText(String.valueOf(o.getValue()));
			output.endEntity();
		}		
	}
}
