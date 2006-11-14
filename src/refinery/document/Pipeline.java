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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Vector;

import refinery.ConditionerTemplate;
import refinery.EntryException;
import refinery.MacroTemplate;
import refinery.Refinery;
import refinery.XMLOutput;
import refinery.display.DisplayAreaDummy;
import refinery.display.DisplayAreaInterface;
import refinery.display.MainGUI;
import refinery.document.Anchor.AnchorType;
import refinery.document.IOBox.IOBoxType;
import refinery.history.ActionHistory;
import refinery.output.ErrorAndWarningReport;
import refinery.output.ExecuteWindow;
import refinery.LibLoader;

/**
 * The <code>Pipeline</code> class represents a set of boxes and links with one input and one output.
 * This is the main class to represent our documents. Pipeline can also be nested into other pipelines,
 * thus allowing to reuse some preset pipelines called macros.
 * 
 * @author Philippe David
 */
public class Pipeline
{
	protected Refinery refinery;
	protected ActionHistory history;
	protected DisplayAreaInterface displayArea;

	protected String fullName;
	protected String description;
	private String saveFileName;

	protected Vector<ConditionerWrapper> conditionersList;
	//protected Vector<IOBox> ioList;
	protected Vector<PipelineMacro> macroList;
	protected LinkedList<IOBox> inputBoxes;
	protected LinkedList<IOBox> outputBoxes;
	protected int inputBoxNumber;
	protected int outputBoxNumber;
	protected LinksList linksList;
	protected ExecuteWindow exWin = null;

	protected boolean autoArrange;


	public Pipeline(Refinery _refinery)
	{
		refinery = _refinery;
		history = new ActionHistory(this);

		saveFileName = null;
		autoArrange = false;

		fullName = null;
		description = "No description set for this document";

		// Initialize lists
		conditionersList = new Vector<ConditionerWrapper>();
		//ioList = new Vector<IOBox>();
		macroList = new Vector<PipelineMacro>();
		linksList = new LinksList();

		// Create input and output with default values
		//inputBox = new IOBox(IOBox.IOBoxType.INPUTBOX, history);
		//outputBox = new IOBox(IOBox.IOBoxType.OUTPUTBOX, history);
		//ioList.add(inputBox);
		//ioList.add(outputBox);
		inputBoxes = new LinkedList<IOBox>();
		outputBoxes = new LinkedList<IOBox>();

		inputBoxNumber = 0;
		outputBoxNumber = 0;

		displayArea = new DisplayAreaDummy();
		//autoArrange = true;
	}

	public String getFullName()
	{
		return fullName;
	}

	public void setFullName(String fn)
	{
		fullName = fn;
		MainGUI gui = refinery.getGUI();
		if (gui != null)
		{
			gui.updateDocumentInfos();
		}
	}

	public String getDescription()
	{
		return description;
	}

	public void setDescription(String d)
	{
		description = d;
	}

	public ActionHistory getHistory()
	{
		return history;
	}

	public Refinery getRefinery()
	{
		return refinery;
	}

	public DisplayAreaInterface getDisplayArea()
	{
		return displayArea;
	}

	public void setDisplayArea(DisplayAreaInterface area)
	{
		displayArea = area;
		Iterator<IOBox> inIt = inputBoxes.iterator();
		while (inIt.hasNext())
		{
			inIt.next().setDisplayArea(area);
		}
		Iterator<IOBox> outIt = outputBoxes.iterator();
		while (outIt.hasNext())
		{
			outIt.next().setDisplayArea(area);
		}
		Iterator<ConditionerWrapper> condIt = conditionersList.iterator();
		while (condIt.hasNext())
		{
			condIt.next().setDisplayArea(area);
		}

		Iterator<PipelineMacro> macroIt = macroList.iterator();
		while (macroIt.hasNext())
		{
			macroIt.next().setSimpleDisplayArea(area);
		}

		Iterator<Link> linkIt = linksList.iterator();
		while (linkIt.hasNext())
		{
			linkIt.next().setDisplayArea(area);
		}
		area.repaint();
	}

	public boolean getAutoArrange()
	{
		return autoArrange;
	}

	public void setAutoArrange(boolean v)
	{
		autoArrange = v;
		if (v)
		{
			arrangeDisplays();
		}
		refinery.addDebugMessage("Auto organize set to " + v);
	}

	public boolean addLink(Link link)
	{
		boolean result;
		if (result = linksList.add(link))
		{
			//	If we could add that link to an input Anchor, we remove the previous one
			Anchor selectedAnchor = link.first;
			Anchor targetAnchor = link.second;
			if (targetAnchor.type == AnchorType.ANCHORIN)
			{ // the input anchor is the target
				Iterator<Link> previousLinksIt = getLinksFromAnchor(targetAnchor);
				while (previousLinksIt.hasNext())
				{
					Link previousLink = previousLinksIt.next();
					if (!previousLink.equals(link))
					{
						removeLink(previousLink);
					}
				}
			}
			else
			{ // the input is the link start point
				Iterator<Link> previousLinksIt = getLinksFromAnchor(selectedAnchor);
				while (previousLinksIt.hasNext())
				{
					Link previousLink = previousLinksIt.next();
					if (!previousLink.equals(link))
					{
						removeLink(previousLink);
					}
				}
			}

			history.addCreateLink(link);
			link.setDisplayArea(displayArea);
			displayArea.repaint();
		}

		if (autoArrange)
		{
			arrangeDisplays();
		}
		else if (refinery.getGUI() != null)
		{
			try
			{
				getCallLevels();
				refinery.getGUI().cleanStatusBar();
			}
			catch (GraphException e)
			{
				refinery.getGUI().setStatusBarError("Cycle detected !");
			}
		}

		return result;
	}

	public Iterator<Link> getLinksFromAnchor(Anchor anc)
	{
		return linksList.getLinksFromAnchor(anc);
	}

	public void removeLink(Link link)
	{
		history.addRemoveLink(link);
		displayArea.removeDisplay(link.getDisplay());
		displayArea.repaint();

		linksList.remove(link);

		if (autoArrange)
		{
			arrangeDisplays();
		}
		else
		{
			try
			{
				getCallLevels();
				refinery.getGUI().cleanStatusBar();
			}
			catch (GraphException e)
			{
				refinery.getGUI().setStatusBarError("Cycle detected !");
			}
		}

	}

	/**
	 * Returns the ConditionerWrapper that match the given name, or null if none found
	 */
	public ConditionerWrapper getConditionerWrapper(String cwName)
	{
		Iterator<ConditionerWrapper> condIT = conditionersList.iterator();
		while (condIT.hasNext())
		{
			ConditionerWrapper cw = condIT.next();
			if (cw.getName().equals(cwName))
			{
				return cw;
			}
		}
		return null;
	}

	public ConditionerWrapper addConditioner(ConditionerTemplate cb)
	{
		ConditionerWrapper cw = new ConditionerWrapper(cb, this, refinery, history);
		conditionersList.add(cw);

		cw.setDisplayArea(displayArea);
		history.addCreateConditioner(cw);
		return cw;
	}

	public void removeConditioner(ConditionerWrapper cw)
	{
		// We will probably removes link too, but we want to undo all that in once, so we pack the next actions
		history.startPackAll();

		Iterator<Anchor> ancs = cw.getInputAnchors();
		while (ancs.hasNext())
		{
			Iterator<Link> links = linksList.getLinksFromAnchor(ancs.next());
			while (links.hasNext())
			{
				removeLink(links.next());
			}
		}

		ancs = cw.getOutputAnchors();
		while (ancs.hasNext())
		{
			Iterator<Link> links = linksList.getLinksFromAnchor(ancs.next());
			while (links.hasNext())
			{
				removeLink(links.next());
			}
		}

		history.addRemoveConditioner(cw);
		history.endPackAll();
		// Now we can remove the conditioner
		displayArea.removeDisplay(cw.getDisplay());
		refinery.getLibLoader().removeConditioner(cw.getIDName());
		conditionersList.remove(cw);
		displayArea.repaint();
	}

	public PipelineMacro getMacro(String macroName)
	{
		Iterator<PipelineMacro> mIt = macroList.iterator();
		while (mIt.hasNext())
		{
			PipelineMacro m = mIt.next();
			if (m.getName().equals(macroName))
			{
				return m;
			}
		}
		return null;
	}

	public PipelineMacro addMacro(MacroTemplate mb) throws Exception
	{
		PipelineMacro macro = null;

		macro = new PipelineMacro(refinery, mb);
		macroList.add(macro);
		macro.setSimpleDisplayArea(displayArea);
		history.addCreateMacro(macro);

		return macro;
	}

	public void removeMacro(PipelineMacro macro)
	{
		// We will probably removes link too, but we want to undo all that in once, so we pack the next actions
		history.startPackAll();

		// We first remove the 2 possible links with this Conditioner
		Iterator<Anchor> ancs = macro.getInputAnchors();
		while (ancs.hasNext())
		{
			Iterator<Link> links = linksList.getLinksFromAnchor(ancs.next());
			while (links.hasNext())
			{
				removeLink(links.next());
			}
		}

		ancs = macro.getOutputAnchors();
		while (ancs.hasNext())
		{
			Iterator<Link> links = linksList.getLinksFromAnchor(ancs.next());
			while (links.hasNext())
			{
				removeLink(links.next());
			}
		}

		history.addRemoveMacro(macro);
		history.endPackAll();
		// Now we can remove the conditioner
		displayArea.removeDisplay(macro.getDisplay());
		macro.destroy();
		macroList.remove(macro);
		displayArea.repaint();
	}

	public IOBox addInputBox()
	{
		IOBox inputBox = new IOBox(IOBox.IOBoxType.INPUTBOX, history);
		inputBox.setNumber(inputBoxNumber);
		inputBoxNumber++;
		inputBoxes.add(inputBox);
		history.addCreateInputBox(inputBox);
		inputBox.setDisplayArea(displayArea);
		displayArea.repaint();
		return inputBox;
	}

	public IOBox addOuputBox()
	{
		IOBox outputBox = new IOBox(IOBox.IOBoxType.OUTPUTBOX, history);
		outputBox.setNumber(outputBoxNumber);
		outputBoxNumber++;
		outputBoxes.add(outputBox);
		history.addCreateOutputBox(outputBox);
		outputBox.setDisplayArea(displayArea);
		displayArea.repaint();
		return outputBox;
	}

	public void removeInputBox(IOBox io)
	{
		if (inputBoxes.contains(io))
		{
			history.startPackAll();
			Iterator<Link> links = linksList.getLinksFromAnchor(io.getOutputAnchor(0));
			while (links.hasNext())
			{
				removeLink(links.next());
			}
			history.addRemoveInputBox(io);
			history.endPackAll();
			displayArea.removeDisplay(io.getDisplay());
			inputBoxes.remove(io);
			int number = io.getNumber();
			Iterator<IOBox> boxIter = inputBoxes.iterator();
			while (boxIter.hasNext())
			{
				IOBox box = boxIter.next();
				if (box.getNumber() > number)
				{
					box.setNumber(box.getNumber() - 1);
				}
			}
			inputBoxNumber--;
			displayArea.repaint();
		}
	}

	public void removeOutputBox(IOBox io)
	{
		if (outputBoxes.contains(io))
		{
			history.startPackAll();
			Iterator<Link> links = linksList.getLinksFromAnchor(io.getInputAnchor(0));
			while (links.hasNext())
			{
				removeLink(links.next());
			}

			history.addRemoveOutputBox(io);
			history.endPackAll();
			displayArea.removeDisplay(io.getDisplay());
			outputBoxes.remove(io);
			int number = io.getNumber();
			Iterator<IOBox> boxIter = outputBoxes.iterator();
			while (boxIter.hasNext())
			{
				IOBox box = boxIter.next();
				if (box.getNumber() > number)
				{
					box.setNumber(box.getNumber() - 1);
				}
			}
			outputBoxNumber--;
			displayArea.repaint();
		}
	}

	/**
	 * checkEntries will make a several checks to garanty that filenames are set, file are readable or writable when they need to be so.
	 * Only boxes in the call list are checked, so unused (unlinked boxes) are not. It also check that the directory for temporary files is ok.
	 * @throws EntryException
	 */
	private void checkEntries(Vector<Box> callList) throws EntryException
	{
		ErrorAndWarningReport errorReport = new ErrorAndWarningReport();


		// Check boxes
		Iterator<Box> boxIt = callList.iterator();
		while (boxIt.hasNext())
		{
			Box box = boxIt.next();
			try
			{
				if (!(box instanceof IOBox && ((IOBox)box).getType() == IOBox.IOBoxType.OUTPUTBOX))
				{
					box.checkEntriesForExecution(this);
				}
			}
			catch (Exception e)
			{
				errorReport.addError(e.getMessage());
			}
		}

		//some outputs never get checked if they aren't in the call list. Explicitely check outputs.
		Iterator<IOBox> outIter = outputBoxes.iterator();
		while (outIter.hasNext())
		{
			try
			{
				outIter.next().checkEntriesForExecution(this);
			}
			catch (Exception e)
			{
				errorReport.addError(e.getMessage());
			}

		}

		if (errorReport.hasErrors())
		{
			errorReport.popupWindow(null, "Execution was aborted due to the previous errors.");
			throw new EntryException("Some pre-execution checks failed");
		}

		return;

	}

	/**
	 * getCallLevels parses the current graph from input boxes by following the links and build a structure to handle this graph,
	 * check for circular calls in the graph and make sub lists of boxes according to the dependencies. Each sub list contains Boxes that
	 * are dependant from the previous sub list. 
	 * Carefull : The very first box is null, it's a way to have a unique root and several inputs.
	 */
	protected Vector<Vector<GraphNode>> getCallLevels() throws GraphException
	{
		int currentLevel = 0;
		Vector<GraphNode> nextLevelNodes = new Vector<GraphNode>();


		// Create main list
		Vector<Vector<GraphNode>> nodesOrderedPerLevels = new Vector<Vector<GraphNode>>();
		// and add the first sub list
		nodesOrderedPerLevels.add(new Vector<GraphNode>());
		GraphNode graphRoot = new GraphNode(null, currentLevel, nodesOrderedPerLevels);
		currentLevel++;

		//For the moment we know we start from inputBoxes
		nodesOrderedPerLevels.add(new Vector<GraphNode>());
		Iterator<IOBox> inIt = inputBoxes.iterator();
		while (inIt.hasNext())
		{
			GraphNode newNode = new GraphNode(inIt.next(), currentLevel, nodesOrderedPerLevels);
			graphRoot.addLinkTo(newNode);
			nextLevelNodes.add(newNode);
		}

		// While we found new nodes to explore
		while (nextLevelNodes.size() > 0)
		{
			Iterator<GraphNode> currentLevelBoxesIt = nextLevelNodes.iterator();
			nextLevelNodes = new Vector<GraphNode>();
			currentLevel++;
			nodesOrderedPerLevels.add(new Vector<GraphNode>());

			// Explore all new Nodes
			while (currentLevelBoxesIt.hasNext())
			{
				GraphNode currentNode = currentLevelBoxesIt.next();

				Iterator<Anchor> ancIt = currentNode.box.getOutputAnchors();
				while (ancIt.hasNext())
				{
					Anchor currentAnchor = ancIt.next();

					Iterator<Link> links = getLinksFromAnchor(currentAnchor);
					while (links.hasNext())
					{
						Link link = links.next();
						Box newBox = link.second.getReferer();
						GraphNode linkedNode = graphRoot.findBox(newBox);
						if (linkedNode != null)
						{ // If this is a box we already had, just update its level
							linkedNode.changeLevel(currentLevel);
							currentNode.addLinkTo(linkedNode);

						}
						else
						{
							linkedNode = new GraphNode(newBox, currentLevel, nodesOrderedPerLevels);
							currentNode.addLinkTo(linkedNode);
							nextLevelNodes.add(linkedNode);

						}
					}
				}
			}
		}
		return nodesOrderedPerLevels;
	}

	protected Vector<Box> makeCallList() throws GraphException
	{
		//System.out.println("makecalllist()");
		Vector<Vector<GraphNode>> nodesOrderedPerLevels = getCallLevels();

		// Finally, build a list with a safe call order, and set the filenames
		Vector<Box> callList = new Vector<Box>();
		Iterator<Vector<GraphNode>> mainIT = nodesOrderedPerLevels.iterator();

		// Skip the root which is null
		mainIT.next();

		while (mainIT.hasNext())
		{
			Iterator<GraphNode> levelIt = mainIT.next().iterator();
			while (levelIt.hasNext())
			{
				GraphNode node = levelIt.next();
				
				if (node.box instanceof PipelineMacro )
				{
					((PipelineMacro)node.box).generateCallList();
				}

				// Set the outfile name for this box 
				int count = node.box.getOutputNumber();
				if (count == 0)
				{
					//System.out.println("no outputs to set! adding then skipping");
					//refinery.addDebugMessage(node.box.getName() + " at level " + node.level);
					//System.out.println(node.box.getName() + " at level " + node.level);

					callList.add(node.box);
					continue;
				}

				String output = null;
				for (int x = 0; x < count; x++)
				{
					output = node.box.getOutput(x);
					// Set properties for all input anchors linked to this output anchor
					Anchor outAnc = node.box.getOutputAnchor(x);
					Iterator<Link> links = getLinksFromAnchor(outAnc);
					int linkNum = 0;
					while (links.hasNext())
					{
						Link link = links.next();
						if (linkNum != 0)
						{
							String newOutput = output.substring(0, output.length() - 4) + linkNum + ".dae";
							link.second.getReferer().addCloneDocument(output, newOutput);
							output = newOutput;
						}
						link.second.getReferer().setInput(link.second.getNumber(), output);
						linkNum++;

						/*switch (link.second.getRefererType())
						{
						case MACRO:
							//TODO That's weak but we will delete that once we pass handle on the dom structure between conditioners (if we do it one day)
							PipelineMacro macro = (PipelineMacro)link.second.getReferer();
							if (this instanceof PipelineMacro)
							{
								macro.setExecutionPrefix(((PipelineMacro)this).prefix + node.level + "_macro_");
							}
							else
							{
								macro.setExecutionPrefix(node.level + "_macro_");
							}
						case IO:
							String outputFile = ((ConditionerWrapper)node.box).getOptionList().getValue("test");
							System.out.println("conditioner out file is " + outputFile);
							link.second.getReferer().getOptionList().setValue("inputfile", outputFile);
							break;
						case COND:
							link.second.getReferer().getOptionList().setValue("inputfile" + ((AnchorConditioner)link.second).getNumber(), outFileName);
							//int optionNum = ((AnchorConditioner) link.second).getReferer().getOptionList().getSize();
							//link.second.getReferer().getOptionList().setValue("inputfile"+ optionNum + 1, outFileName);
							break;
						}*/
					}
				}

				//refinery.addDebugMessage(node.box.getName() + " at level " + node.level);
				//System.out.println(node.box.getName() + " at level " + node.level);

				callList.add(node.box);
			}
		}

		return callList;
	}

	public int execute() throws ExecuteException
	{

		Vector<Box> callList;
		if (exWin != null)
		{
			exWin.dispose();
		}
		try
		{
			LibLoader.clearDOM();
			//System.out.println("Pipeline makecalllist");
			callList = makeCallList();
			checkEntries(callList);
			Iterator<Box> executeIt = callList.iterator();
			if (refinery.getGUI() != null)
			{
				exWin = new ExecuteWindow(refinery, refinery.getGUI().getWindow(), callList.size());
			}
			else
			{
				exWin = null;
			}

			while (executeIt.hasNext())
			{
				Box executingBox = executeIt.next();
				refinery.addNormalMessage("- Executing " + executingBox.getName());
				if (executingBox.execute() != 0)
				{
					throw new ExecuteException(executingBox.getName() + " returned an error :" + refinery.getLibLoader().getErrorMessage());
				}
				if (exWin != null)
				{
					if (exWin.stop)
					{
						throw new ExecuteException("User aborted");
					}
					exWin.addStep();
				}
			}
		}
		catch (Exception e)
		{
			refinery.addNormalMessage("EXECUTION STOPPED " + e.getMessage());
			if (exWin != null)
			{
				exWin.finish();
			}
			return -1;
		}

		refinery.addNormalMessage("\n*** EXECUTION SUCCESSFUL ***\n");
		return 0;
	}

	/**
	 * organizeDisplays will position the boxes in this document on the display.
	 *
	 */
	public void arrangeDisplays()
	{
		//TODO ask the displayArea to resize if necessary
		try
		{
			Vector<Vector<GraphNode>> nodesOrderedPerLevels = getCallLevels();
			// Get the root
			GraphNode root = nodesOrderedPerLevels.get(0).get(0);
			arrangeDisplays_sortChildrenPerLevel(root);

			root.computeXShift();
			//refinery.getGUI().cleanStatusBar();
		}
		catch (GraphException e)
		{
			refinery.getGUI().setStatusBarError("Cycle detected !");
		}

		displayArea.checkDisplaySize();
	}

	private void arrangeDisplays_sortChildrenPerLevel(GraphNode rootNode)
	{
		Iterator<GraphNode> childrenIt = rootNode.linksOut.iterator();

		Vector<GraphNode> sortedChildren = new Vector<GraphNode>();

		while (childrenIt.hasNext())
		{

			GraphNode child = childrenIt.next();

			Iterator<GraphNode> sortedChildrenIt = sortedChildren.iterator();
			int newPosition = 0;
			while (sortedChildrenIt.hasNext() && sortedChildrenIt.next().level >= child.level)
			{
				newPosition++;
			}
			sortedChildren.insertElementAt(child, newPosition);

			arrangeDisplays_sortChildrenPerLevel(child);
		}

		rootNode.linksOut = sortedChildren;
	}

	public void saveToFile(String filename) throws EntryException, FileNotFoundException, IOException
	{
		XMLOutput output = new XMLOutput();

		//TODO checkEntries still needed
		//checkEntries();
		File f = new File(filename);
		FileOutputStream fos = new FileOutputStream(f);

		// Header

		toXML(output, f.getName());

		fos.write(output.toString().getBytes());

		fos.close();

		saveFileName = filename;
	}

	public void toXML(XMLOutput output, String filename)
	{
		output.startEntity("pipeline");

		output.startEntity("fullname");
		if (fullName == null)
		{
			output.addBodyText(filename);
		}
		else
		{
			output.addBodyText(fullName);
		}
		output.endEntity();

		output.startEntity("description");
		output.addBodyText(description);
		output.endEntity();

		// Input box
		Iterator<IOBox> inIt = inputBoxes.iterator();
		while (inIt.hasNext())
		{
			inIt.next().toXML(output);
		}

		// Output Box
		Iterator<IOBox> outIt = outputBoxes.iterator();
		while (outIt.hasNext())
		{
			outIt.next().toXML(output);
		}

		// Little space
		output.newLine();

		// List of conditioners
		Iterator<ConditionerWrapper> condIT = conditionersList.iterator();
		while (condIT.hasNext())
		{
			condIT.next().toXML(output);
		}

		// Little space
		output.newLine();

		// List of macros
		Iterator<PipelineMacro> macroIt = macroList.iterator();
		while (macroIt.hasNext())
		{
			macroIt.next().toSimpleXML(output);
		}

		// Little space
		output.newLine();

		// List of links
		linksList.toXML(output);

		// Little space
		output.newLine();

		// Footer
		output.endEntity();

	}

	/**
	 * @return Returns the saveFileName.
	 */
	public String getSaveFileName()
	{
		return saveFileName;
	}

	/**
	 * @param saveFileName The saveFileName to set.
	 */
	public void setSaveFileName(String saveFileName)
	{
		this.saveFileName = saveFileName;
	}
}
