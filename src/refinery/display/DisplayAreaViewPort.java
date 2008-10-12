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

import javax.swing.JViewport;

public class DisplayAreaViewPort extends JViewport {
	DisplayArea displayArea;
	public DisplayAreaViewPort(DisplayArea area){
		displayArea = area;
		setView(area);
	}
	
	public void setBounds(int x, int y, int w, int h) {
		super.reshape(x, y, w, h);
		
		Dimension newSize = null;
		Dimension areaSize = displayArea.getSize();
		if(w > areaSize.width){
			if (h > areaSize.height){ // width and height should be increased
				newSize = new Dimension(w, h);
			} else { // Increase only width
				newSize = new Dimension(w, areaSize.height);
			}
		} else if (h > areaSize.height){ // Increase only height
			newSize = new Dimension(areaSize.width, h);
		}
		
		if(newSize != null){
			//super.resize(newSize.width, newSize.height);
			displayArea.setDisplaySize(newSize);
			//revalidate();
		}
		
	}
}
