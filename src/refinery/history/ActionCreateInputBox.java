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

import java.awt.Point;
import java.util.UUID;

import refinery.document.IOBox;

public class ActionCreateInputBox extends Action {
	private UUID ioID;
	private UUID displayID;
	private UUID anchOutID;
	private Point position;
	
	public ActionCreateInputBox(ActionHistory h, IOBox io){
		super(h);
		
		ioID = io.getUID();
		h.bigBag.put(ioID, io);
		
		anchOutID = io.getOutputAnchor(0).getUID();
		h.bigBag.put(anchOutID, io.getOutputAnchor(0));
		
		if(io.getDisplay() != null){
			displayID = io.getDisplay().getUID();
			h.bigBag.put(displayID, io.getDisplay());
			position = io.getDisplay().getPosition();
		} else {
			displayID = Identifiable.ZERO;
		}
	}
	
	@Override
	public void undo() throws Exception {
		history.document.removeInputBox((IOBox) history.getFromUID(ioID));

	}

	@Override
	public void redo() throws Exception {
		IOBox io = history.document.addInputBox();
		io.setUID(ioID);
		history.bigBag.put(ioID, io);

		if (! displayID.equals(Identifiable.ZERO)){
			io.getDisplay().setUID(displayID);
			history.bigBag.put(displayID, io.getDisplay());
			io.getDisplay().setPosition(position);
		}
		
		io.getOutputAnchor(0).setUID(anchOutID);
		history.bigBag.put(anchOutID, io.getOutputAnchor(0));
	}

}
