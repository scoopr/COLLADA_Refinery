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

import refinery.display.Display;


/** An <code>ActionEditIO</code> is the undoable action representing a Display move.
* 
* @author Philippe David
*/
public class ActionMoveDisplay extends Action {
	private UUID displayID;
	private int initialX;
	private int initialY;
	private int destinationX;
	private int destinationY;
	
	public ActionMoveDisplay(ActionHistory h, Display _d, int _initialX, int _initialY, int _destinationX, int _destinationY){
		super(h);
		
		h.bigBag.put(_d.getUID(), _d);
		
		displayID = _d.getUID();
		initialX = _initialX;
		initialY = _initialY;
		destinationX = _destinationX;
		destinationY = _destinationY;
	}

	public void undo() {
		Display display = (Display) history.getFromUID(displayID);
		display.setPosition(new Point(initialX, initialY));

	}

	public void redo() {
		Display display = (Display) history.getFromUID(displayID);
		display.setPosition(new Point(destinationX, destinationY));
	}
	
	public void setDestination(int newDestX, int newDestY){
		destinationX = newDestX;
		destinationY = newDestY;
	}

}
