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
package refinery.document;

import java.util.Iterator;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Map;

import refinery.ConditionerTemplate;
import refinery.EntryException;
import refinery.Option;
import refinery.OptionList;
import refinery.ParsingErrorException;
import refinery.Refinery;
import refinery.XMLOutput;
import refinery.display.ConditionerDisplay;
import refinery.display.DisplayAreaInterface;
import refinery.document.Anchor.AnchorType;
import refinery.history.ActionHistory;
import refinery.history.IdentifiableImpl;


/**
 * The <code>ConditionerWrapper</code> class is the wrapper on the java side of 
 * a subsidial plug-in. Its purpose is
 * <ol>
 * 		<li>To expose to the gui an object which reflects the plug-in possibilities
 * 			 and possible properties
 * 		<li>Set properties for this plug-in and this instance through the GUI.
 * 		<li>Call the plug-in execution when asked by the user with parameters set
 * 			at edition time.
 * </ol>
 * 
 * @author Philippe David
 */

public class ConditionerWrapper extends IdentifiableImpl implements Box
{
	private Pipeline document;
	private Refinery refinery;
	private ConditionerDisplay display;
	private ActionHistory history;

	private String name;
	private String description;
	private String baseName;

	private OptionList options;
	private Vector<AnchorConditioner> inputAnchors;
	private Vector<AnchorConditioner> outputAnchors;
	private boolean variableInputNumber;
	private int inputNumber;
	private int outputNumber;

	private class OutputInfo
	{
		public String _output;
		public boolean _linked;
		public boolean _editable;
	}

	private Vector<String> _inputs;
	private Vector<OutputInfo> _outputs;

	private Hashtable<String, String> cloneList;

	public ConditionerWrapper(ConditionerTemplate cb, Pipeline _document, Refinery _refinery, ActionHistory h)
	{
		document = _document;
		refinery = _refinery;
		name = cb.getIDNameWithCount();
		description = cb.getDescription();
		baseName = cb.getIDName();

		options = new OptionList();

		inputAnchors = new Vector<AnchorConditioner>();
		outputAnchors = new Vector<AnchorConditioner>();
		_inputs = new Vector<String>();
		_outputs = new Vector<OutputInfo>();

		//load the conditioner natively - this function will populate the available options
		//as well as the number of inputs and outputs.
		refinery.getLibLoader().loadConditioner(baseName, getIDName(), this);
		refinery.getLibLoader().loadBoolOptions(getIDName(), this);
		refinery.getLibLoader().loadStringOptions(getIDName(), this);
		refinery.getLibLoader().loadFloatOptions(getIDName(), this);
		
		cloneList = new Hashtable<String, String>();
		
		history = h;
		display = null;
	}

	public void init(int numIns, int numOuts, boolean varIns, String[] outputs, boolean[] outputInfo )
	{
		setInputNumber(numIns);
		setOutputNumber(numOuts);
		variableInputNumber = varIns;
		for (int i = 0; i < numOuts; i++)
		{
			_outputs.get(i)._output = outputs[i];
			_outputs.get(i)._linked = outputInfo[i * 2];
			_outputs.get(i)._editable = outputInfo[i * 2 + 1];
			//System.out.println("output " + i + " linked = " + _outputs.get(i)._linked + "; editable = " + _outputs.get(i)._editable);
			//System.out.println("output set to " + outputs[i]);
		}
	}

	public void addBoolOption(String name, String fullName, String description, boolean val)
	{
		try
		{
			Option<Boolean> o = new Option<Boolean>(name, fullName, description,
				Option.OptionType.BOOL);
			o.setValue(val);
			options.addOption(o);
		}
		catch (Exception e)
		{

		}
	}

	public void addStringOption(String name, String fullName, String description, String val, boolean flag )
	{
		try
		{
			Option<String> o = new Option<String>(name, fullName, description,
				Option.OptionType.STRING);
			o.setValue(val);
			o.setFlag(flag);
			//System.out.println("option " + name + " = " + val);
			options.addOption(o);
		}
		catch (Exception e)
		{
		}
	}

	public void addFloatOption(String name, String fullName, String description, float val)
	{
		try
		{
			Option<Float> o = new Option<Float>(name, fullName, description,
				Option.OptionType.FLOAT);
			o.setValue(val);
			//System.out.println("option " + name + " = " + val);
			options.addOption(o);
		}
		catch (Exception e)
		{
		}
	}

	public String getLibName()
	{
		return baseName;
	}

	public void setName(String name)
	{
		if (this.name.equals(name))
		{
			return;
		}
		//System.out.println("Changing Names from " + this.name + " to " + name +"!");
		String oldIDName = getIDName();
		this.name = name;
		refinery.getLibLoader().changeConditionerName(oldIDName, getIDName());
		if (display != null)
		{
			display.repaint();
		}
	}

	public void setDisplayArea(DisplayAreaInterface area)
	{
		if (display == null)
		{
			display = new ConditionerDisplay(this, history);
		}
		display.repaint();
		area.addDisplayAtRandomPosition(display);

	}

	public ConditionerDisplay getDisplay()
	{
		return display;
	}

	public String getName()
	{
		return name;
	}

	public String getIDName()
	{
		return name + getUID();
	}

	public String getDescription()
	{
		return description;
	}

	public OptionList getOptionList()
	{
		return options;
	}

	public Anchor getInputAnchor(int number)
	{
		return inputAnchors.get(number);
	}

	public Iterator<Anchor> getInputAnchors()
	{
		Vector<Anchor> tmp = new Vector<Anchor>(inputAnchors);
		return tmp.iterator();
	}

	public Anchor getOutputAnchor(int number)
	{
		return outputAnchors.get(number);
	}

	public Iterator<Anchor> getOutputAnchors()
	{
		Vector<Anchor> tmp = new Vector<Anchor>(outputAnchors);
		return tmp.iterator();
	}

	/**
	 * Finds the first available Anchor of the given type. This is used when the user isending a link
	 * on the Conditioner instead of a specific anchor to determine to which anchor the link should
	 * end. Can return null if no anchor is considered available.
	 */
	public Anchor getLinkEndAnchor(AnchorType type)
	{
		if (type == AnchorType.ANCHOROUT)
		{
			if (outputNumber == 0)
			{
				return null;
			}
			else if (outputNumber == 1)
			{
				return getOutputAnchor(0);
			}
			else
			{ //select the first free Anchor
				Iterator<Anchor> ancs = getOutputAnchors();
				while (ancs.hasNext())
				{
					Anchor anc = ancs.next();
					if (!document.getLinksFromAnchor(anc).hasNext())
					{
						return anc;
					}
				}
				// We found no free Anchor, return null
				return null;
			}
		}
		else
		{
			if (inputNumber == 0)
			{
				return null;
			}
			else if (inputNumber == 1)
			{
				return getInputAnchor(0);
			}
			else
			{ //select the first free Anchor
				Iterator<Anchor> ancs = getInputAnchors();
				while (ancs.hasNext())
				{
					Anchor anc = ancs.next();
					if (!document.getLinksFromAnchor(anc).hasNext())
					{
						return anc;
					}
				}
				// We found no free Anchor, return null
				return null;
			}
		}
	}

	public int getInputNumber()
	{
		return inputNumber;
	}

	public void setInputNumber(int n)
	{
		// Remove some anchors and their links
		for (int i = inputAnchors.size() - 1; i >= n; i--)
		{
			Iterator<Link> linksToRemove = document.getLinksFromAnchor(inputAnchors.get(i));
			while (linksToRemove.hasNext())
			{
				document.removeLink(linksToRemove.next());
			}
			inputAnchors.remove(i);
			inputNumber--;
		}
		
		// Add anchors
		for (int i = inputAnchors.size(); i < n; i++)
		{
			AnchorConditioner anc = new AnchorConditioner(this, Anchor.AnchorType.ANCHORIN, i);
			inputAnchors.add(anc);
			inputNumber++;
		}
		
		if (display != null)
		{
			display.repaint();
		}

		_inputs.setSize(inputNumber);		
	}

	public int getOutputNumber()
	{
		return outputNumber;
	}

	public void setOutputNumber(int n)
	{
		// Remove some anchors and their links
		for (int i = outputAnchors.size() - 1; i >= n; i--)
		{
			Iterator<Link> linksToRemove = document.getLinksFromAnchor(outputAnchors.get(i));
			while (linksToRemove.hasNext())
			{
				document.removeLink(linksToRemove.next());
			}
			outputAnchors.remove(i);
			outputNumber--;
		}
		_outputs.setSize(outputNumber);
		// Add anchors
		for (int i = outputAnchors.size(); i < n; i++)
		{
			AnchorConditioner anc = new AnchorConditioner(this, Anchor.AnchorType.ANCHOROUT, i);
			outputAnchors.add(anc);
			outputNumber++;
			_outputs.add(new OutputInfo());
		}

		if (display != null)
		{
			display.repaint();
		}
	}

	public String toString()
	{
		return name;
	}

	public String getUsage()
	{
		String usage = baseName + " :\n";
		usage += "Description : " + description + "";

		usage += "\n\nOptions:\n";
		usage += options.toString();
		return usage;
	}

	public void toXML(XMLOutput output)
	{
		output.startEntity("conditioner");
		output.addAttribute("name", name);
		output.addAttribute("idname", getIDName());
		output.addAttribute("libname", baseName);
		if (variableInputNumber)
		{
			output.addAttribute("inputnumber", String.valueOf(inputNumber));
		}


		options.toXML(output);

		if (display != null)
		{
			output.startEntity("display");
			output.startEntity("x");
			output.addBodyText(String.valueOf(display.getX()));
			output.endEntity();

			output.startEntity("y");
			output.addBodyText(String.valueOf(display.getY()));
			output.endEntity();
			output.endEntity();
		}

		output.endEntity();

	}

	public boolean isVariableInputNumber()
	{
		return variableInputNumber;
	}

	public String getOutput(int number)
	{
		if (number >= outputNumber)
			return null;
		if (_outputs.get(number)._linked)
			return _inputs.get(number);
		return _outputs.get(number)._output;
	}

	public boolean isOutputEditable(int number)
	{
		if (number >= outputNumber)
			return false;
		return _outputs.get(number)._editable;
	}

	public boolean setInput(int number, String str)
	{
		//System.out.println( name + "input num " + number + "=" + str);
		if (number >= inputNumber || str == null || str.equals(""))
		{
			System.err.println("invalid args for setInput");
			return false;
		}
		_inputs.set(number, str );
		return true;
	}

	public boolean setOutput(int number, String str)
	{
		if (number >= outputNumber || str == null || str.equals(""))
			return false;
		if (_outputs.get(number)._linked || !_outputs.get(number)._editable )
			return false;
		_outputs.get(number)._output = str;
		return true;
	}

	public void addCloneDocument(String fromDoc, String toDoc)
	{
		cloneList.put(fromDoc, toDoc);
	}

	public void checkEntriesForExecution(Pipeline doc) throws Exception
	{
		Iterator<AnchorConditioner> ancIt = inputAnchors.iterator();
		while (ancIt.hasNext())
		{
			if (!document.getLinksFromAnchor(ancIt.next()).hasNext())
			{
				throw new EntryException("Error in " + getName() + ": not all inputs are plugged");
			}
		}
	}

	/**
	 * This method is used in case of variable input anchors number to check if a new anchor need
	 * to be added, and if so does it.
	 */
	public void checkAnchorNumber()
	{
		if (variableInputNumber)
		{
			Iterator<Anchor> ancs = getInputAnchors();
			while (ancs.hasNext())
			{
				Anchor anc = ancs.next();
				if (!document.getLinksFromAnchor(anc).hasNext())
				{
					return;
				}
			}
			// No free Anchor, we need to add one
			AnchorConditioner anc = new AnchorConditioner(this, Anchor.AnchorType.ANCHORIN, inputNumber);
			inputAnchors.add(inputNumber, anc);
			inputNumber++;
			if (display != null)
			{
				display.repaint();
			}
		}
	}

	public int execute()
	{
		Iterator<Map.Entry<String, String>> iter = cloneList.entrySet().iterator();
		while (iter.hasNext())
		{
			Map.Entry<String, String> entry = iter.next();
			refinery.getLibLoader().cloneDocument(entry.getKey(), entry.getValue());
		}
		cloneList.clear();

		if (variableInputNumber)
		{
			refinery.getLibLoader().setInputNumber(getIDName(), inputNumber);
		}
		for (int x = 0; x < inputNumber; x++)
		{
			refinery.getLibLoader().setInput(getIDName(), x, _inputs.get(x));
		}
		for (int x = 0; x < outputNumber; x++)
		{
			if ( _outputs.get(x)._editable )
			{
				refinery.getLibLoader().setOutput(getIDName(), x, _outputs.get(x)._output);
			}
		}

		Iterator<Option> oIter = options.getOptions();
		while (oIter.hasNext())
		{
			Option o = oIter.next();
			if (o.type == Option.OptionType.BOOL)
			{
				refinery.getLibLoader().setBoolOption(getIDName(), o.name, ((Boolean)o.getValue()).booleanValue());
			}
			else if (o.type == Option.OptionType.STRING)
			{
				refinery.getLibLoader().setStringOption(getIDName(), o.name, (String)o.getValue());
			}
			else
			{
				refinery.getLibLoader().setFloatOption(getIDName(), o.name, ((Float)o.getValue()).floatValue());
			}
		}

		return refinery.getLibLoader().execute(getIDName());
	}

	public void executeSolo(String[] args) throws ParsingErrorException
	{
		options.parse(args);

		//inputNumber = 1;
		execute();
	}

}
