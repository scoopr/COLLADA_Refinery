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

/**
 * The <code>Mark</code> is just a marker in the history when we want to pack everything from a point to another.
 * @author pdavid
 *
 */
public class Mark extends Action {

	public Mark(ActionHistory h) {
		super(h);
	}

	@Override
	public void undo() throws Exception {
	}

	@Override
	public void redo() throws Exception {
	}

}
