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
package refinery.history;

import java.util.UUID;

import refinery.document.IOBox;


/**
 * An <code>ActionEditIO</code> is the undoable action representing a IOBox edition.
 * 
 * @author Philippe David
 */
public class ActionEditIO extends Action {
	private UUID ioID;
	String initialName;
	String initialFilePath;
	String newName;
	String newFilePath;
		
	public ActionEditIO(ActionHistory h, IOBox io, String _initialName, String _initialFilePath, String _newName, String _newFilePath){
		super(h);
		
		h.bigBag.put(io.getUID(), io);
		
		ioID = io.getUID();
		initialName = _initialName;
		initialFilePath = _initialFilePath;
		newName = _newName;
		newFilePath = _newFilePath;
	}
	
	@Override
	public void undo() {
		IOBox io =  (IOBox) history.getFromUID(ioID);
		
		io.setName(initialName);
		io.setFilename(initialFilePath);
	}

	@Override
	public void redo() {
		IOBox io =  (IOBox) history.getFromUID(ioID);
		
		io.setName(newName);
		io.setFilename(newFilePath);
	}
}
