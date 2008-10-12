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

import java.awt.Point;
import java.util.UUID;

import refinery.document.IOBox;

public class ActionRemoveOutputBox extends Action {
	private UUID ioID;
	private UUID displayID;
	private UUID anchInID;
	private Point position;
	
	public ActionRemoveOutputBox(ActionHistory h, IOBox io){
		super(h);
		
		ioID = io.getUID();
		h.bigBag.put(ioID, io);
		
		anchInID = io.getInputAnchor(0).getUID();
		h.bigBag.put(anchInID, io.getInputAnchor(0));
		
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
		IOBox io = history.document.addOuputBox();
		io.setUID(ioID);
		history.bigBag.put(ioID, io);

		if (! displayID.equals(Identifiable.ZERO)){
			io.getDisplay().setUID(displayID);
			history.bigBag.put(displayID, io.getDisplay());
			io.getDisplay().setPosition(position);
		}
		
		io.getInputAnchor(0).setUID(anchInID);
		history.bigBag.put(anchInID, io.getInputAnchor(0));

	}

	@Override
	public void redo() throws Exception {
		history.document.removeOutputBox( (IOBox) history.getFromUID(ioID));
	}

}
