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
