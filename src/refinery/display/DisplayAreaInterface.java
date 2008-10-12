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
package refinery.display;

import java.awt.Dimension;

import refinery.document.Pipeline;

public interface DisplayAreaInterface {

	/**
	 * Adds a display on the top of the display list.
	 */
	public abstract void addDisplay(Display d);

	/**
	 * Empty the DisplayList
	 */
	public abstract void removeAllDisplay();

	/**
	 * Remove only one Display from the DisplayList
	 * @param d Display to be removed
	 */
	public abstract void removeDisplay(Display d);

	/**
	 * Raise a Display on top of the DisplayList. In case of a Conditioner or an IOBox,
	 * we raise its links Displays too.
	 * @param d
	 */
	public abstract void putDisplayOnTop(Display d);
	
	/**
	 * Repaint this component.
	 */
	public abstract void repaint();
	
	/**
	 * Add a Display object and set its position at random within the area
	 */
	public abstract void addDisplayAtRandomPosition(Display d);
	
	public abstract void setDocument(Pipeline d);
	
	public abstract Dimension getSize();
	
	public abstract void setDisplaySize(Dimension size);
	
	public abstract void checkDisplaySize();

}
