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
