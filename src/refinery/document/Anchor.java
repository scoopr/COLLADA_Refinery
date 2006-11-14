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

