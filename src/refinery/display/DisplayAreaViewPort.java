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
