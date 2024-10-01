/*	Cache

PIRL CVS ID: $Id: Cache.hh,v 1.8 2010/11/11 20:33:13 castalia Exp $

Copyright (C) 2005  Arizona Board of Regents on behalf of the Planetary
Image Research Laboratory, Lunar and Planetary Laboratory at the
University of Arizona.

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
#ifndef _Cache_
#define	_Cache_

#include	<iosfwd>
#include    <cstddef>

namespace PIRL
{
/**	A <i>Cache</i> is a dynamic input storage area.

	The storage capacity is zero or more bytes. A Cache of zero capacity
	has no storage allocated to it. Storage capacity will automatically
	inflate as needed. It can also be deflated if desired.

	The available storage capacity is managed as a refillable user data
	space. The user data capacity may be set to less than the amount of
	allocated storage, but will inflate as needed.

	The user data space is divided into used and free space, with the
	former in a contiguous section preceeding the latter. The used space
	can be logically drained, which will shift all user data following a
	specified next valid data location to the front of the user space and
	thus increasing the amount of free space. The free space can be
	refilled from an input stream bound the the Cache.

	A data margin may be specified as the minimum amount of data to
	retain when the user space is drained. This supports the detection of
	a logical end-of-data during a refill following a drain by the
	automatic application of a user supplied data testing function that
	may require more than one datum. A generic data margin, distinct from
	any margin required by a data testing function, may be specified.

	User data can be put directly into the free space, instead of doing a
	refill from a stream source, with the capacity automatically
	increasing as needed. To get an arbitrary amount of user data it will
	be copied sequentially starting from the next valid datum, with
	automatic drain and refill being used to obtain data as needed.

	@author		Bradford Castalia, UA/PIRL

	$Revision: 1.8 $
*/
class Cache
{
public:
/*==============================================================================
	Types:
*/
/**	Data test function pointer.

	Pointer to a function that takes a data address and returns a condition.
	Used to test data for some condtion on the data.
*/
typedef	bool (*Data_Test) (const void*);

/*==============================================================================
	Constants:
*/
//!	Class identification name with source code version and date.
static const char* const
	ID;

/*==============================================================================
	Constructors
*/
/**	Constructs a default Cache.

	The cache has no capacity (no storage is allocated). The data source
	is set to the standard input.
*/
Cache ();

/**	Constructs a Cache of a given capacity.

	The data source is set to the standard input.

	@param	capacity	The capacity, in bytes, of the data storage area.
	@param	data_margin	The minimum amount of data to retain during a
		drain of the cache contents, or the amount of data required by
		the data_test function if the latter is non-NULL.
	@param	data_test	A #Data_Test function that is used to test
		for an end-of-data condition during a refill of the cache
		contents.
	@see	data_test(Data_Test, unsigned long)
*/
explicit Cache
	(
	unsigned long 	capacity,
	unsigned long	data_margin = 0,
	Data_Test		data_test = NULL
	);

/**	Constructs a Cache of a given capacity and data stream source.

	@param	capacity	The capacity, in bytes, of the data storage area.
	@param	source	The istream from which data will be obtained to
		refill the cache contents.
	@param	data_margin	The minimum amount of data to retain during a
		drain of the cache contents, or the amount of data required by
		the data_test function if the latter is non-NULL.
	@param	data_test	A #Data_Test function that is used to test
		for an end-of-data condition during a refill of the cache
		contents.
	@see	data_test(Data_Test, unsigned long)
*/
Cache
	(
	unsigned long	capacity,
	std::istream&	source,
	unsigned long	data_margin = 0,
	Data_Test		data_test = NULL
	);

/**	Destroys the Cache.

	The data storage area is deleted.
*/
virtual ~Cache ();

private:
//	Copying disallowed:
Cache (const Cache& cache);
Cache& operator= (const Cache& cache);

/*==============================================================================
	Accessors
*/
public:
/**	Sets the data stream source used during a refill.

	<b>N.B.</b>: The number of bytes read is not reset.

	@param	source	An istream that will be used as the source of data
		when the cache is refilled.
	@return	This Cache.
	@see	refill(unsigned long)
*/
Cache& source (std::istream& source)
	{Source = &source; return *this;}

/**	Gets the data stream source.

	@return	The istream
*/
std::istream& source () const
	{return *Source;}

/**	Gets the total number of bytes read into the cache from
	the data source stream.

	If, during a refill, the stream is repositioned back to the location
	of an end-of-data condition the bytes logically returned to the
	stream are not counted as having been read.

	@return	The total number of bytes read into the cache.
	@see	refill(unsigned long)
*/
unsigned long long bytes_read () const
	{return Bytes_Read;}

/**	Sets the minimum amount of data to be retained during a drain.

	The Cache will always guarantee that at least the specified amount of
	data will remain after the Cache has been drained.

	If the amount is non-zero then at least one additional byte must be
	provided for adding new data during a refill. Therefore, the capacity
	of the data storage area will be increased to the amount + 1, if
	necessary.

	<b>N.B.</b>: If a Data_Test function has been provided, then the
	effective data margin amount may be larger than the amount set here.

	@param	amount	The minimum amount of data to be retained during
		a drain.
	@return	This Cache.
	@see	drain()
	@see	data_margin()
	@see	data_test(Data_Test, unsigned long)
*/
Cache& data_margin (unsigned long amount);

/**	Gets the size of the data margin that will be retained during a drain.

	A generic data margin may be defined for any purpose. The required
	data amount for a data_test function is also a data margin. The
	<i>effective</i> data margin is the larger of the data margin set by the
	user, or the data amount (if any) required by the end-of-data test
	function minus one.

	@return The <i>effective</i> data margin amount.
	@see	data_margin(unsigned long)
	@see	data_test(Data_Test, unsigned long)
	@see	drain()
*/
unsigned long data_margin () const
	{return
		(Data_Test_Amount &&
		(Data_Test_Amount - 1) > Data_Margin) ?
		(Data_Test_Amount - 1) : Data_Margin;}

/**	Sets the end-of-data test function and the amount of data required
	for testing.

	The tester function is a Data_Test type: it must take a single const
	char* argument, which will be the address of a valid user datum in
	the cache, and return a bool that is true when an end-of-data
	condition is encountered. The data_amount specifies the number of
	valid user data bytes that must be present starting at the data
	address passed to the tester function. <b>N.B.</b>: While the data
	address passed to the tester function is a char*, the data is not
	necessarily a C-string terminated with a zero-value character.

	The tester function will be used during a refill to test the data
	acquired from the source. Testing starts with the next datum in the
	cache after the last one tested during the previous refill and
	continues to the datum at the required data amount distance from the
	end of user data. During a drain the remaining untested data is
	retained to ensure that it can be tested after more data is added
	after a refill. Note that if the tester function only examines the
	single datum at the address it is given (the data amount is 1) then
	no data will be retained when the cache is drained.

	If the amount of data required by the tester function is greater than
	the cache capacity, then the capacity is increased to the
	data_amount.

	@param	tester	A #Data_Test function. This may be NULL to disable
		end-of-data testing.
	@param	data_amount	The amount of valid user data following the data
		address given to the tester function that must be present in the
		Cache. If this is 0 then tester will be set to NULL.
	@return	This Cache.
	@see	data_margin(unsigned long)
	@see	refill(unsigned long)
*/
Cache& data_test (Data_Test tester, unsigned long data_amount);

/**	Gets the end-of-data test function.

	@return	A pointer to the end-of-data test function. This will be
		NULL if there is no test function.
	@see	data_test(Data_Test, unsgined long)
*/
Data_Test data_test () const
	{return EOD_Test;}

/**	Gets the amount of data required by the end-of-data test function.

	@return	The amount of data, in bytes, required by the test function.
*/
unsigned long data_test_amount () const
	{return Data_Test_Amount;}

/*==============================================================================
	Manipulators
*/
/**	Drains the cache of consumed user data.

	All user data from the start of the cache storage up to, but not
	including, the location of the next data pointer is defined as
	"consumed" and effectively removed from the cache. The remaining data
	is shifted to the start of the cache storage area, thus increasing
	the amount of free space for new user data to be added.

	However, at least the amount of user data equal to the effective data
	margin is retained during a drain regardless of the location of the
	next data pointer. Therefore this amount at the end of the user data
	is always excluded from the consumed data.

	The next data pointer is decremented by the amount of data consumed.
	The address in the cache storage where the user data ends (last) is
	also shifted.

	@see	next(char*)
*/
virtual void drain ();

/**	Refills the cache from the data source stream.

	If the amount of free data storage space is less than the max_amount
	to refill the cache it is first drained to maximize the amount of free
	space. <b>Warning</b>: If no free space can be made available then
	the cache is full and can not be refilled; it is not enlarged and
	the return value is true.

	An attempt is made to fill the free space with data bytes read from the
	source stream. No more than max_amount of data will be read. The
	amount actually read may be less than the amount requested. The
	number of bytes read and the last user data pointer is updated
	by the amount read.

	If no data is read and an end-of-file condition is encountered on the
	stream the return value will be false. <b>N.B.</b>: The stream may
	have an end-of-file status but the refill still return true if any
	data was read.

	If an end-of-data test function has been provided, and the user data
	amount required by the function is present,	then the new data is
	scanned by the function. The new data starts where the last scan left
	off: the pre-refill last margin datum (previous last() -
	data_test_amount()) plus 1 (previous margin datum was the last one
	scanned). The new data ends at the post-refill last margin datum
	(current last() - data_test_amount()). If the data test function
	returns true the datum that was passed to the function is considered
	to mark the logical end of user data. In this case an attempt is made
	reposition the input stream back to this location. If this fails
	(probably because the stream is not repositionable) an attempt is
	made to unget the data back onto the stream (a standard input stream
	can unget at least one byte). Failure to move a stream back
	sufficiently causes an overflow_error exception, with the Cache
	left in a state as if the extra bytes had not been read. A logical
	end-of-data condition will not affect any further operations on the
	Cache, but unless the stream is moved past the end-of-data position
	the next refill will encounter it again. A logical end-of-data
	condition will not result in a false return unless moving the stream
	position back resulted in effectively no data having been read.

	@param	max_amount	The maximum amount of data to read from the stream.
	@return	false if the stream end-of-file or logical end-of-data
		was encounterd; true otherwise. If the cache has no capacity
		false is always returned.
	@throws	std::ios::failure	If a source stream failure occurs.
		The cache pointers will still remain valid.
	@throws overflow_error	If the stream could not be repositioned
		back to an end-of-data location.
	@see	drain()
	@see	data_test(Data_Test, unsigned long)
*/
virtual bool refill (unsigned long max_amount = (unsigned long)-1);

/**	Puts data into the cache storage.

	The data is appended to any user data. If the amount_free is
	less than the amount of data to be added the cache is drained
	to maximize the amount of free space. If this is insufficient
	the cache capacity is enlarged to provided the needed space.

	@param	data	The address from which the user data is to be read.
	@param	amount	The amount of data to put, in bytes.
	@return	This Cache.
	@see	last()
	@see	amount_free()
	@see	drain()
	@see	capacity(unsigned long)
*/
virtual Cache& put (char* data, unsigned long amount);

/**	Gets data from the cache storage.

	Data is copied out of cache storage starting at the next
	user data location. If the amount of user data to get is
	greater than the amount_remaining the cache will be refilled
	as needed to obtain more data.

	@param	data	The address where the user data is to be written.
	@param	amount	The amount of data to get, in bytes.
	@return	The amount of data that was copied to the data address.
		The only time this will be less than the amount requested is
		if the end-of-data has been reached.
	@see	next()
	@see	amount_remaining()
	@see	data_test (Data_Test, unsigned long)
*/
virtual unsigned long get (char* data, unsigned long amount);

/**	Resets the cache to its empty state.

	The number of {@link bytes_read()const bytes read} is reset to zero.
*/
virtual void reset ();

/*==============================================================================
	Cache Management
*/
/**	Gets the capacity of the data storage area.

	<b>N.B.</b>: The capacity is the amount of allocated storage space
	allocated to the cache. This is not necessarily the same as the sum
	of amount_used and amount_free if the capacity has been set to an
	amount less than the allocated storage size.

	@return	The number of bytes allocated to data storage.
	@see	capacity(unsigned long)
*/
unsigned long capacity () const
	{return Capacity;}

/**	Sets the <i>effective</i> capacity of the data storage area.

	The <i>effective</i> capacity is the maximum amount of the total
	storage capacity that will be used during a refill.

	If the capacity amount is not being changed then nothing is done.

	<b>N.B.</b>: If the amount is zero, then the presumptive capacity
	is the amount_used after a drain. If this is still zero then
	the storage area will be deallocated.

	A non-zero amount will not be allowed to be less than a non-zero
	effective data margin plus one byte.

	Setting a non-zero amount less than the current capacity will
	change the amount of space available for user data during a refill,
	but the amount of storage space allocated will not be changed. If
	the amount specified is less than the amount_used the cache will be
	drained.

	When the capacity is being increased from its current storage amount
	the cache will first be drained. This will minimize the amount of
	data that needs to be copied to the new storage when the capacity is
	increased, and minimize potential loss of user data when the capacity
	is being increased. However, if the capacity is being reduced - which
	does not require storage reallocation and copying - and the new
	amount still has room for the current user data contents, the
	contents will not be drained.

	@param	amount The <i>effective</i> capacity, in bytes, of the
		storage area.
	@return	This Cache.
	@see	drain()
	@see	amount_used()
*/
Cache& capacity (unsigned long amount);

/**	Gets the amount of cache storage in use.

	@return	The amount, in bytes, of storage occupied by user data.
*/
unsigned long amount_used () const
	{return Last - Start;}

/**	Gets the amount of free cache space.

	<b>N.B.</b>: The amount of free space does not necessarily
	include all of the remaining storage capacity. Rather, it is
	the amount available for use during a refill.

	@return	The amount, in bytes, of storage not occupied by user data
		that is available for use by a refill.
*/
unsigned long amount_free () const
	{return End - Last;}

/**	Gets the starting address of the cache data storage area.

	Any user data will start at this address.

	@return	The start address of the cache storage area. <b>N.B.</b>:
		This will be NULL if the cache has no data storage.
*/
char* start () const
	{return Start;}

/**	Sets the location of the next user data.

	All data before the next pointer is considered to be "consumed" by
	the user and will be logically removed from the cache when it is
	drained. However, the effective data margin will always be retained
	in the cache during a drain regardless of the location of the next
	pointer.

	The next pointer provides the user with the means of logically moving
	in the cache to the next data location. The next pointer may be
	safely moved forward to any location, including locations beyond the
	last of the user data or the end of the cache storage. It may also be
	set back to a location before its current location, but will be
	constrained to an address that is greater than or equal to the start
	of the cache storage.

	@param	location	The new address of the next data pointer.
	@return	This Cache.
	@see	drain()
	@see	data_margin()
*/
Cache& next (char* location)
	{Next = (location > Start) ? location : Start; return *this;}

/**	Gets the address of the next user data.

	<b>N.B.</b>: The next pointer will always be at or beyond the start
	of the cache storage area, however it may also be located beyond the
	last valid user datum or the end of the cache storage area.

	@return	A pointer to the next user data location. <b>N.B.</b>: This
		will be NULL if the cache has no data storage.
*/
char* next () const
	{return Next;}

/**	Gets the amount of user data used (consumed).

	<b>N.B.</b>: Since the next user data pointer may be at any
	location beyond the start of the cache, it is possible to obtain a
	value that is greater than the amount_used or the capacity.

	@return	The distance, in bytes, from the start of the cache to the
		next user data.
*/
unsigned long amount_consumed () const
	{return Next - Start;}

/**	Gets how much more user data is available.

	<b>N.B.</b>: Since the next user data pointer may be at any
	location beyond the start of the cache, it is possible to obtain
	a negative value.

	@return	The distance, in bytes, from the next user datum to the
		end (last) of user data.
*/
long amount_remaining () const
	{return Last - Next;}

/**	Gets the address where the user data ends.

	@return	A pointer to the byte following the last user datum in the
		cache. <b>N.B.</b>: This will be NULL if the cache has no data
		storage.
*/
char* last () const
	{return Last;}

/**	Sets the location where the user data ends.

	<b>Caution</b>: User data will be effectively deleted - if the new
	location is less than the current location - or generated (from
	remnant cache storage contents) - if the location is increased.

	@param	location	The address of the last (exclusive) user
		data in the storage area.
	@return	This Cache.
*/
Cache& last (char* location);

/**	Generates a report on a stream of the current cache state.

	@param	stream	The ostream where the report will be written.
*/
void state_report (std::ostream& stream) const;

/**	Generates a report on stdout of the current cache state.

	This is a convenience method that just passes cout to the
	state_report(std::ostream&) method.

	@see	state_report(std::ostream&)
*/
void state_report () const;

/*==============================================================================
	Data members:
*/
private:
//!	Source of data for refills.
std::istream*
	Source;

//!	Total number of bytes read from the Source.
unsigned long long
	Bytes_Read;

//!	Minimum amount of data to retain during a drain.
unsigned long
	Data_Margin;

//!	Function to test for logical end-of-data during a refill. 
Data_Test
	EOD_Test;

//!	The amount of data required by the Data_Test function.
unsigned long
	Data_Test_Amount;

//!	Capacity of the allocated storage area.
unsigned long
	Capacity;

//	Cache management pointers:

//!	Start (inclusive) of the data storage area.
char*
	Start;
//!	Next user data location; data before this location are "consumed".
char*
	Next;
//!	Last user data location; no valid user data at and beyond this location.
char*
	Last;
//!	End (exclusive) of the data storage area.
char*
	End;

};	//	End of Cache class.
}	//	namespace PIRL
#endif
