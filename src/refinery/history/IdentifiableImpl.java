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
package refinery.history;

import java.util.UUID;

/**
 * The <code>Identifiable</code> abstract class provides an unique id created randomly in
 * the constructor, and getter and setter on this id.
 * 
 * Note: Making this a class instead of an interface may cause problems because we cannot
 * make a class inherit from 2 classes at the same time in Java. But we have some code here,
 * so we factorize as long as this is not a problem for something else.
 * @author PDavid
 */

public abstract class IdentifiableImpl implements Identifiable{
	protected UUID id;
	
	
	public IdentifiableImpl(){
		id = UUID.randomUUID();
	}

	public UUID getUID() {
		return id;
	}

	public void setUID(UUID _id) {
		id = _id;
	}
}
