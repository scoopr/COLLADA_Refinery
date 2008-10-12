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

import refinery.history.IdentifiableImpl;


/**
 * The <code>Anchor</code> class represents an Anchor of a Conditioner or an IOBox. It is mainly 
 * used to represent where links are attached on these boxes (as an input or an output, and which
 *  number). It offers a common interface to IOBox and Conditioner anchors. 
 * @author Philippe David
 */
public abstract class Anchor extends IdentifiableImpl {
	public enum AnchorType {ANCHORIN, ANCHOROUT};
	public enum RefererType {IO, COND, MACRO};
	
	public AnchorType type;

	public AnchorType getType() {
		return type;
	}
	
	public abstract Point getPosition();
	
	public abstract RefererType getRefererType();

	public abstract String toString();
	
	public abstract Box getReferer();
	
	public abstract int getNumber();
}

