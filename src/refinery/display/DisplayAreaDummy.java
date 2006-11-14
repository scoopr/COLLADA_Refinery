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

import refinery.document.Pipeline;

public class DisplayAreaDummy implements DisplayAreaInterface {

	public void addDisplay(Display d) {}

	public void removeAllDisplay() {}

	public void removeDisplay(Display d) {}

	public void putDisplayOnTop(Display d) {}

	public void repaint() {}
	
	public void addDisplayAtRandomPosition(Display d){}
	
	public void setDocument(Pipeline d){}
	
	public Dimension getSize() {return new Dimension(0,0);}

	public void setDisplaySize(Dimension size) {}

	public void checkDisplaySize() {}
}
