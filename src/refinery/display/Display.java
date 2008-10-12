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
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.image.ImageObserver;

import javax.swing.JFrame;

import refinery.document.Anchor;
import refinery.history.ActionHistory;
import refinery.history.IdentifiableImpl;


/**
 * The <code>Display</code> abstract class is a common denominator to all objects that have
 * to be displayed on the BoxArea. It provides methods to draw, move, select objects as well
 * as access their referer.
 * 
 * @author Philippe David
 */
public abstract class Display extends IdentifiableImpl{
	public enum RefererType {IO, COND, MACRO, LINK};	// Possible types of referer: IOBox, Conditioner, Macro or Link
	protected RefererType refererType;			// Actual type of the referer
	protected ActionHistory history;			// Reference to the history
	
	public Display(RefererType _type, ActionHistory h){
		super();
		refererType = _type;
		history = h;
	}

	public abstract Point getPosition();
	
	public abstract Dimension getSize();
	
	/**
	 * Draw in the specified Graphic2D
	 * @param big Graphics2D to draw on
	 * @param io
	 */
	public abstract void drawOn(Graphics2D big, ImageObserver io); 
	
	/**
	 * Inform the Display that it is selected so that it can change its appareance
	 */
	public abstract void select();
	
	/**
	 * Inform the Display that it is selected so that it can change its appareance
	 * and get prepared to move relatively to the position (x, y)
	 */
	public abstract void select(int x, int y);
	
	/**
	 * Inform the Display that it is unselected so that it can change its appareance
	 */
	public abstract void deSelect();
	
	/**
	 * Check if the point (x, y) is in the Display surface
	 * @return True if the point is inside, false otherwise
	 */
	public abstract boolean contains(int x, int y);
	
	/**
	 * Chekc if the rectangle rect intersects with the Display surface
	 * @return True is the surfaces intersect, false otherwise
	 */
	public abstract boolean intersects(Rectangle rect);
	
	/**
	 * Move with the constraint to remain in the box. Require a previous call to select
	 * @param box The box the display has to stay into
	 */
	public abstract void mouseMove(int x, int y, Dimension box);
	
	/**
	 * @param p New position of the Display
	 */
	public abstract void setPosition(Point p);
	
	/**
	 * Find an anchor attached to this Display at the point (mx, my)
	 * @return The found Anchor, null if none found
	 */
	public abstract Anchor getAnchorAt(int mx, int my);
	
	/**
	 * Give the position of an anchor in a display box. The display might not be relative to a Box, this is a shortcut to access anchor
	 * in the case the referer is a box. If the referer is not a box, then the implementation must should return null in most cases.
	 * This is more efficient than using "instanceof Box" on a class that implements Display and then cast the referer to a Box and
	 * access the anchor. 
	 * @param a Anchor for which we want the position
	 * @return The position of a
	 */
	public abstract Point getAnchorPosition(Anchor a);
	
	/**
	 * Display the window to edit properties of the referer.
	 * @param parentWindow Parent to make the windows global from
	 */
	public abstract void displayPropertyWindows(JFrame parentWindow);

	/**
	 * @return The referer type: IOBox, Conditioner or Link
	 */
	public RefererType getRefererType() {
		return refererType;
	}
	
	/**
	 * @return The referer of this Display
	 */
	public abstract Object getReferer();
	
	public abstract void highlightLinkEnd(Anchor linkStart, MouseEvent event);
	public abstract void unHighlight();
	

}
