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

import java.awt.Point;


/**
 * The <code>AnchorConditioner</code> class is the version of Anchor specific to Conditioners.
 * @author Philippe David
 */
public class AnchorConditioner extends Anchor {
	protected ConditionerWrapper cw;
	// The Anchor position is the ConditionerWrapper
	private int number;
	
	public AnchorConditioner(ConditionerWrapper _cw, AnchorType _type, int _number){
		cw = _cw;
		type = _type;
		number = _number;
	}
	
	public RefererType getRefererType(){
		return RefererType.COND;
	}
	
	public ConditionerWrapper getReferer() {
		return cw;
	}
	
	public Point getPosition(){
		return cw.getDisplay().getAnchorPosition(this);
		/*
		int dx, dy;
		
		if (type == AnchorType.ANCHORIN){
			Point p = cw.getDisplay().getInputAnchorPosition(number);
			dx = p.x;
			dy = p.y;
		} else {
			//dy = (int) cw.getDisplay().getSize().getHeight() - 3;
			dy = (int) cw.getDisplay().getSize().getHeight();
			dx = (int) cw.getDisplay().getSize().getWidth()/2;
		}

		return new Point(cw.getDisplay().getX()+dx, cw.getDisplay().getY()+dy);
		*/
	}
	
	public int getNumber(){
		return number;
	}
	
	public String toString(){
		return cw.toString();
	}
}
