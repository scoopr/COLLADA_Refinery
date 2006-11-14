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
package refinery;

/**
 * The <code>Couple</code> class is a simple implementation of a couple of generic objects.
 * 
 * @author Philippe David
 *
 * @param <T1> Type of the first member
 * @param <T2> Type of the second member
 */
public class Couple<T1, T2> {
	public T1 first;
	public T2 second;
	
	public Couple(T1 _first, T2 _second){
		first = _first;
		second = _second;
	}
	
	public boolean equals(Couple c2){
		return (first.equals(c2.first) && second.equals(c2.second));
	}
	
	public String toString(){
		return "Couple "+first+" <-> "+second;
	}
}
