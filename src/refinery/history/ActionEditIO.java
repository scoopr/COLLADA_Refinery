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
