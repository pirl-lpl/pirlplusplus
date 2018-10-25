/*	Data_Block

PIRL CVS ID: $Id: Data_Block.hh,v 1.37 2007/02/01 06:08:20 castalia Exp $

Copyright (C) 2003, 2004, 2005  Arizona Board of Regents 
on behalf of the Planetary Image Research Laboratory, 
Lunar and Planetary Laboratory at the University of Arizona.

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
#ifndef _Data_Block_
#define _Data_Block_

#include	"endian.hh"

#include	<iostream>
#include	<vector>

/**	The Planetary Image Research Laboratory.

The Planetary Image Research Laboratory (http://PIRL.LPL.Arizona.edu/)
is located at the University of Arizona in the  Department of
Planetary Sciences and Lunar and Planetary Laboratory
(http://www.lpl.arizona.edu/).
*/
namespace PIRL
{
/*=****************************************************************************
	Data_Block
*/
/**	A <i>Data_Block</i> manages access to a block of binary data bytes.

	The binary data structure is described by the starting address of a
	suitable data storage area and the byte offset from this address to
	the first byte of each <i>element</i> of the structure, plus an
	end-of-elements offset to the byte following the last element.

	The data storage area for the binary block of data is owned by the
	user, not the Data_Block. It is the responsibility of the user to
	provide sufficient storage for all elements that will be accessed.

	All elements defined for the data block are contiguous. The first
	element of the data block is not required to be located at the
	beginning of the data storage area, nor is the last element required
	to end at the end of the storage area.

	Each element offset location in the data storage area is maintained
	in an ordered (lowest to highest) list that is referenced by an
	element <i>Index</i>. Typically the application uses a simple enum
	list to provide element index values symbollically.

	Optionally, any element of the structure may be described as an
	array of equal sized values by providing a count of the number of
	values in the element. Element array values may be accessed as a
	group by their element index, or individually by also providing the
	element array value index.

	The element binary values in the user provided storage may be be
	specified to be byte ordered (their endianness) differently from the
	native host order, in which case data transfers between the data
	block and application variables will reverse the byte order.

	Typically a binary data block is managed by a struct definition.
	The Data_Block is not a substitute for a struct. The Data_Block
	offers special capabilities:

<dl>
<dt><b>Byte order independence</b>
	<dd>The block of data being managed may contain values that are byte
	ordered differently than the host system where the values will be
	used. Data transfers between the data block and application variables
	will be reordered appropriately. This capability is especially useful
	when the byte ordering of a data block read from an external source
	should be hidden from the application using the data. For example,
	the data may have been written on a host architecture with a specific
	byte ordering, but the application using the data needs to use the
	values consistently on different host architectures. The application
	may specify the byte ordering of the data set to the Data_Block. For
	any access to an element value data ordering is transparent to the
	application; binary values are always provided in the native order of
	the host system. An application may also specify the data order
	before creating and writing a data set to ensure a specific external
	order.

<dt><b>Byte alignment independence</b>
	<dd>The block of data being managed is not constrained by the byte
	alignment requirements of the host architecture. This is a
	significant, and usually hidden, problem with struct definitions of
	external data blocks. Odd data sizes - e.g. three byte integers -
	and odd data locations - e.g. four byte integers that do not begin
	at an even address - are acceptable. Also values in Elements that
	are a different size than the application variables that are to
	receive them will be correctly coerced and aligned during transfers
	between data block and variable storage.

<dt><b>Dynamic structure definition</b>
	<dd>The structure of the block of data being managed is defined at
	run time rather than compile time. It may be that information in
	the data itself, or from an external source, dictates an alteration
	in the presumed structure of a block of data. It is also possible
	that the structure of a block of data may be obtained from an
	external description or user interaction.

<dt><b>Multiple data views</b>
	<dd>Any block of data may have more than one Data_Block structure
	associated with it. This is conceptually similar to a union. For
	example, one Data_Block might provide Elements with four byte
	integers while another provides an array of single bytes for the
	same block of data, and yet another provides different array
	organization or integer sizes.

<dt><b>Data memory independence</b>
	<dd>The block of data being managed is not owned by the Data_Block.
	The memory containing the block of data is supplied by the
	application. For example, the location of the block of data might be
	dynamically allocated storage, a memory mapped disk file, or an
	address on system hardware. At any time the Data_Block may be
	redirected to a different data storage location without altering the
	data structure description owned by the Data_Block.
</dl>

@author		Bradford Castalia, UA/PIRL, 12/03

$Revision: 1.37 $ 
*/
class Data_Block
{
public:
/*==============================================================================
	Types
*/
//!	Element index, offset and array count values.
typedef unsigned int		Index;

//!	Value list.
typedef std::vector<Index>	Value_List;

//!	Pointer to a function that copies bytes from a source to a destination.
typedef void (*Copier)
	(
	unsigned char*			destination,
	int						destination_amount,
	const unsigned char*	source,
	int						source_amount
	);

/*==============================================================================
	Constants
*/
//!	Class identification name with source code version and date.
static const char* const
	ID;

/**	Data order specification.

	Multi-byte data values are ordered with either the Most or the
	Least Significant Byte as the first accessed (lowest address). If a
	multi-byte datum (e.g. an int) has the value of 1, then first byte
	of LSB data will also have a value of 1 or the first byte of MSB
	data will have a value of 0.
*/
enum Data_Order
	{
	MSB,
	LSB
	};

//!	An Index value (-1) meaning an invalid or non-existent Index value.
static const Index
	NO_INDEX;

/**	Symbolic references to element index and value count pairs.

	The count of values in array elements may be defined by element
	index and value count pairs. This enum helps make it clear which is
	which.

	@see	array_indexed_counts(const Index[][2], Index, bool)
*/
enum Array_Index_Counts
	{
	ELEMENT_INDEX,
	INDEX_COUNT
	};
//!	For backwards compatibility (deprecated).
#define INDEX_COUNTS INDEX_COUNT

/**	Symbolic references to element offset and value count pairs.

	The count of values in array elements may be defined by element
	offset and value count pairs. This enum helps make it clear which
	is which.

	@see	array_offset_counts(const Index[][2], Index, bool)
*/
enum Array_Offset_Counts
	{
	ELEMENT_OFFSET,
	OFFSET_COUNT
	};
//!	For backwards compatibility (deprecated).
#define OFFSET_COUNTS OFFSET_COUNT

/**	Element access limits checking.

	Define the DATA_BLOCK_LIMITS_CHECK macro to be true if Element
	access limits checking is to be applied by default.
*/
#ifndef DATA_BLOCK_LIMITS_CHECK
#define DATA_BLOCK_LIMITS_CHECK	false
#endif

/*==============================================================================
	Constructors
*/
/**	Constructs an empty Data_Block.

	<b>Warning</b>: This Data_Block will not have any {@link data(void* const)
	data} storage area associated with it.

	The data order mode will be set to {@link native(bool) native}
	host order.
*/
Data_Block ();

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and, optionally, array value counts.

	The byte order of the data block values may be specified as
	non-native.

	@param	data	The address of the data block to be managed.
	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure, plus the offset to the next byte after the last
		element of the structure. For an alternative method of describing
		the data block element locations see #element_sizes.
	@param	total_elements	The number of elements in the data block. If
		the total_elements argument is not provided, then the final entry
		(after the end-of-elements offset) in the offsets array must be
		0, and the total_elements value will be determined by counting,
		from the second offsets entry (because the first is expected to
		be 0), the number of non-0 offsets array entries and subtracting
		one for the final end-of-elements offset.
	@param	array_counts	A list of array value counts for each element
		in the data block. This list corresponds to the offsets list, but
		without the extra end-of-elements entry. A count of 0 or 1 means
		the corresponding data block element is not an array (has one
		value). If array_counts is NULL, the data block contains no
		arrays, and the count of each element will be set to 1. For an
		alternative method of specifying data block array counts see
		#array_offset_counts.
	@param	native_order	true if the data is in native host order; false
		otherwise.
	@see	element_offsets(const Index*, Index)
	@see	array_counts(const Index*)
*/
Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements = 0,
	const Index*	array_counts = NULL,
	bool			native_order = true
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and array value counts.

	The byte order of the data block values may be specified as
	non-native.

	@param	data	The address of the data block to be managed.
	@param	offsets	A vector of element offset values, plus the
		end-of-elements offset.
	@param	array_counts	A vector of array value counts for each
		element in the data block. This list corresponds to the offsets
		list, but without the extra end-of-elements entry. A count of 0
		or one means the corresponding data block element is not an array
		(has one value). Any unspecified array counts are set to 1. Extra
		values are ignored. For an alternative method of specifying data
		block array counts see #array_offset_counts.
	@param	native_order	true if the data is in native host order;
		false otherwise.
	@see	element_offsets(const Index*, Index)
	@see	array_counts(const Index*)
*/
Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	const Value_List&	array_counts,
	bool				native_order = true
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and the byte order of the data
	block.

	All elements of the data block have a single value.

	The byte order of the data block values may be specified as
	non-native.

	@param	data	The address of the data block to be managed.
	@param	offsets	A vector of element offset values, plus the
		end-of-elements offset.
	@param	native_order	true if the data is in native host order;
		false otherwise.
*/
Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	bool				native_order = true
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets, array value counts, and the byte
	order of the data block.

	@param	data	The address of the data block to be managed.
	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure, plus the offset to the next byte after the last
		element of the structure. For an alternative method of
		describing the data block element locations see #element_sizes.
	@param	total_elements	The number of elements in the data block.
		This corresponds to one less than the number of entries in the
		offsets array because it excludes the final end-of-elements
		offset. <b>N.B.</b>: the offsets array must contain one more
		entry (the end-of-elements offset) than the total_elements value.
		However, if the total_elements argument is not provided, then the
		final entry (after the end-of-elements offset) in the offsets
		array must be 0, and the total_elements value will be determined
		by counting, from the second offsets entry (because the first is
		expected to be 0), the number of non-0 offsets array entries
		and subtracting one for the final end-of-elements offset.
		<b>N.B.</b>: In this case if the first element of the data block
		is located at the beginning of the data block (which is typical)
		then this 0 offset must be the first offsets array entry or it
		will be seen as marking the end of the list.
	@param	array_counts	A list of array value counts for each element
		in the data block. This list corresponds to the offsets list, but
		without the extra end-of-elements entry. A count of 0 or one
		means the corresponding data block element is not an array. If
		array_counts is NULL, the data block contains no arrays and the
		count of each element will be set to 1. For an alternative method
		of specifying data block array counts see #array_offset_counts.
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
	@see	element_offsets(const Index*, Index)
	@see	array_counts(const Index*)
	@see	array_offset_counts(const Index[][2], Index, bool)
*/
Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements,
	const Index*	array_counts,
	Data_Order		data_order
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets, array value counts, and the byte
	order of the data block.

	@param	data	The address of the data block to be managed.
	@param	offsets	A vector of element offset values, plus the
		end-of-elements offset.
	@param	array_counts	A vector of array value counts for each
		element in the data block. This list corresponds to the offsets
		list, but without the extra end-of-elements entry. A count of 0
		or one means the corresponding data block element is not an array
		(has one value). Any unspecified array counts are set to 1. Extra
		values are ignored. For an alternative method of specifying data
		block array counts see #array_offset_counts.
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
	@see	element_offsets(const Index*, Index)
	@see	array_counts(const Index*)
*/
Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	const Value_List&	array_counts,
	Data_Order			data_order
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and the byte order of the data
	block.

	All elements of the data block have a single value.

	@param	data	The address of the data block to be managed.
	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure, plus the offset to the next byte after the last
		element of the structure. For an alternative method of
		describing the data block element locations see #element_sizes.
	@param	total_elements	The number of elements in the data block.
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
	@see	element_offsets(const Index*, Index)
	@see	Data_Order
*/
Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Index			total_elements,
	Data_Order		data_order
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and the byte order of the data
	block.

	All elements of the data block have a single value.

	@param	data	The address of the data block to be managed.
	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure, plus the offset to the next byte after the last
		element of the structure. The end of the list must be marked
		with a 0 entry (the first entry may be also be valid 0
		offset).
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
	@see	element_offsets(const Index*, Index)
	@see	Data_Order
*/
Data_Block
	(
	void* const		data,
	const Index*	offsets,
	Data_Order		data_order
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets and the byte order of the data
	block.

	All elements of the data block have a single value.

	@param	data	The address of the data block to be managed.
	@param	offsets	A vector of element offset values, plus the
		end-of-elements offset.
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
*/
Data_Block
	(
	void* const			data,
	const Value_List&	offsets,
	Data_Order			data_order
	);

/**	Constructs a Data_Block for a block of data having a structure
	defined by its element offsets, a set of array counts for various
	elements, and the byte order of the data block.

	@param	data	The address of the data block to be managed.
	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure, plus the offset to the next byte after the last
		element of the structure. The end of the list must be marked
		with a 0 entry (the first entry may be also be valid 0
		offset).
	@param	indexed_counts	An array[][2] of element index,count pairs;
		where indexed_counts[][ELEMENT_INDEX] is the index for the
		element to have its array value count set, and
		indexed_counts[][INDEX_COUNT] is the count of values in the
		array. The entries do not need to be ordered. All elements not
		specified are taken to have a single value. A count of 0 or
		one means the corresponding element is not an array.
	@param	data_order	A #Data_Order value of either MSB or LSB to
		indicated that the values in the data block are ordered Most or
		Least Significant Byte first.
	@see	element_offsets(const Index*, Index)
	@see	array_indexed_counts(const Index[][2], Index, bool);
	@see	Data_Order
*/
Data_Block
	(
	void* const		data,
	const Index*	offsets,
	const Index		indexed_counts[][2],
	Data_Order		data_order
	);

/*..............................................................................
	Copy
*/
/**	Constructs a Data_Block as a copy of another Data_Block.

	<b>N.B.</b>: The data storage area is shared by both objects;
	only the pointer to the storage area is copied, not the data
	itself. This allows a single storage area to be managed with
	different structure definitions.

	@param	data_block	A reference to the Data_Block to be copied.
*/
Data_Block (const Data_Block& data_block);

/**	Assigns the data block, structure, and data order of another
	Data_Block to this Data_Block.

	<b>N.B.</b>: The data storage area is shared by both objects;
	only the pointer to the storage area is copied, not the data
	itself. This allows a single storage area to be managed with
	different structure definitions.

	@param	data_block	A reference to the Data_Block to be copied.
*/
Data_Block& operator= (const Data_Block& data_block);

/*..............................................................................
	Destructor
*/
/*	Destroys the Data_Block, deleting the dynamically allocated
	structure definition arrays.

	<b>N.B.</b>: The memory containing the binary data block remains
	the responsibility of the user. It may still be in use by some
	other Data_Block. If the memory was provided dynamically, the user
	must free it when it is no longer needed. 
*/
virtual ~Data_Block ();

/*==============================================================================
	Accessors
*/
/*------------------------------------------------------------------------------
	Data order
*/
/**	Tests if the data is specified in native order.

	@return	true if the data is in native order; false otherwise.
*/
bool native () const
	{return Native;}

/**	Sets whether the data is in native order.

	@param	native_order	true if the data is to be treated as in
		host native data order; false otherwise.
	@return	This Data_Block.
*/
Data_Block& native (bool native_order);

/**	Gets the native order of the host system.

	@return	The Data_Order of the host system.
	@see	host_is_high_endian()
*/
static Data_Order native_order ()
	{return host_is_high_endian () ? MSB : LSB;}

/**	Gets the data ordering.

	@return	A Data_Order value of either MSB or LSB.
	@see	Data_Order
*/
Data_Order data_order () const;

/**	Sets the data ordering.

	@param	order	A Data_Order value of either MSB or LSB.
	@return	This Data_Block.
	@see	Data_Order
*/
Data_Block& data_order (Data_Order order);

/*------------------------------------------------------------------------------
	Data storage
*/
/**	Gets the address of the data storage area.

	@return	A pointer (unsigned char*) to the data storage block.
*/
unsigned char* data () const
	{return Block;}

/**	Sets the address of the data storage area.

	<b>N.B.</b>: Changing the data storage location does not change the
	existing data structure description.

	@param	block	The address (void* const) of the new data storage block.
	@return	This Data_Block.
*/
virtual Data_Block& data (void* const block);

/**	Gets the size (bytes) of the element containing data block.

	@return	The number of contiguous bytes from the beginning  of the
		data block to the end (exclusive) of the last element. This is
		the end-of-elements offset. <b>N.B.</b>: This is not necessarily
		the size of the actual data storage area, which is owned by the
		user. Note that the first element may not be located at the
		beginning of the data block.
*/
Index size () const
	{return (Counts.size () ? Offsets[Counts.size ()] : 0);}

/*------------------------------------------------------------------------------
	Element offsets
*/
/**	Gets the element offsets list of the data block structure.

	@return	A vector of the current data block offsets, including
		the final end-of-elements offset.
*/
Value_List element_offsets_list () const
	{return Offsets;}

/**	Gets the element offsets list of the data block structure.

	<b>CAUTION</b>: This method does not return a copy of the element
	offsets values; a pointer to the list of values in use by the object
	is returned. Direct modification of the list contents can result in
	structure inconsistencies such that subsequent operations may not
	work correctly. The list will become invalid when the object with
	which it is associated is deleted. Use of the structure modification
	methods is the recommended approach.

	@return	A pointer to the current data block offsets list.
	@see	element_offsets_list()
*/
Index* element_offsets ()
	{return &Offsets[0];}

/**	Sets the element offsets of the data block structure.

	This method determines the total number of {@link elements()const
	elements} in the data block.

	<b>N.B.</b>: The count of array values for each element is reset to 1
	if the element has a size of 0 or is not an even multiple of its
	current array value count.

	@param	offsets	A vector of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure. In addition, the last value is the offset to the next
		byte after the last element of the structure. <b>N.B.</b>: There
		is one more offset value than the number of elements in the data
		block structure. The offset value entries may be in any order;
		they will be sorted. If the offsets vector is empty there are no
		elements.
	@return	true if any array count was reset; false otherwise.
	@throws	length_error	If the offsets vector only contains a single
		value.
*/
bool element_offsets (const Value_List& offsets);

/**	Sets the element offsets of the data block structure.

	This method determines the total number of {@link elements()const
	elements} in the data block.

	<b>N.B.</b>: The count of array values for each element is reset to 1
	if the element has a size of 0 or is not an even multiple of its
	current array value count.

	<b>A Single Empty Element</b>: To specify a single empty element
	without specifying the total_elements argument provide an offsets
	array containing only two 0 values. This is a special case where a
	single element at offset 0 is empty so it has an end-of-elements
	offset of 0, also, which can only occur in this case.

	@param	offsets	An array of Index values specifying the byte offset
		from the data block address to each element of the data block
		structure. In addition, the last value is the offset to the
		next byte after the last element of the structure. Typically,
		but not necessarily, the first offset is 0 and the last
		offset is the size of the data block. <b>N.B.</b>: There is one
		more offset than the number of elements in the data block
		structure. The offset value entries may be in any order (except
		see the discussion of the total_elements argument); they will
		be sorted. If offsets is NULL there are no elements.
	@param	total_elements	The number of elements in the data block.
		This corresponds to one less than the number of entries in the
		offsets array because it excludes the final end-of-elements
		offset. <b>N.B.</b>: The offsets array must contain one more
		entry (the end-of-elements offset) than the total_elements value.
		However, if the total_elements value is not provided, then the
		final entry (after the end-of-elements offset) in the offsets
		array must be 0, and the total_elements value will be
		determined by counting, from the second offsets entry (because
		the first is expected to be 0), the number of non-0 offsets
		entries and subtracting one for the final end-of-elements offset.
		<b>N.B.</b>: In this case if the first element of the data block
		is located at the beginning of the data block (which is typical)
		then this 0 offset must be the first offsets array entry or it
		will be seen as marking the end of the list. If total_elements is
		0 there are no elements.
	@return	true if any array count was reset; false otherwise.
	@throws	length_error	If the offsets vector only contains a single
		non-zero offset value and total_elements was not specified.
	@see	reset_counts(bool)
*/
bool element_offsets (const Index* offsets, Index total_elements = NO_INDEX);

/**	Sets the element offsets of the data block structure based on
	the size of each element.

	This method determines the total number of {@link elements()const
	elements} in the data block.

	<b>N.B.</b>: The count of array values for each element is reset to 1
	if the element has a size of 0 or is not an even multiple of its
	current array value count.

	@param	sizes	A vector of Index values specifying the size in
		bytes of each element of the data block structure. The size
		entries must be in the same order as the elements of the data
		block.
	@return	true if any array count was reset; false otherwise.
	@see	reset_counts(bool)
*/
bool element_sizes (const Value_List& sizes);

/**	Sets the element offsets of the data block structure based on
	the size of each element.

	This method determines the total number of {@link elements()const
	elements} in the data block.

	<b>N.B.</b>: The count of array values for each element is reset to 1
	if the element has a size of 0 or is not an even multiple of its
	current array value count.

	@param	sizes	An array of Index values specifying the size in
		bytes of each element of the data block structure. The size
		entries must be in the same order as the elements of the data
		block.
	@param	total_elements	The number of elements in the data block. If
		the total_elements value is not provided, then the final entry in
		the sizes array must be 0, and the total_elements value will
		be determined by counting the number of non-0 offsets array
		entries.
	@return	true if any array count was reset; false otherwise.
	@see	reset_counts(bool)
*/
bool element_sizes (const Index* sizes, Index total_elements = NO_INDEX);

/**	Gets the offset of a data block element.

	@param	element	The data block element to examine.
	@return	The offset to the element in data block.
	@throws std::out_of_range	If an invalid element is specified.
*/
Index offset_of (Index element) const;

/**	Sets the offset of a data block element.

	<b>CAUTION</b>: After the element offset is set the list of offset
	values for all elements is sorted to ensure that they remain in
	offset order. This could result in an element effectively being
	relocated to a new Index position; but element data will not be
	relocated.

	<b>WARNING</b>: The element array value counts are each reset if the
	amount of space available to the element is no longer a multiple of
	the number of array values.

	@param	element	The element for which to set the data block offset.
	@param	offset	The byte offset of the element from the beginning
		of the data block.
	@return	true if any array count was reset; false otherwise.
	@throws std::out_of_range	If an invalid element is specified.
	@throws std::length_error	If the offset is beyond the end of the
		data block.
	@see	reset_counts(bool)
*/
bool offset_of (Index element, Index offset);

/**	Inserts a new element.

	The new element is inserted before the first element at the same or
	greater offset than the new element. If there is no such element, the
	new element is appended to the elements list and the end-of-elements
	offset is addusted accordingly. All following elements at higher
	offsets (if any) are shifted up by the size of the new element. The
	size of the preceding element (if any) will be changed if the offset
	of the new element is not the same as an existing element or the
	end-of-elements offset. In this case the array value count of the
	preceding element is reset to 1.

	@param	offset	The offset of the new element.
	@param	size	The size of the new element.
	@param	array_count	The count of array values for the element.
	@return	true if the array count of the preceding element was reset;
		false otherwise.
	@throws	length_error	If the size is not an integer multiple
		of the array_count.
*/
bool insert_element (Index offset, Index size, Index array_count = 1);

/**	Deletes an element.

	All following elements at higher offsets are shifted down by
	the size of the deleted element.

	@param	element	The Index of the element to be deleted.
	@return	This Data_Block.
*/
Data_Block& delete_element (Index element);

/**	Shift all the element offset locations.

	The shift may be positive or negative. However, the result of the
	shift may not move the first element to a negative offset.

	@param	amount	The amount of the offsets shift.
	@return	This Data_Block.
	@throws	invalid_argument	If a negative shift amount would result
		in a negative first element offset.
*/
Data_Block& shift_offsets (int amount);

/*------------------------------------------------------------------------------
	number of ...
*/
/**	Gets the number of elements in the data block.

	@return	The number of data block elements (does not include data
		block array counts).
*/
Index elements () const
	{return Counts.size ();}

/**	Gets the size of a data block element.

	@param	element	The data block element to examine.
	@return	The size of the entire element.
	@throws std::out_of_range	If an invalid element is specified.
*/
Index size_of (Index element) const;

/**	Sets the size of a data block element.

	<b>WARNING</b>: The offsets of all elements beyond the resized
	element are adjusted. This will alter the effective and expected size
	of the data storage area of the data block for which the user must
	allow.

	<b>CAUTION</b>: If the array value count of the resized element does
	not accommodate the new size, the count is reset to 1.

	@param	element	The data block element to examine.
	@param	size	The size (bytes) of the entire element.
	@return	true if the array count of the element was reset; false
		otherwise.
	@throws std::out_of_range	If an invalid element is specified.
	@see	data(void*)
*/
bool size_of (Index element, Index size);

/**	Gets the size of a data block element value.

	If the element is not an array of values the size of the element is
	returned (same as #size_of (Index)). Otherwise the size of a single
	array value is returned.

	@param	element	The data block element to examine.
	@return	The size of a single array element value,
		or the entire element if it has only one value.
	@throws std::out_of_range	If an invalid element is specified.
*/
Index value_size_of (Index element) const;

/**	Sets the size of a data block element value.

	If the element is an array of values, the size of each value is
	set; otherwise the size of the element is set (same as
	#size_of (Index, Index)).

	@param	element	The data block element to examine.
	@param	size	The size (bytes) of a single element value,
		or the entire element if it has only one value.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element is specified.
	@see	size_of(Index, Index)
*/
Data_Block& value_size_of (Index element, Index size);

/**	Gets the element index for a data block offset.

	@param	offset	The data block offset to find.
	@return	The index of the corresponding element; or #NO_INDEX if the
		offset value can not be found in the offsets list.
*/
Index index_of (Index offset) const;

/*------------------------------------------------------------------------------
	Array value counts
*/
/**	Gets the list of element array value counts.

	@return	A vector of the current element array value counts.
*/
Value_List array_counts_list () const
	{return Counts;}

/**	Gets the list of array element value counts.

	<b>CAUTION</b>: This method does not return a copy of the array
	counts values; a pointer to the list of values in use by the object
	is returned. Direct modification of the list contents can result in
	structure inconsistencies such that subsequent operations may not
	work correctly. The list will become invalid when the object with
	which it is associated is deleted. Use of the structure modification
	methods is the recommended approach.

	@return	A pointer to the current element counts list.
*/
Index* array_counts ()
	{return &Counts[0];}

/**	Sets the count of array values for elements of the data block.

	@param	counts	A vector of value counts. The values must be in the
		same order as the elements of the data block. A count of 0 (which
		will be coerced to 1) or 1 means the corresponding data block
		element is not an array.
	@param	begin	The index of the first element to have its count set.
	@return	This Data_Block.
	@throws std::length_error	If the size of an element is not an
		integral multiple of the array value count to be set for it.
	@see	element_offsets(const Index*, Index)
*/
Data_Block& array_counts (const Value_List& counts, Index begin = 0);

/**	Sets the count of array values for elements of the data block.

	@param	counts	An array of value counts for each element in the
		[being,end) range. The values must be in the same order as the
		elements of the data block. A count of 0 (which will be coerced
		to 1) or 1 means the corresponding data block element is not an
		array. If counts is NULL, the elements contain no arrays, and
		the count of each element will be reset to 1.
	@param	begin	The index of the first element to have its count
		set.
	@param	end		The index of the last element (exclusive) in the
		range of elements for which array counts are to be set. If end is
		beyond the end of the element range it is limited to the end of
		the element range. If not specified it is taken to be the end of
		the element range.
	@return	This Data_Block.
	@throws std::length_error		If the size of an element can not
		accommodate the array value count or is not an integral
		multiple of the count.
	@see	element_offsets(const Index*, Index)
*/
Data_Block& array_counts
	(const Index* counts, Index begin = 0, Index end = NO_INDEX);

/**	Sets the element counts for arrays keyed by element Index.

	@param	indexed_counts	An array[][2] of element index,count pairs;
		where indexed_counts[][ELEMENT_INDEX] is the index for the
		element to have its array value count set, and
		indexed_counts[][INDEX_COUNT] is the count of values in the
		array. The entries do not need to be ordered. Duplicate index
		values are allowed; each is applied in the order they occur in
		the array. A count of 0 or one means the corresponding data
		block element is not an array. If indexed_counts is NULL, all
		counts will be reset to 1.
	@param	total_counts	The total number of index,count pairs. If
		this argument is not provided, then the end of the list must be
		marked by index and count values of 0.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element index is specified.
	@throws std::length_error	If the size of an element can not
		accommodate the array value count or is not an integral
		multiple of the count.
	@see	reset_counts(bool)
*/
Data_Block& array_indexed_counts
	(const Index indexed_counts[][2], Index total_counts = NO_INDEX);

/**	Sets the element counts for arrays at various data block offsets.

	<b>N.B.</b>: When setting both offsets and value counts, always
	set the offsets first.

	@param	offset_counts	An array[][2] of offset,count Index pairs;
		where offset_counts[][ELEMENT_OFFSET] is the offset in the data
		block for the element, and offset_counts[][OFFSET_COUNT] is the
		count of values in the element. Each offset is matched with its
		element. A count of 0 or one means the corresponding data
		block element is not an array. The entries do not need to be
		ordered. Duplicate offset values are allowed; each is applied in
		the order they occur in the array. If offset_counts is NULL, all
		counts will be reset to 1.
	@param	total_counts	The total number of offset,count pairs. If
		this argument is not provided, then the end of the list must be
		marked by offset and count values 0.
	@return	This Data_Block.
	@throws std::invalid_argument	If no matching element for an offset
		can be found.
	@throws std::length_error	If the size of an element can not
		accommodate the array value count or is not an integral
		multiple of the count.
	@see	reset_counts(bool)
*/
Data_Block& array_offset_counts
	(const Index offset_counts[][2], Index total_counts = NO_INDEX);

/**	Gets the number of values in a data block element.

	The return value will be 0 if, and only if, the {@link size_of(Index)const
	size of} the element is 0.

	@param	element	The data block element to examine.
	@return	The number of element values.
	@throws std::out_of_range	If an invalid element is specified.
*/
Index count_of (Index element) const;

/**	Sets the array value count for an element.

	@param	element	The element for which to set the array value count.
	@param	count	The number of array values in the element. A count
		of 0 or one means the corresponding data block element is
		not an array (has 1 value).
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element is specified.
	@throws std::length_error	If the size of the element can not
		accommodate the array value count or is not an integral
		multiple of the count.
*/
Data_Block& count_of (Index element, Index count);

/**	Resets element array value counts.

	Each affected array count is reset to 1.

	@param	unconditional	If true, the array counts are unconditionally
		reset; otherwise only elements with a size that is not a multiple
		of the count of its array values are reset.
	@param	begin	The index of the first element to have its count
		reset.
	@param	end		The index of the last element (exclusive) in the
		range of elements for which array counts are to be reset. If end
		is beyond the end of the element range it is limited to the end
		of the element range. If not specified it is taken to be the end
		of the element range.
	@return	true if any array count was reset; false otherwise.
*/
bool reset_counts (bool unconditional = true,
	Index begin = 0, Index end = NO_INDEX);

/*==============================================================================
	I/O
*/
/*..............................................................................
	Primary get/put generic interface
*/
/**	Pointer to a #Copier function that gets data from the
	Block (source) into a host (destination) variable in the correct order.
*/
Copier
	Get;

/**	Pointer to a #Copier function that puts data into the
	Block (destination) from a host (source) variable in the correct order.
*/
Copier
	Put;

/**	Get data from the block into a value of any type.

	If the element index is to an array element, only one value will be
	transferred. If the data block element value is larger than the
	application value, then only the least significant bytes from the
	data block that will fit in the variable are transferred (this
	corresponds to an integer downcast). If the data block element
	value is smaller than the application value, then the data bytes
	are transferred into the least significant bytes of the variable.

	<b>N.B.</b>: No type conversion is done (the data types of data
	block elements are unknown). Transferring data between mismatched
	type values or sizes may produced unexpected results.

	@param	T		The data type of the value to be assigned.
	@param	limits_check	True if limit checking for the element and
		index arguments is to be applied; otherwise no limit checking
		is done, which is slightly faster.
	@param	value	A reference to a variable of type T that will
		receive the data.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and limits_check is true.
*/
template<typename T, bool limits_check>
const Data_Block&
get (T& value, const Index element, const Index index = 0) const
{
if (Block)
	{
	if (limits_check)
		limits_checker (element, index, true, false);
	unsigned int
		amount = (Offsets[element + 1] - Offsets[element]) / Counts[element];
	Get
		(
		reinterpret_cast<unsigned char*>(&value),	//	Destination: User Value
		sizeof (T),
		Block + Offsets[element] + (amount * index),//	Source: Data Block
		amount
		);
	}
return *this;
}

/**	Get data from the block into a value of any type.

	Equivalent to the template with the same signature except
	DATA_BLOCK_LIMITS_CHECK is used by default.

	@param	T		The data type of the value to be assigned.
	@param	value	A reference to a variable of type T that will
		receive the data.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.
*/
template<typename T>
const Data_Block&
get (T& value, const Index element, const Index index = 0) const
	{return get<T, DATA_BLOCK_LIMITS_CHECK> (value, element, index);}

/**	Get a datum of any type from the block.

	@param	T		The data type of the value to be returned.
	@param	limits_check	True if limit checking for the element and
		index arguments is to be applied; otherwise no limit checking
		is done, which is slightly faster.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	A value of type T.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.
*/
template<typename T, bool limits_check>
T get (const Index element, const Index index = 0) const
	{
	T value;
	get<T, limits_check> (value, element, index);
	return value;
	}

/**	Get a datum of any type from the block.

	Equivalent to the template with the same signature except
	DATA_BLOCK_LIMITS_CHECK is used by default.

	@param	T		The data type of the value to be returned.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	A value of type T.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.

*/
template<typename T>
T get (const Index element, const Index index = 0) const
	{return get<T, DATA_BLOCK_LIMITS_CHECK> (element, index);}


/**	Put data into the block from a value of any type.

	If the element index is to an array element, only one value will be
	transferred. If the data block element value is larger than the
	application value, then the variable bytes are transferred into the
	least significant bytes of the data element value. If the data
	block element value is smaller than the application value, then
	only the least significant bytes from the application variable that
	will fit in the data element value are transferred (this
	corresponds to an integer downcast).

	<b>N.B.</b>: No type conversion is done (the data types of data
	block elements are unknown). Transferring data between mismatched
	type values or sizes may produced unexpected results.

	@param	T	The data type of the value to be transferred.
	@param	limits_check	True if limit checking for the element and
		index arguments is to be applied; otherwise no limit checking
		is done, which is slightly faster.
	@param	value	A reference to a variable of type T that contains
		the data to be transferred.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and limits_check is true.
*/
template<typename T, bool limits_check>
Data_Block& put (T& value, const Index element, const Index index = 0)
{
if (Block)
	{
	if (limits_check)
		limits_checker (element, index, false, false);
	unsigned int
		amount = (Offsets[element + 1] - Offsets[element]) / Counts[element];
	Put
		(
		Block + Offsets[element] + (amount * index),//	Destination: Data Block
		amount,
		reinterpret_cast<const unsigned char*>(&value),	//	Source: User Value
		sizeof (T)
		);
	}
return *this;
}

/**	Put data into the block from a value of any type.

	Equivalent to the template with the same signature except
	DATA_BLOCK_LIMITS_CHECK is used by default.

	@param	T		The data type of the value to be transferred.
	@param	value	A reference to a variable of type T that contains
		the data to be transferred.
	@param	element	A data block element offset entry Index.
	@param	index	An array element value index.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.
*/
template<typename T>
Data_Block& put (T& value, const Index element, const Index index = 0)
	{return put<T, DATA_BLOCK_LIMITS_CHECK> (value, element, index);}

/**	Get data from the block into an array of values of any type.

	A data block element with a single value (non-array element) is the
	same as an array element with only one value.

	@param	T		The data type of the array values.
	@param	limits_check	True if limit checking for the element and
		index arguments is to be applied; otherwise no limit checking
		is done, which is slightly faster.
	@param	array	A pointer to an array of values of type T.
	@param	element	A data block element offset entry Index.
	@param	count	The number of values to transfer. If this value is
		0 then all array values at the element offset indexed will be
		transferred (make sure the array has sufficient space). If this
		value is larger than the number of element array values, then
		only the number of element array values will be transferred.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and limits_check is true.
*/
template<typename T, bool limits_check>
const Data_Block& get (T* array, const Index element, Index count = 0) const
{
if (Block)
	{
	if (limits_check)
		limits_checker (element, count, true, true);
	unsigned int
		amount = (Offsets[element + 1] - Offsets[element]) / Counts[element];
	if (! count || count > Counts[element])
		count = Counts[element];
	unsigned char*
		data = Block + Offsets[element];

	//	Get each array element
	while (count--)
		{
		Get (reinterpret_cast<unsigned char*>(array), sizeof (T), data, amount);
		data += amount;
		array++;
		}
	}
return *this;
}

/**	Get data from the block into an array of values of any type.

	Equivalent to the template with the same signature except
	DATA_BLOCK_LIMITS_CHECK is used by default.

	@param	T		The data type of the array values.
	@param	array	A pointer to an array of values of type T.
	@param	element	A data block element offset entry Index.
	@param	count	The number of values to transfer. If this value is
		0 then all array values at the element offset indexed will be
		transferred (make sure the array has sufficient space). If this
		value is larger than the number of element array values, then
		only the number of element array values will be transferred.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.
*/
template<typename T>
const Data_Block& get (T* array, const Index element, Index count = 0) const
	{return get<T, DATA_BLOCK_LIMITS_CHECK> (array, element, count);}


/**	Put data into the block from an array of values of any type.

	A data block element with a single value (non-array element) is the
	same as an array element with only one value.

	@param	T		The data type of the array values.
	@param	limits_check	True if limit checking for the element and
		index arguments is to be applied; otherwise no limit checking
		is done, which is slightly faster.
	@param	array	A pointer to an array of values of type T.
	@param	element	A data block element offset entry Index.
	@param	count	The number of values to transfer. If this value is
		0 then all array values at the element offset indexed will be
		filled (make sure the array has sufficient values). If this
		value is larger than the number of element array values, then
		only the number of element array values will be filled.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and limits_check is true.
*/
template<typename T, bool limits_check>
Data_Block& put (T* array, const Index element, Index count = 0)
{
if (Block)
	{
	if (limits_check)
		limits_checker (element, count, false, true);
	unsigned int
		amount = (Offsets[element + 1] - Offsets[element]) / Counts[element];
	if (! count || count > Counts[element])
		count = Counts[element];
	unsigned char*
		data = Block + Offsets[element];

	//	Get each array element
	while (count--)
		{
		Put (data, amount,
			reinterpret_cast<const unsigned char*>(array), sizeof (T));
		data += amount;
		array++;
		}
	}
return *this;
}

/**	Put data into the block from an array of values of any type.

	Equivalent to the template with the same signature except
	DATA_BLOCK_LIMITS_CHECK is used by default.

	@param	T		The data type of the array values.
	@param	array	A pointer to an array of values of type T.
	@param	element	A data block element offset entry Index.
	@param	count	The number of values to transfer. If this value is
		0 then all array values at the element offset indexed will be
		filled (make sure the array has sufficient values). If this
		value is larger than the number of element array values, then
		only the number of element array values will be filled.
	@return	This Data_Block.
	@throws std::out_of_range	If an invalid element or index is specified
		and DATA_BLOCK_LIMITS_CHECK is true.
*/
template<typename T>
Data_Block& put (T* array, const Index element, Index count = 0)
	{return put<T, DATA_BLOCK_LIMITS_CHECK> (array, element, count);}

/**	Input data bytes from a stream into the data block.

	@param	stream	The istream from which to read the data.
	@return	The stream.
*/
std::istream& input  (std::istream& stream);

/**	Output data bytes from the data block into a stream.

	@param	stream	The ostream to which to write the data.
	@return	The stream.
*/
std::ostream& output (std::ostream& stream) const;

/*	Prints a description of the Data_Block to a stream.

	@param	stream	The ostream to which to print the description.
	@return	The stream.
*/
std::ostream& print (std::ostream& stream = std::cout) const;

/*==============================================================================
	Utilities
*/
/**	Converts a vector of size values to offset values.

	<b>N.B.</b>: The size values are converted to offset values in place.

	Each size value is added to its new offset value to determine the new
	offset value of the next value. The first value is set to a 0
	offset. An additional final offset value is appended that is the
	end-of-elements value.

	@param	sizes	A vector of size values.
	@return	The vector with the values converted to offsets.
*/
static Value_List& sizes_to_offsets (Value_List& sizes);

/**	Converts an array of size values to an array of offset values.

	<b>N.B.</b>: The size values are converted to offset values in place.

	Each size value is added to its new offset value to determine the new
	offset value of the next value. The first value is set to a 0
	offset. An additional final offset value is appended that is the
	end-of-elements value.

	The array must have room for one more element than the number of size
	elements (total_elements). This is typically done by putting a 0 in
	this extra location to mark the end of the array and then omitting
	the total_elements argument. After conversion the final, formerly
	extra, value will be filled with the end-of-elements offset value.

	@param	sizes	An array of size values (plus one extra). If NULL
		nothing is done.
	@param	total_elements	The total number of size values in the array.
		If 0, nothing is done. If this argument is omitted then the
		end of the array must be marked with a 0 value.
	@return	The array with the values converted to offsets.
*/
static Index* sizes_to_offsets (Index* sizes, Index total_elements = NO_INDEX);

/**	Converts a vector of offset values to size values.

	<b>N.B.</b>: The offset values are converted to size values in place.

	The vector values are sorted into increasing value order. Then each
	value is replaced with the difference between the next value and the
	current value. The exception is the last value which is removed from
	the vector.

	@param	offsets	A vector of offset values.
	@returns The vector with the values converted to sizes.
	@throws	std::length_error	If the offsets vector contains only one
		value.
*/
static Value_List& offsets_to_sizes (Value_List& offsets);

/**	Converts an array of offset values to size values.

	<b>N.B.</b>: The offset values are converted to size values in place.

	The array values are sorted into increasing value order. Then each
	value is replaced with the difference between the next value and the
	current value. The exception is the last value which is replaced with
	a 0 value.

	@param	offsets	An array of offset values.
	@param	total_elements	The total number of offset values to be
		converted. <b>N.B.</b>: This does not include the final
		end-of-elements offset value. If this argument is omitted then the
		end of the array must be marked with a 0 value. <b>N.B.</b>: If
		one of the offset values is 0 (which is typical) and the
		total_elements argument is omitted, then the 0 offset must be
		the first entry in the array or it will be misinterpreted as
		marking the end of the array.
	@returns The array with the values converted to sizes.
	@throws	std::length_error	If the offsets array contains only one
		value.
*/
static Index* offsets_to_sizes
	(Index* offsets, Index total_elements = NO_INDEX);

/*..............................................................................
	Static data movers.
*/
protected:

//!	Get data from the block into a host variable in native order.
static void
get_forwards
	(
	unsigned char*			host,			//	Destination: User Value
	int						host_amount,
	const unsigned char*	data,			//	Source: Data Block
	int						data_amount
	);

//!	Get data from the block into a host variable in reverse order.
static void
get_backwards
	(
	unsigned char*			host,			//	Destination: User Value
	int						host_amount,
	const unsigned char*	data,			//	Source: Data Block
	int						data_amount
	);

//!	Put data from a host variable into the block in native order.
static void
put_forwards
	(
	unsigned char*			data,			//	Destination: Data Block
	int						data_amount,
	const unsigned char*	host,			//	Source: User Value
	int						host_amount
	);

//!	Put data from a host variable into the block in reverse order.
static void
put_backwards
	(
	unsigned char*			data,			//	Destination: Data Block
	int						data_amount,
	const unsigned char*	host,			//	Source: User Value
	int						host_amount
	);

//!	Limits checker used by the I/O template functions.
void limits_checker
	(
	const Index		element,
	const Index		index,
	bool			getter,
	bool			array
	) const;

//..............................................................................
private:

//!	Pointer to the data block storage area (may be NULL).
unsigned char*
	Block;

/**	Offsets of the data elements.

	The values are maintained in increasing order. Each value is the
	offset into the Block where the element starts. There is one more
	value than the number of elements: the last value is the offset of
	the end (exclusive) of elements.
*/
Value_List
	Offsets;

/**	Counts of the number of array values in each element.

	The values are maintained so that each evenly divides the element
	into equal sized units. A value that is unable to equally divide its
	element is reset to 1.
*/
Value_List
	Counts;

//!	Data value bytes are in native host order flag.
bool
	Native;

};	//	class Data_Block

/*==============================================================================
	I/O operators
*/
/**	Prints a description of the Data_Block to a stream.

	@param	stream	The ostream to which to print the description.
	@param	data_block	The Data_Block to describe.
	@return	The stream.
	@see	Data_Block::print(ostream)
*/
std::ostream& operator<<
	(std::ostream& stream, const Data_Block& data_block);

/**	Input data bytes from a stream into the data block.

	@param	stream	The istream from which to read the data.
	@param	data_block	The Data_Block to receive the data.
	@return	The stream.
	@see	Data_Block#output(ostream)
*/
std::istream& operator>>=
	(std::istream& stream, Data_Block& data_block);

/**	Output data bytes from the data block into a stream.

	@param	stream	The ostream to which to write the data.
	@param	data_block	The Data_Block to write.
	@return	The stream.
	@see	Data_Block#output(ostream)
*/
std::ostream& operator<<=
	(std::ostream& stream, const Data_Block& data_block);

}	//	namespace PIRL
#endif
