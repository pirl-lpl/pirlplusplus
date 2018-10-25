/*	Cache

PIRL CVS ID: $Id: Cache.cc,v 1.10 2010/11/11 20:41:40 castalia Exp $

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
#include	"Cache.hh"
using namespace PIRL;

#include	<iostream>
using std::istream;
using std::cin;
using std::cout;
using std::endl;
using std::streamoff;

#include	<iomanip>
using std::hex;
using std::dec;
using std::setw;
using std::setfill;

#include	<sstream>
using std::ostringstream;

#include    <stdexcept>
using std::length_error;
using std::overflow_error;

#include	<cstring>

#if defined (DEBUG)
/*******************************************************************************
	DEBUG controls

	DEBUG report selection options.
	Define any of the following options to obtain the desired debug reports:
*/
#define DEBUG_ALL			(-1)
#define DEBUG_CONSTRUCTORS	(1 << 0)
#define DEBUG_ACCESSORS		(1 << 1)
#define DEBUG_MANIPULATORS	(1 << 2)
#define DEBUG_MANAGERS		(1 << 3)

using std::clog;
#include	<iomanip>
using std::uppercase;
using std::nouppercase;
using std::boolalpha;
#endif	//	DEBUG

/*******************************************************************************
	Cache
*/
/*==============================================================================
	Constants:
*/
const char* const
	Cache::ID =
		"PIRL::Cache ($Revision: 1.10 $ $Date: 2010/11/11 20:41:40 $)";

/*==============================================================================
	Constructors
*/
Cache::Cache ()
	:	Source (&cin),
		Bytes_Read (0),
		Data_Margin (0),
		EOD_Test (NULL),
		Data_Test_Amount (0),
		Capacity (0),
		Start (NULL)
{
reset ();
capacity (0);
}

Cache::Cache
	(
	unsigned long	amount,
	unsigned long	data_margin,
	Data_Test		data_test
	)
	:	Source (&cin),
		Bytes_Read (0),
		Data_Margin (data_test ? 0 : data_margin),
		EOD_Test (data_test),
		Data_Test_Amount (data_test ? data_margin : 0),
		Capacity (0),
		Start (NULL)
{
reset ();
capacity (amount);
}

Cache::Cache
	(
	unsigned long	amount,
	std::istream&	source,
	unsigned long	data_margin,
	Data_Test		data_test
	)
	:	Source (&source),
		Bytes_Read (0),
		Data_Margin (data_test ? 0 : data_margin),
		EOD_Test (data_test),
		Data_Test_Amount (data_test ? data_margin : 0),
		Capacity (0),
		Start (NULL)
{
reset ();
capacity (amount);
}


//	Destructor
Cache::~Cache ()
{
delete [] Start;
}

/*==============================================================================
	Accessors
*/
Cache&
Cache::data_margin
	(
	unsigned long	amount
	)
{
if ((Data_Margin = amount) &&
	(Data_Margin + 1) > capacity ())
	//	Reallocate a larger data storage area.
	capacity (amount + 1);
return *this;
}


Cache&
Cache::data_test
	(
	Data_Test		tester,
	unsigned long	data_amount
	)
{
#if ((DEBUG) & DEBUG_ACCESSORS)
clog << ">>> Cache::data_test:" << endl
	 << "    Tester " << (tester ? "" : "not ") << "specified" << endl
	 << "    Data amount = " << data_amount << endl;
#endif
if ((EOD_Test = tester))
	{
	 if (! (Data_Test_Amount = data_amount))
	 	{
		#if ((DEBUG) & DEBUG_ACCESSORS)
		if (tester)
			clog << "    No tester because no data amount" << endl;
		#endif
		EOD_Test = NULL;
		}
	else if (Data_Test_Amount > capacity ())
		capacity (Data_Test_Amount);
	}
else
	{
	#if ((DEBUG) & DEBUG_ACCESSORS)
	if (data_amount)
		clog << "    No data amount because no tester" << endl;
	#endif
	Data_Test_Amount = 0;
	}
return *this;
}

/*==============================================================================
	Manipulators
*/
void
Cache::drain ()
{
#if ((DEBUG) & DEBUG_MANIPULATORS)
clog << ">>> Cache::drain:" << endl;
state_report (clog);
#endif
if (Next > Start &&
	//	Must retain any data margin.
	(unsigned long)(Last - Start) > data_margin ())
	{
	//	Effective next location.
	char*
		next = Last - data_margin ();
	if (next > Next)
		next = Next;

	//	Shift (next - Last] to the front of the cache.
	#if ((DEBUG) & DEBUG_MANIPULATORS)
	clog << "    Shift " << (unsigned long)(Last - next) << " bytes" << endl
		 << "     from " << setw (2 * sizeof (void*))
			<< (void*)next << endl
		 << "       to " << setw (2 * sizeof (void*))
			<< (void*)Start << endl;
	#endif
	memcpy (Start, next, Last - next);

	//	Shift the data pointers.
	Next = Start + ((next == Next) ? 0 : (Next - next));
	Last -= next - Start;
	#if ((DEBUG) & DEBUG_MANIPULATORS)
	state_report (clog);
	#endif
	}
#if ((DEBUG) & DEBUG_MANIPULATORS)
clog << "<<< Cache::drain" << endl;
#endif
}


bool
Cache::refill
	(
	unsigned long	max_amount
	)
{
if (! Start)
	return false;
#if ((DEBUG) & DEBUG_MANIPULATORS)
clog << ">>> Cache::refill" << endl;
#endif
if (amount_free () >= Data_Test_Amount &&
	EOD_Test &&
	EOD_Test (Last))
	{
	//	Already at end-of-data (logical EOF).
	#if ((DEBUG) & DEBUG_MANIPULATORS)
	clog << "    Last at logical end-of-data." << endl
		 << "<<< Cache::refill: false" << endl;
	#endif
	return false;
	}

if (amount_free () < max_amount)
	//	Drain out any consumed data to maximize the free space.
	drain ();

unsigned long
	amount = amount_free ();
if (amount > max_amount)
	amount = max_amount;
if (amount)
	{
	//	Read data from the Source.
	#if ((DEBUG) & DEBUG_MANIPULATORS)
	clog << "    Get " << amount << " bytes" << endl;
	#endif
	Source->read (Last, amount);

	//	The amount actually read.
	unsigned long
		count = (unsigned long)Source->gcount ();
	#if ((DEBUG) & DEBUG_MANIPULATORS)
	clog << "    Got " << count << " bytes" << endl;
	#endif
	//	Update the last data pointer and amount read.
	Last       += count;
	Bytes_Read += count;

	if (Source->bad ())
		{
		ostringstream
			message;
		message << ID << endl
				<< "Unable to read " << amount << " data byte"
					<< ((amount != 1) ? "s." : ".") << endl
				<< "An error condition was encountered after reading "
					<< count << " byte"
					<< ((count != 1) ? "s." : ".") << endl;
		throw std::ios::failure (message.str ());
		}

	//	Check for an EOF condition:

	if (! count &&
		Source->eof ())
		{
		//	Nothing read; at physical EOF.
		#if ((DEBUG) & DEBUG_MANIPULATORS)
		clog << "    EOF" << endl;
		#endif
		amount = 0;
		}

	else if (EOD_Test &&
			(unsigned long)(Last - Start) > (data_test_amount () - 1))
		{
		//	Scan the new data for an end-of-data (logical EOF) condition.
		char
			*last = Last - Data_Test_Amount + 1,
			*next = ((Start + count) < last) ? (last - count) : Start;
		while (next < last)
			if (EOD_Test (next++))
				break;
		if (next != last)
			{
			//	Backup the Source to the end-of-data location in the stream.
			streamoff
				offset = --next - Last;	//	Careful! Pointer arithmetic.
			#if ((DEBUG) & DEBUG_MANIPULATORS)
			clog << "    Logical end of data - " <<endl
				 << "      total read: "
				 	<< setw (6) << Bytes_Read << endl
				 << "          backup: "
				 	<< setw (6) << -offset << endl
				 << "       effective: "
				 	<< setw (6) << (Bytes_Read + offset) << endl
				 << "    stream state: ";
			if (*Source)
				clog << "good" << endl;
			else
				clog << (Source->eof () ? "EOF " : "")
					 << (Source->fail () ? "fail " : "")
					 << (Source->bad () ? "bad" : "")
					 << endl;
			#endif
			Last = next;
			Bytes_Read += (unsigned long long)offset;
			amount = count + (unsigned long)offset;	//	Effective amount read.

			Source->seekg (offset, std::ios::cur);
			if (! *Source)
				{
				//	Couldn't reposition the stream. Try unget instead.
				#if ((DEBUG) & DEBUG_MANIPULATORS)
					clog << "    Unable to reposition the stream: "
						 << (Source->eof () ? "EOF " : "")
						 << (Source->fail () ? "fail " : "")
						 << (Source->bad () ? "bad" : "")
						 << endl;
				#endif
				Source->clear ();
				while (Source->unget () && ++offset);
				if (! *Source)
					{
					//	Couldn't backup all the way.
					Bytes_Read += count;
					ostringstream
						message;
					message << ID << endl
							<< "Unable to backup the stream "
								<< "after end-of-data found." << endl
							<< -offset << " byte stream overflow." << endl;
					throw overflow_error (message.str ());
					}
				}
			}
		}
	}
else
	//	The cache is already full.
	amount = 1;

#if ((DEBUG) & DEBUG_MANIPULATORS)
clog << "<<< Cache::refill: " << boolalpha << (bool)amount << endl;
#endif
return (bool)amount;
}


Cache&
Cache::put
	(
	char*			data,
	unsigned long	amount
	)
{
if (amount > amount_free ())
	{
	//	Try to get more free space.
	drain ();
	if (amount > amount_free ())
		//	Enlarge the cache.
		capacity (amount_used () + amount);
	}

//	Copy in the new user data.
memcpy (Last, data, amount);
Last += amount;

return *this;
}


unsigned long
Cache::get
	(
	char*			data,
	unsigned long	amount
	)
{
unsigned long
	got = 0;
while (got < amount)
	{
	while (Next >= Last && refill ());
	if (Next >= Last)
		break;	//	No more data.
	//	Copy out the user data.
	while (Next < Last &&
			++got < amount)
		*data++ = *Next++;
	}
return got;
}


void
Cache::reset ()
{
#if ((DEBUG) & DEBUG_MANIPULATORS)
clog << ">-< Cache::reset" << endl;
#endif
Bytes_Read	= 0;
Next		=
Last		= Start;
}

/*==============================================================================
	Cache Management
*/
Cache&
Cache::capacity
	(
	unsigned long	amount
	)
{
#if ((DEBUG) & DEBUG_MANAGERS)
clog << ">>> Cache::capacity: " << amount << endl;
#endif
if (amount == Capacity)
	{
	End = Start + amount;
	#if ((DEBUG) & DEBUG_MANAGERS)
	clog << hex << uppercase << setfill ('0')
		 << "    Using all effective capacity." << endl
		 << "             Start @ 0x" << setw (2 * sizeof (void*))
	 		<< (void*)Start << endl
		 << "               End @ 0x" << setw (2 * sizeof (void*))
	 		<< (void*)End << endl
		 << dec << nouppercase << setfill (' ')
		 << "<<< Cache::capacity: " << Capacity << endl;
	#endif
	return *this;
	}

if (amount &&
	amount < Capacity)
	{
	//	Just shorten the effective capacity.
	#if ((DEBUG) & DEBUG_MANAGERS)
	clog << hex << uppercase << setfill ('0')
		 << "    Reducing effective capacity." << endl
		 << "             Start @ 0x" << setw (2 * sizeof (void*))
	 		<< (void*)Start << endl
		 << dec << nouppercase << setfill (' ');
	#endif
	if (data_margin () &&
		amount <= data_margin ())
		{
		//	Guarantee at least the non-zero data margin plus one byte.
		amount = data_margin () + 1;
		#if ((DEBUG) & DEBUG_MANAGERS)
		clog << "    Allow for data_margin + 1: " << amount << endl;
		#endif
		}
	if ((End = Start + amount) < Last)
		{
		//	Drain the cache to maximize free space.
		drain ();
		if (End < Last)
			{
			//	Prevent data loss.
			End = Last;
			#if ((DEBUG) & DEBUG_MANAGERS)
			clog << "    Prevent data loss." << endl;
			#endif
			}
		}
	#if ((DEBUG) & DEBUG_MANAGERS)
	clog << hex << uppercase << setfill ('0')
		 << "               End @ 0x" << setw (2 * sizeof (void*))
	 		<< (void*)End << endl
		 << dec << nouppercase << setfill (' ')
		 << "<<< Cache::capacity: effective - " << amount << endl;
	#endif
	return *this;
	}

if (Start)
	//	Drain the cache to minimize the amount of used space.
	drain ();

if (! amount)
	{
	/*	Shrink the cache storage area to the minimum amount necessary.

		Note that this could be zero which will result in no storage.
	*/
	amount = amount_used ();
	#if ((DEBUG) & DEBUG_MANAGERS)
	clog << "    Shrink wrap storage to data." << endl;
	#endif
	}
if (data_margin () &&
	amount <= data_margin ())
	{
	//	Guarantee at least the non-zero data margin plus one byte.
	amount = data_margin () + 1;
	#if ((DEBUG) & DEBUG_MANAGERS)
	clog << "    Allow for data_margin + 1: " << amount << endl;
	#endif
	}

if (amount == Capacity)
	End = Start + amount;
else
	{
	char*
		start = NULL;
	if (amount)
		{
		//	Allocate new storage.
		#if ((DEBUG) & DEBUG_MANAGERS)
		clog << "    Allocate new storage: " << amount << endl;
		#endif
		start = new char[amount];
		if (amount_used ())
			{
			//	Copy the user data.
			#if ((DEBUG) & DEBUG_MANAGERS)
			clog << "    Copy old storage: " << amount_used () << endl;
			#endif
			memcpy (start, Start, amount_used ());
			}
		}

	//	Replace the old storage.
	delete [] Start;
	Next  = start + (Next - Start);
	Last  = start + (Last - Start);
	End   = start + amount;
	Start = start;
	Capacity = amount;
	#if ((DEBUG) & DEBUG_MANAGERS)
	state_report (clog);
	#endif
	}
#if ((DEBUG) & DEBUG_MANAGERS)
clog << "<<< Cache::capacity: " << Capacity << endl;
#endif
return *this;
}


Cache&
Cache::last
	(
	char*	location
	)
{
Last = (location < Start) ? Start
	 : (location > End)   ? End
	 :  location;
return *this;
}


void
Cache::state_report
	(
	std::ostream&	stream
	)
	const
{
static int
	LABEL   = 11,
	ADDRESS = (2 * sizeof (void*)) + 2,
	VALUE   = 7;
char
	filler = stream.fill ();
stream
	<< setfill (' ')
	<< setw (LABEL) << "Start @ "
		<< setw (ADDRESS) << (void*)Start << endl
	<< setw (LABEL) << "Next @ "
		<< setw (ADDRESS) << (void*)Next
		<< ", used = " << dec << setfill (' ')
		<< setw (VALUE) << (unsigned int)(Next - Start) << endl
	<< setw (LABEL) << "Last @ "
		<< setw (ADDRESS) << (void*)Last
		<< ", more = " << dec << setfill (' ')
		<< setw (VALUE) << (unsigned int)(Last - Next) << endl
	<< setw (LABEL) << "End @ "
		<< setw (ADDRESS) << (void*)End
		<< ", free = " << dec << setfill (' ')
		<< setw (VALUE) << (unsigned int)(End - Last) << endl
	<< setw (LABEL + ADDRESS + 9) << "Capacity = "
		<< setw (VALUE) << Capacity << endl
	<< setw (LABEL + ADDRESS + 9) << "Read = "
		<< setw (VALUE) << Bytes_Read << endl;
stream.fill (filler);
}

void
Cache::state_report () const
{state_report (cout);}
