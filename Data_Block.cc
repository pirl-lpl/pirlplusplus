/*	Data_Block

PIRL CVS ID: $Id: Data_Block.cc,v 1.34 2010/11/11 20:42:16 castalia Exp $

Copyright (C) 2003-2010  Arizona Board of Regents  on behalf of the
Planetary Image Research Laboratory,  Lunar and Planetary Laboratory at
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
*/
#include	"Data_Block.hh"
using namespace PIRL;

#include	<cstring>
using std::memcpy;

#include	<string>
using std::string;

#include	<algorithm>
using std::sort;

#include	<istream>
using std::istream;
#include	<ostream>
using std::ostream;
#include	<sstream>
using std::ostringstream;

#include	<iomanip>
using std::setw;
using std::endl;

#include	<stdexcept>
using std::logic_error;
using std::out_of_range;
using std::length_error;
using std::invalid_argument;

#if defined (DEBUG)
/*******************************************************************************
	DEBUG controls
*/
#define DEBUG_ALL				-1
#define DEBUG_CONSTRUCTORS		(1 << 0)
#define	DEBUG_ACCESSORS			(1 << 1)
#define	DEBUG_GET				(1 << 2)
#define DEBUG_PUT				(1 << 3)
#define	DEBUG_OFFSETS			(1 << 4)
#define DEBUG_COUNTS			(1 << 5)

#include	<iostream>
using std::clog;

using std::dec;
using std::hex;
using std::showbase;
using std::noshowbase;
using std::boolalpha;
using std::setfill;
using std::internal;
#endif	//	DEBUG

/*==============================================================================
	Constants:
*/
const char* const
	Data_Block::ID =
		"PIRL::Data_Block ($Revision: 1.34 $ $Date: 2010/11/11 20:42:16 $)";

const Data_Block::Index
	Data_Block::NO_INDEX = static_cast<Data_Block::Index>(-1);

#ifndef DOXYGEN_PROCESSING
namespace
{

#ifndef DATA_BLOCK_INITIAL_CAPACITY
#define DATA_BLOCK_INITIAL_CAPACITY			8192
#endif
const unsigned int
	INITIAL_CAPACITY						= DATA_BLOCK_INITIAL_CAPACITY;
}
#endif	//	DOXYGEN_PROCESSING

/*==============================================================================
	Constructors
*/
Data_Block::Data_Block ()
	:	Block (NULL)
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">-< Data_Block::constructor: (default)" << endl;
#endif
native (true);
}


Data_Block::Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements,
	const Index*	counts,
	bool			native_order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (native_order ? "" : "non-") << "native order" << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets, total_elements);
array_counts (counts);
native (native_order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	const Value_List&	counts,
	bool				native_order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (native_order ? "" : "non-") << "native order" << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
array_counts (counts);
native (native_order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	bool				native_order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (native_order ? "" : "non-") << "native order" << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
native (native_order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements,
	const Index*	counts,
	Data_Order		order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets, total_elements);
array_counts (counts);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	const Value_List&	counts,
	Data_Order			order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
array_counts (counts);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements,
	Data_Order		order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets, total_elements);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Data_Order		order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	Data_Order			order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}


Data_Block::Data_Block
	(
	void* const		data,
	const Index*	offsets,
	const Index		indexed_counts[][2],
	Data_Order		order
	)
	:	Block (reinterpret_cast<unsigned char*>(data))
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">>> Data_Block::constructor: "
	 << (order == MSB ? "MSB" : "LSB") << " order (" << order << ')' << endl
	 << "    data @ " << data << endl;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);

element_offsets (offsets);
array_indexed_counts (indexed_counts);
data_order (order);
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << "<<< Data_Block::constructor" << endl;
#endif
}

/*..............................................................................
	Copy/Assign
*/
Data_Block::Data_Block
	(
	const Data_Block&	data_block
	)
	:	Get		(data_block.Get),
		Put		(data_block.Put),
		Block	(data_block.Block),
		Offsets	(data_block.Offsets),
		Counts	(data_block.Counts),
		Native	(data_block.Native)
{
#if ((DEBUG) & DEBUG_CONSTRUCTORS)
clog << ">-< Data_Block::constructor: copy -" << endl
	 << data_block;
#endif
Offsets.reserve (INITIAL_CAPACITY);
Counts.reserve  (INITIAL_CAPACITY);
}

Data_Block&
Data_Block::operator=
	(
	const Data_Block&	data_block
	)
{
if (&data_block != this)
	{
	#if ((DEBUG) & DEBUG_CONSTRUCTORS)
	clog << ">-< Data_Block::constructor: copy -" << endl
		 << data_block;
	#endif
	Block	= data_block.Block;
	Offsets	= data_block.Offsets;
	Counts	= data_block.Counts;
	Native	= data_block.Native;
	Get		= data_block.Get;
	Put		= data_block.Put;
	}
return *this;
}

/*..............................................................................
	Destructor
*/
Data_Block::~Data_Block ()
{}

/*==============================================================================
	Accessors
*/
namespace
{
void
bubble_sort
	(
	Data_Block::Index*	offsets,
	Data_Block::Index	count
	)
{
if (count > 1)
	{
	Data_Block::Index
		*first = offsets,
		*last  = offsets + count,
		*bubble,
		*element;
	for (--last;
		 first != last;
		 --last)
		{
		bubble = first;
		do
			{
			element = bubble++;
			if (*bubble < *element)
				{
				Data_Block::Index
					value = *bubble;
				*bubble = *element;
				*element = value;
				}
			}
			while (bubble != last);
		}
	}
}


bool
multiple_of
	(
	const Data_Block::Index		size,
	const Data_Block::Index		count
	)
{return ((count < 2) || ((size / count) * count) == size);}

}	//	local namespace

/*------------------------------------------------------------------------------
	data order
*/
Data_Block&
Data_Block::native
	(
	bool	native_order
	)
{
#if ((DEBUG) & DEBUG_ACCESSORS)
clog << ">-< Data_Block::native: " << boolalpha << native_order << endl;
#endif
if ((Native = native_order))
	{
	Get = get_forwards;
	Put = put_forwards;
	}
else
	{
	Get = get_backwards;
	Put = put_backwards;
	}
return *this;
}


Data_Block::Data_Order
Data_Block::data_order () const
{
if (Native)
	return host_is_high_endian () ? MSB : LSB;
return host_is_high_endian () ? LSB : MSB;
}


Data_Block&
Data_Block::data_order
	(
	Data_Order	order
	)
{
#if ((DEBUG) & DEBUG_ACCESSORS)
clog << ">-< Data_Block::data_order: "
		<< ((order == MSB) ? "MSB" : "LSB")
		<< ", host is " << (host_is_high_endian () ? "MSB" : "LSB") << endl;
#endif
return native ((order == MSB) ?
	  host_is_high_endian () :
	! host_is_high_endian ());
}

/*------------------------------------------------------------------------------
	Data storage
*/
Data_Block&
Data_Block::data
	(
	void* const		block
	)
{
#if ((DEBUG) & DEBUG_ACCESSORS)
clog << ">-< Data_Block::data @ " << block << endl;
#endif
Block = reinterpret_cast<unsigned char*>(block);
return *this;
}

/*------------------------------------------------------------------------------
	Element offsets
*/
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	These methods determine the number of elements in the Data_BLock.
*/
bool
Data_Block::element_offsets
	(
	const Index*	offsets,
	Index			total_elements
	)
{
#if ((DEBUG) & DEBUG_OFFSETS)
clog << ">>> Data_Block::element_offsets: " << (int)total_elements << endl;
#endif
if (total_elements == NO_INDEX)
	{
	//	Count the offsets.
	total_elements = 0;
	if (offsets)
		{
		//	Skip the first element (it's probably 0);
		while (offsets[++total_elements]);
			
		if (total_elements == 1)
			{
			if (*offsets)
				{
				ostringstream
					message;
				message << ID << endl
						<< "Attempt to set element offsets with single offset "
							<< *offsets << endl
						<< "but no end-of-elements offset.";
				throw length_error (message.str ());
				}
			/*	
				Single empty element.

				This is a special case where a single element at offset 0
				is empty so it has an end-of-elements offset of 0, also.
				This is mistaken for the array end marker. There's no need
				to check for a third 0 value to confirm that a single emtpy
				element has been specified; the {0,0} array can only occur
				in that case.
			*/
			}
		else			
			//	Exclude the end-of-data offset.
			--total_elements;
		}
	}
if (! total_elements ||
	! offsets)
	{
	//	Empty the data block of elements.
	Offsets.clear ();
	Counts.clear ();
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    Empty." << endl
		 << "<<< Data_Block::element_offseets:" << endl;
	#endif
	return false;
	}

//	Possible change to the number of elements.
Offsets.resize (total_elements + 1);
Counts.resize (total_elements, (Index)1);

if (offsets != &Offsets[0])
	//	Copy in the new offset values.
	memcpy (&Offsets[0], offsets, (total_elements + 1) * sizeof (Index));

//	Sort them into increasing order.
sort (Offsets.begin (), Offsets.end ());

//	Reset the array counts that are no longer valid.
bool
	counts_reset = reset_counts (false);

#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    Offsets of " << Counts.size () << " elements:";
for (unsigned int
		count = 0;
		count < Counts.size ();
		count++)
	{
	if (count % 10)
		clog << ", ";
	else
		{
		if (count)
			clog << ',';
		clog << endl
			 << "    ";
		}
	clog << setw (4) << Offsets[count];
	}
clog << endl;
if (Counts.size ())
	clog << "    < " << Offsets[Counts.size ()] << endl;
clog << "<<< Data_Block::element_offsets: counts reset - "
		<< boolalpha << counts_reset << endl;
#endif
return counts_reset;
}


bool
Data_Block::element_offsets
	(
	const Value_List&	offsets
	)
{
if (offsets.size () == 1)
	throw length_error
		(
		string (ID) + '\n' +
		"Unable to set element offsets with a vector of only one value.");
return element_offsets (&offsets[0], offsets.size ());
}


bool
Data_Block::element_sizes
	(
	const Index*	sizes,
	Index			total_elements
	)
{
#if ((DEBUG) & DEBUG_OFFSETS)
clog << ">>> Data_Block::element_sizes: " << (int)total_elements << endl;
#endif
if (total_elements == NO_INDEX)
	{
	//	Count the sizes.
	total_elements = 0;
	if (sizes)
		while (sizes[total_elements])
		 	++total_elements;
	}
if (! total_elements ||
	! sizes)
	{
	//	Empty the data block of elements.
	Offsets.clear ();
	Counts.clear ();
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    Empty." << endl
		 << "<<< Data_Block::element_sizes:" << endl;
	#endif
	return false;
	}
#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    Sizes of " << total_elements << " elements:";
for (Index
		count = 0;
		count < total_elements;
		count++)
	{
	if (count % 10)
		clog << ", ";
	else
		{
		if (count)
			clog << ',';
		clog << endl
			 << "    ";
		}
	clog << setw (4) << sizes[count];
	}
clog << endl;
#endif

Index
	//	MSVC won't dynamically allocate a variable array on the stack.
	*array = new Index[total_elements + 1];
memcpy (array, sizes, total_elements * sizeof (Index));

bool
	counts_reset = element_offsets
		(sizes_to_offsets (array, total_elements), total_elements);

delete[] array;
#if ((DEBUG) & DEBUG_OFFSETS)
clog << "<<< Data_Block::element_sizes: counts reset - "
		<< boolalpha << counts_reset << endl;
#endif
return counts_reset;
}


bool
Data_Block::element_sizes
	(
	const Value_List&	sizes
	)
{return element_offsets (&sizes[0], sizes.size ());}


bool
Data_Block::insert_element
	(
	Index	offset,
	Index	size,
	Index	array_count
	)
{
if (array_count == 0)
	array_count = 1;

bool
	count_reset = false;
Value_List::iterator
	element = lower_bound (Offsets.begin (), Offsets.end (), offset);
Index
	index = element - Offsets.begin ();
if (index)
	{
	//	There is a preceeding element.
	if (index >= Counts.size ())
		{
		//	Append the new element.
		Offsets.push_back (offset + size);	//	New end-of_elements offset.
		Counts.push_back (array_count);
		if (index > Counts.size () ||	//	After end-of-elements (lengthen).
			*element != offset)			//	Not continguous append (shorten).
			{
			//	Reset former end-of-elements offset.
			if (index > Counts.size ())
				--index;
			Offsets[index] = offset;	//	New element offset.
			Counts[--index] = 1;	//	Reset the previous element array count.
			count_reset = true;
			}
		return count_reset;
		}
	if (*element != offset)
		{
		//	Not at an existing element offset.
		Counts[index - 1] = 1;	//	Reset the previous element array count.
		count_reset = true;
		}
	}

//	Insert the new element.
element = Offsets.insert (element, offset);

//	Shift the following elements up.
while (++element != Offsets.end ())
	*element += size;

//	Insert the new array count.
Counts.insert (Counts.begin () + index, array_count);

return count_reset;
}


Data_Block&
Data_Block::delete_element
	(
	Index	element
	)
{
if (element >= Counts.size ())
	{
	ostringstream
		message;
	message << ID << endl
			<< "Can't delete element " << element << " -" << endl
			<< "Invalid element (of " << Counts.size () << ").";
	throw out_of_range (message.str ());
	}

//	Delete the array count.
Counts.erase (Counts.begin () + element);

//	Size of the deleted element.
Index
	size = Offsets[element + 1] - Offsets[element];

//	Delete the element offset.
Offsets.erase (Offsets.begin () + element);

if (size)
	//	Shift the following elements down.
	while (element < Offsets.size ())
		Offsets[element++] -= size;

return *this;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Data_Block::Index
Data_Block::offset_of
	(
	Index	element
	)
	const
{
if (element < Counts.size ())
	return Offsets[element];
ostringstream
	message;
message << ID << endl
		<< "Can't get the offset of element " << element << " -" << endl
		<< "Invalid element (of " << Counts.size () << ").";
throw out_of_range (message.str ());
}


bool
Data_Block::offset_of
	(
	Index	element,
	Index	offset
	)
{
if (element >= Counts.size ())
	{
	ostringstream
		message;
	message << ID << endl
			<< "Can't set the offset of element " << element
			<< " to " << offset << " -" << endl
			<< "Invalid element (of " << Counts.size () << ").";
	throw out_of_range (message.str ());
	}
if (offset > Offsets[Counts.size ()])
	{
	ostringstream
		message;
	message << ID << endl
			<< "Can't set offset of element " << element
			<< " to " << offset << " -" << endl
			<< "Invalid offset (max " << Offsets[Counts.size ()] << ").";
	throw length_error (message.str ());
	}

Offsets[element] = offset;
sort (Offsets.begin (), Offsets.end ());
return reset_counts (true);
}


Data_Block&
Data_Block::shift_offsets
	(
	int		amount
	)
{
if (amount &&
	Counts.size ())
	{
	if (amount < 0 &&
		(Index)-amount > Offsets[0])
		{
		ostringstream
			message;
		message << ID << endl
				<< "Attempt to shift offsets by " << amount << endl
				<< "when the first offset is " << Offsets[0] << '.';
		throw invalid_argument (message.str ());
		}
	for (Value_List::iterator
			value = Offsets.begin ();
			value < Offsets.end ();
		  ++value)
		*value += amount;
	}
return *this;
}

/*------------------------------------------------------------------------------
	number of ...
*/
Data_Block::Index
Data_Block::size_of
	(
	Index	element
	)
	const
{
if (element < Counts.size ())
	return Offsets[element + 1] - Offsets[element];
ostringstream
	message;
message << ID << endl
		<< "Can't get the size of element " << element << " -" << endl
		<< "Invalid element (of " << Counts.size () << ").";
throw out_of_range (message.str ());
}


bool
Data_Block::size_of
	(
	Index	element,
	Index	size
	)
{
if (element >= Counts.size ())
	{
	ostringstream
		message;
	message << ID << endl
			<< "Can't set the size of element " << element
			<< " to " << size << " -" << endl
			<< "Invalid element (of " << Counts.size () << ").";
	throw out_of_range (message.str ());
	}
bool
	counts_reset = false;
int
	difference = size - (Offsets[element + 1] - Offsets[element]);
if (difference)
	{
	if (! size ||
		! multiple_of (size, Counts[element]))
		{
		//	The array value count became invalid; reset it.
		Counts[element] = 1;
		counts_reset = true;
		}
	//	Shift offsets of elements beyond the current element.
	while (++element <= Counts.size ())
		Offsets[element] += difference;
	}
return counts_reset;
}


Data_Block::Index
Data_Block::value_size_of
	(
	Index	element
	)
	const
{
if (element < Counts.size ())
	return (Offsets[element + 1] - Offsets[element]) / Counts[element];
ostringstream
	message;
message << ID << endl
		<< "Can't get value size of element " << element << " -" << endl
		<< "Invalid element (of " << Counts.size () << ").";
throw out_of_range (message.str ());
}


Data_Block&
Data_Block::value_size_of
	(
	Index	element,
	Index	size
	)
{
size_of (element, size * count_of (element));
return *this;
}


Data_Block::Index
Data_Block::index_of
	(
	Index	offset
	)
	const
{
//	Search using a binary tree algorithm.
Index
	low_index = 0,
	middle_index,
	half_length,
	length = Counts.size ();
while (length)
	{
	half_length = length >> 1;
	middle_index = low_index + half_length;
	if (Offsets[middle_index] == offset)
		return middle_index;
	else if (Offsets[middle_index] < offset)
		{
		low_index = ++middle_index;
		length -= ++half_length;
		}
	else
		length = half_length;
	}
return NO_INDEX;
}

/*------------------------------------------------------------------------------
	Array value counts
*/
Data_Block::Index
Data_Block::count_of
	(
	Index	element
	)
	const
{
if (element < Counts.size ())
	{
	if (Offsets[element + 1] - Offsets[element])
		return Counts[element];
	return 0;
	}
ostringstream
	message;
message << ID << endl
		<< "Can't get the count of element " << element << " -" << endl
		<< "Invalid element (of " << Counts.size () << ").";
throw out_of_range (message.str ());
}


Data_Block&
Data_Block::count_of
	(
	Index	element,
	Index	count
	)
{
if (element >= Counts.size ())
	{
	ostringstream
		message;
	message << ID << endl
			<< "Can't set the array count of element " << element
				<< " to " << count << '.' << endl
			<< "Invalid element (of " << Counts.size () << ").";
	throw out_of_range (message.str ());
	}
if (! multiple_of (Offsets[element + 1] - Offsets[element], count))
	{
	//	The count doesn't fit the element size.
	ostringstream
		message;
	message << ID << endl
			<< "Can't set the array count of element " << element << endl
			<< "because the element size of "
				<< (Offsets[element + 1] - Offsets[element]) << endl
			<< "is not a multiple of value count " << count << '.';
	throw length_error (message.str ());
	}
if (! (Counts[element] = count))
	   Counts[element] = 1;
return *this;
}


bool
Data_Block::reset_counts
	(
	bool	unconditional,
	Index	begin,
	Index	end
	)
{
if (end > Counts.size ())
	end = Counts.size ();
if (begin > end)
	{
	ostringstream
		message;
	message << ID << endl
			<< "Invalid array reset counts element range ["
				<< begin << ',' << end << ").";
	throw invalid_argument (message.str ());
	}
bool
	counts_reset = false;
if (unconditional)
	for (Index
			index = begin;
			index < end;
		  ++index)
		if (Counts[index] != 1)
			counts_reset = (Counts[index] = 1);
else
	for (Index
			index = begin;
			index < end;
		  ++index)
		if (! Counts[index] ||
			! multiple_of
				(Offsets[index + 1] - Offsets[index], Counts[index]))
			counts_reset = (Counts[index] = 1);
return counts_reset;
}


Data_Block&
Data_Block::array_counts
	(
	const Index*	counts,
	Index			begin,
	Index			end
	)
{
#if ((DEBUG) & DEBUG_COUNTS)
clog << ">>> Data_Block::array_counts: [" << begin << ',' << end << ')' << endl;
#endif
if (end > Counts.size ())
	end = Counts.size ();
if (begin > end)
	{
	ostringstream
		message;
	message << ID << endl
			<< "Invalid array counts element range ["
				<< begin << ',' << end << ").";
	throw invalid_argument (message.str ());
	}

if (! counts)
	{
	//	Reset counts.
	reset_counts (true, begin, end);
	#if ((DEBUG) & DEBUG_COUNTS)
	clog << "    Reset counts." << endl
		 << "<<< Data_Block::array_counts" << endl;
	#endif
	return *this;
	}

//	Confirm valid counts.
const Index
	*array = counts;
Index
	element;
for (element = begin;
	 element < end;
   ++element)
	{
	#if ((DEBUG) & DEBUG_COUNTS)
	clog << "    element " << element
			<< ", size = "
				<< setw (4) << (Offsets[element + 1] - Offsets[element])
			<< ", count = "
				<< setw (4) << *array << endl;
	#endif
	if (! multiple_of (Offsets[element + 1] - Offsets[element], *array++))
		{
		//	The count doesn't fit the element size.
		ostringstream
			message;
		message << ID << endl
				<< "Can't set array counts at element " << element << endl
				<< "because the element size of "
					<< (Offsets[element + 1] - Offsets[element]) << endl
				<< "is not a multiple of value count " << *(--array) << '.';
		throw length_error (message.str ());
		}
	}

//	Set the new counts.
array = counts;
for (element = begin;
	 element < end;
   ++element)
	if (! (Counts[element] = *array++))
		   Counts[element] = 1;
	
#if ((DEBUG) & DEBUG_COUNTS)
clog << "<<< Data_Block::array_counts" << endl;
#endif
return *this;
}


Data_Block&
Data_Block::array_counts
	(
	const Value_List&	counts,
	Index				begin
	)
{return array_counts (&counts[0], begin, begin + counts.size ());}


Data_Block&
Data_Block::array_offset_counts
	(
	const Index	offset_counts[][2],
	Index		total_counts
	)
{
#if ((DEBUG) & DEBUG_COUNTS)
clog << ">>> Data_Block::array_offset_counts: " << (int)total_counts << endl;
#endif
if (! offset_counts)
	{
	reset_counts (true);
	#if ((DEBUG) & DEBUG_COUNTS)
	clog << "    Reset counts." << endl
		 << "<<< Data_Block::array_offset_counts" << endl;
	#endif
	return *this;
	}

if (total_counts == NO_INDEX)
	for (total_counts = 0;
		 offset_counts[total_counts][ELEMENT_OFFSET] ||
		 offset_counts[total_counts][OFFSET_COUNT];
	   ++total_counts);
if (! total_counts)
	return *this;

Index
	//	MSVC won't dynamically allocate a variable array on the stack.
	*index = new Index[total_counts],
	count;
for (count = 0;
	 count < total_counts;
   ++count)
	{
	//	Get the element index for the offset.
	if ((index[count] = index_of (offset_counts[count][ELEMENT_OFFSET]))
		== NO_INDEX)
		{
		ostringstream
			message;
		message << ID << endl
				<< "Can't set the array count for the element with offset "
					<< offset_counts[count][ELEMENT_OFFSET] << endl
				<< "to array value count "
					<< offset_counts[count][OFFSET_COUNT] << endl
				<< "because there is no element at that offset.";
		throw invalid_argument (message.str ());
		}

	if (! multiple_of (Offsets[index[count] + 1] - Offsets[index[count]],
			offset_counts[count][OFFSET_COUNT]))
		{
		//	The count doesn't fit the element size.
		delete[] index;
		ostringstream
			message;
		message << ID << endl
				<< "Can't set the array count for element " << index[count]
					<< " at offset "
					<< offset_counts[count][ELEMENT_OFFSET] << endl
				<< "because the element size of "
					<< (Offsets[index[count] + 1] - Offsets[index[count]]) << endl
				<< "is not a multiple of value count "
					<< offset_counts[count][OFFSET_COUNT] << '.';
		throw length_error (message.str ());
		}
	}

//	Set the new counts.
for (count = 0;
	 count < total_counts;
   ++count)
	if (! (Counts[index[count]] = offset_counts[count][OFFSET_COUNT]))
		   Counts[index[count]] = 1;

delete[] index;
#if ((DEBUG) & DEBUG_COUNTS)
clog << "<<< Data_Block::array_offset_counts" << endl;
#endif
return *this;
}


Data_Block&
Data_Block::array_indexed_counts
	(
	const Index	indexed_counts[][2],
	Index		total_counts
	)
{
#if ((DEBUG) & DEBUG_COUNTS)
clog << ">>> Data_Block::array_indexed_counts: " << (int)total_counts << endl;
#endif
if (! indexed_counts)
	{
	reset_counts (true);
	#if ((DEBUG) & DEBUG_COUNTS)
	clog << "    Reset counts." << endl
		 << "<<< Data_Block::array_indexed_counts" << endl;
	#endif
	return *this;
	}
if (total_counts == NO_INDEX)
	for (total_counts = 0;
		 indexed_counts[total_counts][ELEMENT_INDEX] ||
		 indexed_counts[total_counts][INDEX_COUNT];
	   ++total_counts);
if (! total_counts)
	return *this;

Index
	index,
	count;
for (count = 0;
	 count < total_counts;
   ++count)
	{
	if ((index = indexed_counts[count][ELEMENT_INDEX]) >= Counts.size ())
		{
		ostringstream
			message;
		message << ID << endl
				<< "Can't set the array count for element "
					<< index << " -"  << endl
				<< "Invalid element (of " << Counts.size () << ").";
		throw out_of_range (message.str ());
		}

	if (! multiple_of (Offsets[index + 1] - Offsets[index],
			indexed_counts[count][INDEX_COUNT]))
		{
		//	The count doesn't fit the element size.
		ostringstream
			message;
		message << ID << endl
				<< "Can't set the array count for element " << index << endl
				<< "because the element size of "
					<< (Offsets[index + 1] - Offsets[index]) << endl
				<< "is not a multiple of value count "
					<< indexed_counts[count][INDEX_COUNT] << '.';
		throw length_error (message.str ());
		}
	}

//	Set the new counts.
for (count = 0;
	 count < total_counts;
   ++count)
	if (! (Counts[indexed_counts[count][ELEMENT_INDEX]]
			= indexed_counts[count][INDEX_COUNT]))
		   Counts[indexed_counts[count][ELEMENT_INDEX]] = 1;
#if ((DEBUG) & DEBUG_COUNTS)
clog << "<<< Data_Block::array_indexed_counts" << endl;
#endif
return *this;
}

/*==============================================================================
	I/O
*/
/*..............................................................................
	Static data movers.
*/
void
Data_Block::get_forwards
	(
	unsigned char*			host,			//	Destination: User Value
	int						host_amount,
	const unsigned char*	data,			//	Source: Data Block
	int						data_amount
	)
{
static bool
	HOST_IS_HIGH_ENDIAN = host_is_high_endian ();
#if ((DEBUG) & DEBUG_GET)
clog << ">>> Data_Block::get_forwards:" << endl
	 << "    host " << host_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)host
		<< ", data " << data_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)data << endl;
#endif
int
	difference = data_amount - host_amount;
if (difference == 0)
	{
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (data_amount--) *host++ = *data++;
	}
else if (difference > 0)
	{
	//	Host variable is smaller; only get data LSBs (down cast).
	if (HOST_IS_HIGH_ENDIAN)	//	Both host and data are MSB.
		//	Skip data MSBs.
		data += difference;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " LSBs: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (host_amount--) *host++ = *data++;
	}
else
	{
	//	Host variable is larger; pad MSBs with zeros (promote).
	if (HOST_IS_HIGH_ENDIAN)
		{
		//	First byte of host variable is MSB.
		#if ((DEBUG) & DEBUG_GET)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)host << endl;
		#endif
		do *host++ = 0;
		while (++difference);
		}
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (data_amount--) *host++ = *data++;
	//	Last byte of host is MSB?
	#if ((DEBUG) & DEBUG_GET)
	if (difference)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (difference++) *host++ = 0;
	}
#if ((DEBUG) & DEBUG_GET)
clog << "<<< Data_Block::get_forwards"
	 << noshowbase << setfill (' ') << dec << endl;
#endif
}

void
Data_Block::get_backwards
	(
	unsigned char*			host,			//	Destination: User Value
	int						host_amount,
	const unsigned char*	data,			//	Source: Data Block
	int						data_amount
	)
{
static bool
	HOST_IS_HIGH_ENDIAN = host_is_high_endian ();
#if ((DEBUG) & DEBUG_GET)
clog << ">>> Data_Block::get_backwards:" << endl
	 << "    host " << host_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)host
		<< ", data " << data_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)data << endl;
#endif
int
	difference = data_amount - host_amount;
if (difference == 0)
	{
	data += data_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (data_amount--) *host++ = *--data;
	}
else if (difference > 0)
	{
	//	Host variable is smaller; only get data LSBs (down cast).
	if (! HOST_IS_HIGH_ENDIAN)	//	Data is MSB.
		//	Skip data MSBs.
		data += difference;
	data += host_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " LSBs: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (host_amount--) *host++ = *--data;
	}
else
	{
	//	Host variable is larger; pad MSBs with zeros (promote).
	if (HOST_IS_HIGH_ENDIAN)
		{
		//	First byte of host is MSB.
		#if ((DEBUG) & DEBUG_GET)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)host << endl;
		#endif
		do *host++ = 0;
		while (++difference);
		}
	data += data_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)host
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (data_amount--) *host++ = *--data;
	//	Last byte of host is MSB?
	#if ((DEBUG) & DEBUG_GET)
	if (difference)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (difference++) *host++ = 0;
	}
#if ((DEBUG) & DEBUG_GET)
clog << "<<< Data_Block::get_backwards"
	 << noshowbase << setfill (' ') << dec << endl;
#endif
}

/**	Put data into the Block.
*/
void
Data_Block::put_forwards
	(
	unsigned char*			data,			//	Destination: Data Block
	int						data_amount,
	const unsigned char*	host,			//	Source: User Value
	int						host_amount
	)
{
static bool
	HOST_IS_HIGH_ENDIAN = host_is_high_endian ();
#if ((DEBUG) & DEBUG_GET)
clog << ">>> Data_Block::put_forwards:" << endl
	 << "    data " << data_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)data
		<< ", host " << host_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)host << endl;
#endif
int
	difference = host_amount - data_amount;
if (difference == 0)
	{
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (host_amount--) *data++ = *host++;
	}
else if (difference > 0)
	{
	//	Data size is smaller; only put host LSBs (down cast).
	if (HOST_IS_HIGH_ENDIAN)	//	Both host and data are MSB.
		//	Skip host MSBs.
		host += difference;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << data_amount << " LSBs: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (data_amount--) *data++ = *host++;
	}
else
	{
	//	Data size is larger; pad MSBs with zeros (promote).
	if (HOST_IS_HIGH_ENDIAN)
		{
		//	First byte of data is MSB.
		#if ((DEBUG) & DEBUG_GET)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)data << endl;
		#endif
		do *data++ = 0;
		while (++difference);
		}
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    +"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (host_amount--) *data++ = *host++;
	//	Last byte of data is MSB?
	#if ((DEBUG) & DEBUG_GET)
	if (difference)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (difference++) *data++ = 0;
	}
#if ((DEBUG) & DEBUG_GET)
clog << "<<< Data_Block::put_forwards"
	 << noshowbase << setfill (' ') << dec << endl;
#endif
}

/**	Put data into the Block.
*/
void
Data_Block::put_backwards
	(
	unsigned char*			data,			//	Destination: Data Block
	int						data_amount,
	const unsigned char*	host,			//	Source: User Value
	int						host_amount
	)
{
static bool
	HOST_IS_HIGH_ENDIAN = host_is_high_endian ();
#if ((DEBUG) & DEBUG_GET)
clog << ">>> Data_Block::put_backwards:" << endl
	 << "    data " << data_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)data
		<< ", host " << host_amount << " @ "
		<< setw (sizeof (void*) << 1) << (void*)host << endl;
#endif
int
	difference = host_amount - data_amount;
if (difference == 0)
	{
	host += host_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (host_amount--) *data++ = *--host;
	}
else if (difference > 0)
	{
	//	Data size is smaller; only put host LSBs (down cast).
	if (HOST_IS_HIGH_ENDIAN)
		//	Skip host MSBs.
		host += difference;
	host += data_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " LSBs: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (data_amount--) *data++ = *--host;
	}
else
	{
	//	Data size is larger; pad MSBs with zeros (promote).
	if (! HOST_IS_HIGH_ENDIAN)	//	Data is MSB.
		{
		//	First byte of data is MSB.
		#if ((DEBUG) & DEBUG_GET)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)data << endl;
		#endif
		do *data++ = 0;
		while (++difference);
		}
	host += host_amount;
	#if ((DEBUG) & DEBUG_GET)
	clog << "    " << host_amount << " move: +"
			<< setw (sizeof (void*) << 1) << (void*)data
			<< "    <-    -"
			<< setw (sizeof (void*) << 1) << (void*)host << endl;
	#endif
	while (host_amount--) *data++ = *--host;
	//	Last byte of data is MSB?
	#if ((DEBUG) & DEBUG_GET)
	if (difference)
		clog << "    " << -difference << " pads: +"
				<< setw (sizeof (void*) << 1) << (void*)data << endl;
	#endif
	while (difference++) *data++ = 0;
	}
#if ((DEBUG) & DEBUG_GET)
clog << "<<< Data_Block::put_backwards" << endl;
#endif
}

/*..............................................................................
	Stream data movers.
*/
istream&
Data_Block::input
	(
	std::istream&		stream
	)
{
#if ((DEBUG) & DEBUG_GET)
clog << ">-< Data_Block::input (istream)" << endl;
#endif
stream.read (reinterpret_cast<char*>(Block), size ());
return stream;
}

std::istream&
PIRL::operator>>=
	(
	std::istream&		stream,
	Data_Block&			data_block
	)
{return data_block.input (stream);}


ostream&
Data_Block::output
	(
	std::ostream&		stream
	)
	const
{
#if ((DEBUG) & DEBUG_PUT)
clog << ">-< Data_Block::output (ostream)" << endl;
#endif
stream.write (reinterpret_cast<char*>(Block), size ());
return stream;
}

std::ostream&
PIRL::operator<<=
	(
	std::ostream&		stream,
	const Data_Block&	data_block
	)
{return data_block.output (stream);}

/*..............................................................................
	Printing
*/
std::ostream&
Data_Block::print
	(
	std::ostream&		stream
	)
	const
{
unsigned int
	elements = Counts.size ();
stream
	<< "Data_Block @" << (void*)Block
		<< ", " << (data_order () == MSB ? "MSB" : "LSB")
		<< " (" << (Native ? "" : "non-") << "native) order" << endl
	<< elements << " element" << ((elements == 1) ? "" : "s") << endl;
if (elements)
	{
	stream
		<< "Element     Offset      Count" << endl
		<< "-------  ---------  ---------" << endl;
	for (unsigned int
			element = 0;
			element < elements;
		  ++element)
		stream
			<< setw (7)  << element
			<< setw (11) << Offsets[element]
			<< setw (11) << Counts[element] << endl;
	stream
		<< setw (7) << "Size" << setw (11) << Offsets[Counts.size ()] << endl;
	}
return stream;
}


std::ostream&
PIRL::operator<<
	(
	std::ostream&		stream,
	const Data_Block&	data_block
	)
{return data_block.print (stream);}

/*==============================================================================
	Utilities
*/
Data_Block::Index*
Data_Block::sizes_to_offsets
	(
	Index*	sizes,
	Index	total_elements
	)
{
#if ((DEBUG) & DEBUG_OFFSETS)
clog << ">>> Data_Block::sizes_to_offsets: " << (int)total_elements << endl;
#endif
if (! sizes)
	{
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    NULL sizes." << endl
		 << "<<< Data_Block::sizes_to_offsets" << endl;
	#endif
	return sizes;
	}
if (total_elements == NO_INDEX)
	for (total_elements = 0;
		 sizes[total_elements];
	   ++total_elements);
if (! total_elements)
	{
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    0 sizes." << endl
		 << "<<< Data_Block::sizes_to_offsets" << endl;
	#endif
	return sizes;
	}
	
#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    " << total_elements << " sizes:" << endl;
#endif
Index
	size,
	offset = 0;
for (Index
		index = 0;
		index < total_elements;
	  ++index)
	{
	size = sizes[index];
	sizes[index] = offset;
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    offset " << setw (4) << offset
		 << ", size " << setw (4) << size << endl;
	#endif
	offset += size;
	}
sizes[total_elements] = offset;	//	end-of-data offset.
#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    offset " << setw (4) << offset << " is end-of-elements" << endl
	 << "<<< Data_Block::sizes_to_offsets" << endl;
#endif
return sizes;
}


Data_Block::Value_List&
Data_Block::sizes_to_offsets
	(
	Value_List&	sizes
	)
{
sizes.push_back (0);
sizes_to_offsets (&sizes[0], sizes.size () - 1);
return sizes;
}


Data_Block::Index*
Data_Block::offsets_to_sizes
	(
	Index*	offsets,
	Index	total_elements
	)
{
#if ((DEBUG) & DEBUG_OFFSETS)
clog << ">>> Data_Block::offsets_to_sizes: " << (int)total_elements << endl;
#endif
if (! offsets)
	{
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    NULL sizes." << endl
		 << "<<< Data_Block::offsets_to_sizes" << endl;
	#endif
	return offsets;
	}
if (total_elements == NO_INDEX)
	{
	//	Count the offsets.
	total_elements = 0;
	//	Skip the first element (it's probably 0);
	while (offsets[++total_elements]);
	if (! --total_elements)
		throw length_error
			(
			string (ID) + '\n' +
			"Can't convert offsets to sizes -\n"
			"Only one offset value (no end-of-elements offset)."
			);
	}
if (! total_elements)
	{
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    0 offsets." << endl
		 << "<<< Data_Block::offsets_to_sizes" << endl;
	#endif
	return offsets;
	}

//	Sort the offset values; include the end-of-elements offset.
bubble_sort (offsets, total_elements + 1);

#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    " << total_elements << " offsets:" << endl;
#endif
//	Convert the offset values to size values.
for (Index
		index = 0;
		index < total_elements;
	  ++index)
	{
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << "    offset " << setw (4) << offsets[index];
	#endif
	offsets[index] = offsets[index + 1] - offsets[index];
	#if ((DEBUG) & DEBUG_OFFSETS)
	clog << ", size " << setw (4) << offsets[index] << endl;
	#endif
	}
#if ((DEBUG) & DEBUG_OFFSETS)
clog << "    offset " << setw (4) << offsets[total_elements]
		<< " is end-of-elements" << endl
	 << "<<< Data_Block::offsets_to_sizes" << endl;
#endif
offsets[total_elements] = 0;	//	Mark the end of the sizes array.
return offsets;
}


Data_Block::Value_List&
Data_Block::offsets_to_sizes
	(
	Value_List&	offsets
	)
{
if (offsets.size () > 1)
	{
	offsets_to_sizes (&offsets[0], offsets.size () - 1);
	offsets.pop_back ();	//	Remove the extra value.
	}
else if (offsets.size ())
	throw length_error
		(
		string (ID) + '\n' +
		"Can't convert offsets vector to sizes vector -\n"
		"Only one value present (no end-of-elements offset)."
		);
return offsets;
}


void
Data_Block::limits_checker
	(
	const Index		element,
	const Index		entry,
	bool			getter,
	bool			array
	)
	const
{
if (! Block)
	{
	ostringstream
		message;
	message << ID << '\n'
			<< (getter ? "Get" : "Put")
			<< (array ? " array" : " ")
			<< " element " << element
			<< " attempted without any data storage available.";
	throw logic_error (message.str ());
	}
if (element >= Counts.size ())
	{
	ostringstream
		message;
	message << ID << '\n'
			<< (getter ? "Get" : "Put")
			<< (array ? " array" : " ")
			<< " element " << element << " exceeds the limit of "
			<< Counts.size () << " elements.";
	throw out_of_range (message.str ());
	}
if (entry >= Counts[element])
	{
	ostringstream
		message;
	message << ID << '\n'
			<< (getter ? "Get" : "Put") 
			<< " array " << (array ? "count" : "index")
			<< ' ' << entry << " exceeds the limit of "
			<< Counts[element] << " entries for element " << element << '.';
	throw out_of_range (message.str ());
	}
}
