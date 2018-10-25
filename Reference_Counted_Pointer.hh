/*	Reference_Counted_Pointer

PIRL CVS ID: $Id: Reference_Counted_Pointer.hh,v 1.22 2010/10/19 02:30:51 castalia Exp $

Copyright (C) 2009-2010  Arizona Board of Regents on behalf of the
Planetary Image Research Laboratory, Lunar and Planetary Laboratory at
the University of Arizona.

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License, version 2.1,
as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.


This implementation is based on the CountedPtr class in "The C++ Standard
Library", by Nicolai M. Josuttis.

*******************************************************************************/

#ifndef	_Reference_Counted_Pointer_
#define _Reference_Counted_Pointer_


#if defined (DEBUG)
//	DEBUG controls.
#define DEBUG_ALL		-1
#define	DEBUG_RCP		(1 << 14)

//	Default DEBUG value
#if (DEBUG +0 == DEBUG_RCP)
#define RCP_DEBUG DEBUG_RCP
#endif
#if (DEBUG +0 == DEBUG_ALL)
#define RCP_DEBUG -1
#endif

#include	<iostream>
#include	<iomanip>
using std::clog;
using std::endl;
#endif	//	DEBUG


namespace PIRL
{
/**	A <i>Reference_Counted_Pointer</i> acts like a normal pointer with
	the addition of reference counting.

	A Reference_Counted_Pointer associates a reference counter, allocated
	on the heap, with a copy of a normal pointer. The normal pointer may
	refer to any type of object other than an array, but the object must
	have been allocated on the heap (i.e. with the new method).

	Whenever a Reference_Counted_Pointer is copied, or assigned to
	another Reference_Counted_Pointer, both the pointer to the referenced
	object and the pointer to its reference counter are copied and the
	resulting shared reference count is incremented. Whenever a
	Reference_Counted_Pointer is destroyed, or assigned a
	Reference_Counted_Pointer or normal pointer that refers to a object
	different from the one currently being referenced, its reference
	count value is decremented and if the value becomes zero both the
	referenced object and the reference counter are deleted; otherwise
	both remain valid for any other Reference_Counted_Pointers using
	them. In this way a Reference_Counted_Pointer may be deleted as usual
	but if there are any other references to the shared object being
	referenced it will only be deleted when the last reference is
	deleted.

	A Reference_Counted_Pointer may be dereferenced, but it may not be
	altered (e.g. incremented or decremented).

	When sharing a normal pointer the issue of object ownership needs to
	be considered: Only the owner of the object is to delete it (it can
	only be deleted once). This creates a problem when the object is to
	be shared by multiple objects that will use the reference object:
	There can be only one owner so who deletes the object can be difficult
	to determine. The Reference_Counted_Pointer solves this problem by
	transferring ownership of the referenced object to a lightweight object
	that acts like a normal pointer to the object but keeps track of the
	number of shared references to the object, automatically deleting
	the object when there are no more external references to it. This
	"last one to leave turns out the lights" paradigm enables the object
	to be shared by many objects with no concern for ownership. In
	particular, a Reference_Counted_Pointer may be a data member of
	an object while the Reference_Counted_Pointer is copied to other
	objects and the referenced object will remain valid even if the
	original object where the referenced object was constructed is
	destroyed.
	
	The only concern is that the Reference_Counted_Pointer be
	constructed, or assigned, from its normal pointer to the shared
	object in a one-to-one relationship with the construction of the
	shared object. If the same normal pointer is used to construct, or be
	assigned to, more than one Reference_Counted_Pointer the
	Reference_Counted_Pointer objects become distinct - pointing to the
	same referenced object but using different reference counters - which
	creates a situation where one Reference_Counted_Pointer can delete
	the shared object while the other Reference_Counted_Pointers still
	hold a pointer to the now destroyed object. So equivalent, but
	distinct, Reference_Counted_Pointer objects should be avoided.
	Distinct Reference_Counted_Pointer objects can be bound together by
	assigning one to the other.

	This implementation is based on the CountedPtr class in "The C++
	Standard Library", by Nicolai M. Josuttis.

	@author		Bradford Castalia, UA/PIRL
	@version	$Revision: 1.22 $
*/
template <class T>
class Reference_Counted_Pointer
{
private:
T
	*Pointer;
long
	*Counter;


public:

/**	Construct a Reference_Counted_Pointer for a normal pointer.

	The pointer argument must refer to an object that was allocated on the
	heap (by the new operator or equivalent).

	<b>Caution</b>: When the same normal pointer is used to construct to
	more than one Reference_Counted_Pointer the two Reference_Counted_Pointers
	become {@link operator==(const Reference_Counted_Pointer&)const equivalent} but
	{@link is_distinct_from(const Reference_Counted_Pointer&)const distinct}. This
	should be avoided as it creates a situation where one
	Reference_Counted_Pointer can delete the object it refers to while
	the other Reference_Counted_Pointer still holds a reference to the
	deleted object.

	@param	pointer	A normal pointer to be reference counted.
*/
explicit Reference_Counted_Pointer (T* pointer = 0)
	:	Pointer (pointer),
		Counter (new long (1))
{
#ifdef RCP_DEBUG
clog << ">-< Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ") Constructed from pointer" << endl;
#endif
}

/**	Copy a Reference_Counted_Pointer.

	The new Reference_Counted_Pointer object shares the normal pointer
	and reference counter with the object that was copied. The reference
	count is incremented.

	@param	RCP	The Reference_Counted_Pointer to be copied.
*/
Reference_Counted_Pointer (const Reference_Counted_Pointer<T>& RCP)
	:	Pointer (RCP.Pointer),
		Counter (RCP.Counter)
{
++*Counter;
#ifdef RCP_DEBUG
clog << ">-< Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ") Copy of RCP @ " << (void*)&RCP << endl;
#endif
}

/**	Destroy this Reference_Counted_Pointer.

	The contents are disposed of: The reference count is decremented and
	if it becomes zero both the pointer and counter are deleted.
	<b>N.B.</b>: Deleting the pointer will destroy the object referred to
	by the normal pointer.
*/
~Reference_Counted_Pointer ()
{dispose ();}

/**	Assign another Reference_Counted_Pointer to this
	Reference_Counted_Pointer.

	Nothing is done if the Reference_Counted_Pointer to be assigned is
	the same object being assigned or is not distinct. Two
	Reference_Counted_Pointer objects are {@link
	is_distinct_from(const Reference_Counted_Pointer&)const distinct}
	when they refer to differenct objects - have different pointer values -
	or have different reference counters (counter objects, not their
	values).

	If the two Reference_Counted_Pointers are distinct and the object
	they reference is different then the contents
	of this object are first disposed of (decrementing the reference
	count and possibly deleting the currently referenced object) and then
	the assigned Reference_Counted_Pointer's contents are copied to this
	object's contents. The reference count is incremented on completion
	of the assignment. Note that the assigned Reference_Counted_Pointer
	object will, of course, see its reference count increment.

	<b>N.B.</b>: Two Reference_Counted_Pointers can be constructed from
	the same pointer value. But this should be avoided if at all possible
	as it creates a situation where the object referenced by the distinct
	Reference_Counted_Pointer objects may be deleted by one while the
	other still holds its reference. Thus a Reference_Counted_Pointer for
	an object should be constructed at the same point where the
	referenced object is constructed and then this initial
	Reference_Counted_Pointer used to contruct, or be assigned to, other
	Reference_Counted_Pointer objects as needed.

	@param	RCP	The Reference_Counted_Pointer to be assigned.
	@return	This Reference_Counted_Pointer.
	@see ~Reference_Counted_Pointer()
*/
Reference_Counted_Pointer<T>& operator=
	(const Reference_Counted_Pointer<T>& RCP)
{
#ifdef RCP_DEBUG
clog << ">>> Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ")::operator= (RCP& @ " << (void*)&RCP
		<< " (@" << (void*)RCP.Pointer << '#' << *RCP.Counter << "))" << endl;
#endif
if (this != &RCP)
	{
	if (Pointer != RCP.Pointer)
		{
		//	New reference object.
		#ifdef RCP_DEBUG
		clog << "    normal assignment" << endl;
		#endif
		dispose ();
		Pointer = RCP.Pointer;
		}
	else
	if (Counter != RCP.Counter)
		{
		/*
			Special case: Same reference object but different counter.

			Join with the other RCP by using its counter,
			but don't dispose of the Pointer; the reference object is
			not to be deleted even if this RCP Counter decrements to zero.
		*/
		if (--*Counter == 0)
			delete Counter;
		#ifdef RCP_DEBUG
		clog << "    same Pointer, different Counters!" << endl;
		#endif
		}
	Counter = RCP.Counter;
	++*Counter;
	#ifdef RCP_DEBUG
	clog << "    Counter = " << *Counter << endl;
	#endif
	}
#ifdef RCP_DEBUG
clog << "<<< Reference_Counted_Pointer operator= (RCP&)" << endl;
#endif
return *this;
}

/**	Assign a pointer to this Reference_Counted_Pointer.

	If the pointer has a different value than the pointer of this object,
	the contents of this object are first disposed of and then this
	object is reinitialized with the new pointer value and a new
	reference counter with a value of 1. Note that assigning a normal
	pointer to a Reference_Counted_Pointer that is alread bound to the
	same pointer does not increment the reference count.

	The pointer argument must refer to an object that was allocated on
	the heap (by the new operator or equivalent), but must not be an
	array.

	<b>Caution</b>: When the same normal pointer is assigned to more than
	one Reference_Counted_Pointer the two Reference_Counted_Pointers
	become {@link operator==(const Reference_Counted_Pointer&)const equivalent}
	but {@link is_distinct_from(const Reference_Counted_Pointer&)const distinct}.
	This should be avoided as it creates a situation where one
	Reference_Counted_Pointer can delete the object it refers to while
	the other Reference_Counted_Pointer still holds a reference to the
	deleted object.

	@param	pointer	A normal pointer to be reference counted. The pointer
		must refer to a non-array object that was allocated on the heap.
	@see	Reference_Counted_Pointer(T*)
*/
Reference_Counted_Pointer<T>& operator= (T* pointer)
{
#ifdef RCP_DEBUG
clog << ">>> Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ")::operator= (T* " << (void*)pointer << ")" << endl;
#endif
if (Pointer != pointer)
	{
	dispose ();
	Pointer = pointer;
	Counter = new long (1);
	#ifdef RCP_DEBUG
	clog << "    reset Pointer and Counter" << endl;
	#endif
	}
#ifdef RCP_DEBUG
else
	clog << "    same Pointer" << endl;
clog << "<<< Reference_Counted_Pointer::operator= (T*)" << endl;
#endif
return *this;
}

/**	Dereference this Reference_Counted_Pointer.

	@return	A reference to the object referenced by this
		Reference_Counted_Pointer.
*/
T& operator* () const
{
#if defined (RCP_DEBUG) && (RCP_DEBUG == DEBUG_ALL)
clog << ">-< Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ")::operator*" << endl;
#endif
return *Pointer;
}

/**	Access the pointer value of this Reference_Counted_Pointer.

	@return	A pointer to the object referenced by this
		Reference_Counted_Pointer.
*/
T* operator-> () const
{
#if defined (RCP_DEBUG) && (RCP_DEBUG == DEBUG_ALL)
clog << ">-< Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter
		<< ")::operator->" << endl;
#endif
return Pointer;
}

/**	Get the value of the pointer held by this Reference_Counted_Pointer.

	@return	The value of the pointer held by this
		Reference_Counted_Pointer.
*/
operator T* () const
{return Pointer;}

/**	Test if this Reference_Counted_Pointer has a pointer with a non-NULL
	value.

	@return	true if the value of this object's pointer is non-NULL; false
		otherwise.
*/
operator bool () const
{return Pointer != 0;}


/**	Test if this Reference_Counted_Pointer is equivalent to another.

	Two Reference_Counted_Pointers are equivalent if the value of their
	normal pointers are the same.

	<b>N.B.</b>: Two Reference_Counted_Pointers may be equivalent but also
	{@link is_distinct_from(const Reference_Counted_Pointer&)const distinct}
	from each other. This should be avoided.

	@param	RCP	A Reference_Counted_Pointer to be compared for equality
		with this one.
	@return	true if the normal pointers of both this and the other
		Reference_Counted_Pointer have the same value; false otherwise.
*/
bool operator== (const Reference_Counted_Pointer& RCP) const
{return Pointer == RCP.Pointer;}


/**	Test if this Reference_Counted_Pointer does not equal another.

	Two Reference_Counted_Pointers are not equal if the value of their
	normal pointers are different.

	@param	RCP	A Reference_Counted_Pointer to be compared for equality
		with this one.
	@return	true if the normal pointers of both this and the other
		Reference_Counted_Pointer have different values; false otherwise;
	@see	operator==(const Reference_Counted_Pointer&)const
*/
bool operator!= (const Reference_Counted_Pointer& RCP) const
{return Pointer != RCP.Pointer;}

/**	Test if this Reference_Counted_Pointer is distinct from another
	Reference_Counted_Pointer.

	Two Reference_Counted_Pointer objects are distinct when they refer to
	differenct objects - have different pointer values - or have
	different reference counters (counter objects, not their values).

	<b>N.B.</b>: Two Reference_Counted_Pointers can refer to the same
	object but still be distinct. This will happen if the two objects
	were constructed from the same normal pointer. When two
	Reference_Counted_Pointers are found to be
	{@link operator==(const Reference_Counted_Pointer&)const equivalent} and
	distinct from each other, {@link
	operator=(const Reference_Counted_Pointer&) assigning} one to
	the other will cause them to be bound together and no longer be
	distinct.

	@param	RCP	A Reference_Counted_Pointer to be compared for
		distinctness with this one.
	@return	true if the normal pointers of both this and the other
		Reference_Counted_Pointer have different values or the
		Reference_Counted_Pointers are using different reference
		counters for the same object; false otherwise;
*/
bool is_distinct_from (const Reference_Counted_Pointer& RCP) const
{return Pointer != RCP.Pointer || Counter != RCP.Counter;}


/**	Get the reference count.

	@return	The current number of references.
*/
long reference_count () const
{return *Counter;}


protected:
/**	Dispose of the Reference_Counted_Pointer contents if no longer in use.

	The reference count is decremented. If it becomes zero the reference
	counter and the object being referenced by the pointer are deleted.
*/
void dispose ()
{
#ifdef RCP_DEBUG
clog << ">-< Reference_Counted_Pointer @ " << (void*)this
		<< " (@" << (void*)Pointer << '#' << *Counter << ")::dispose" << endl;
#endif
if (--*Counter == 0)
	{
	delete Counter;
	if (Pointer)
		{
		#ifdef RCP_DEBUG
		clog << "    delete object @ " << (void*)Pointer << endl;
		#endif
		delete Pointer;
		Pointer = 0;
		}
	}
}
};	//	End of class Reference_Counted_Pointer


}	//	namespace PIRL
#endif	//	_Reference_Counted_Pointer_
