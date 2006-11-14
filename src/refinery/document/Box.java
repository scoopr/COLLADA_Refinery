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

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;

import refinery.OptionList;
import refinery.display.Display;
import refinery.display.DisplayAreaInterface;
import refinery.document.Anchor.AnchorType;


/**
 * The <code>Box</code> interface contains the basic methods every 
 * object manageable in the pipeline must implement.
 * 
 * @author Philippe David
 */


public interface Box {

	/**
	 * This method implements the policy describe in <code>DisplayArea.mouseReleased</code> which select
	 * an Anchor to end a link. The Anchor may already be linked with something else.
	 * @param type of the Anchor the method will return
	 * @return The Anchor to end the link with if there is one, null otherwise
	 */
	public Anchor getLinkEndAnchor(AnchorType type);
	
	/**
	 * @return An iterator over the input anchors
	 */
	public Iterator<Anchor> getInputAnchors();
	
	/**
	 * @return The input Anchor if it exists, null otherwise
	 */
	public Anchor getInputAnchor(int number);
	
	/**
	 * @return An Iterator over the output anchors
	 */
	public Iterator<Anchor> getOutputAnchors();
	
	/**
	 * @param anchorNumber TODO
	 * @return The output Anchor if it exists, null otherwise
	 */
	public Anchor getOutputAnchor(int anchorNumber);
	
	/**
	 * @return The box display
	 */
	public Display getDisplay();
	
	/**
	 * Sets a display area and register display(s) into it.
	 */
	public void setDisplayArea(DisplayAreaInterface area);
	
	public String getName();
	public void setName(String newName);
	
	public String getIDName();
	
	//public void setFileOption(String option, String value);
	public OptionList getOptionList();

	public int getOutputNumber();
	public int getInputNumber();

	public String getOutput(int number);

	public boolean setInput(int number, String str);

	public boolean setOutput(int number, String str);

	public void addCloneDocument(String fromDoc, String toDoc);
	/**
	 * Execute this box with current parameters set.
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public int execute() throws ExecuteException ;
	
	public void checkEntriesForExecution(Pipeline doc) throws Exception;
	
}
