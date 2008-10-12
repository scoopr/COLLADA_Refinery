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
