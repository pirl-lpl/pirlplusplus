/*	Binary_IO

PIRL CVS ID: $Id: Binary_IO.hh,v 1.17 2010/11/11 20:31:46 castalia Exp $

Copyright (C) 2003-2010  Arizona Board of Regents on behalf of the
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
#ifndef _binary_stream_
#define _binary_stream_

#include	<istream>
#include	<ostream>

#include	"endian.hh"

#if defined (DEBUG)
/*=*****************************************************************************
	DEBUG controls
*/
#define DEBUG_BINARY_IO		(1 << 13)

//	Default DEBUG value
#if (DEBUG +0 == DEBUG_BINARY_IO)
#define BINARY_IO_DEBUG
#endif

#include	<iostream>
using std::cerr;

#include	<iomanip>
using std::endl;

#endif	//	DEBUG


namespace PIRL
{
/*=*****************************************************************************
	Data_Binder
*/
/**	A <i>Data_Binder</i> binds an object to data of some amount.

	A Data_Binder contains a reference to the bound object and the data
	location and amount bound to it. A Data_Binder is used to
	convey a relationship between some object and some data; typically
	as an intermediate, temporary object during the evaluation of a
	operation. For example, a stream manipulator can bind a stream to
	the data being manipulated.

	@author		Bradford Castalia, UA/PIRL, 12/03

	$Revision: 1.17 $
*/
template<typename Bindor_object>
class Data_Binder
{
public:
//!	The object that is being bound.
Bindor_object&
	Bindor;

//!	Pointer (char*) to the data being bound.
mutable char*
	Data;
//!	Amount (bytes) of data being bound.
unsigned long
	Amount;

/**	Constructs a Data_Binder that binds an object to data.

	@param	bindor	A reference to the Bindor_object being bound.
	@param	data	A pointer to the data being bound.
	@param	amount	The amount (bytes) of data being bound.
*/
Data_Binder
	(
	const Bindor_object&	bindor,
	char*					data,
	unsigned long			amount
	)
	:	Bindor (const_cast<Bindor_object&>(bindor)),
		Data (data),
		Amount (amount)
{}
};

/*=*****************************************************************************
	Binary_IO
*/
/**	The <i>Binary_IO</i> class provides input and output of binary data
	values.

	The data may optionally be reversed to handle byte ordering
	differences between the host system that writes the binary data and
	the host system that reads it.

	A stream manipulator interface is provided. This provides an operator()
	method that takes an argument of any type (for which a reference
	can be provided) and returns a Binder, and the external
	operator&lt;&lt; and operator&gt;&gt; functions that take an
	ostream or istream, repsectively, and a Binder. For example:

	<code>
	ofstream output (filename, ios::out | ios::binary);<br>
	Binary_IO binary (REVERSED);<br>
	int number = 42;<br>
	output &lt;&lt; binary (number);<br>
	</code>

	This will write the binary value of the int number to the stream with
	the bytes of the value reversed. On a low-endian host this will
	result in MSB-ordered output data.

	<b>Note</b>: The argument of the manipulator must be a referencable
	variable, not a literal constant (use a temporary variable for
	constant output). Also, because the manipulator does not know about
	the structure of complex objects, only primitive data types should be
	reversed during I/O (use a Data_Block to define complex binary
	structures that can be properly reordered during I/O).

	The get and put methods, which are used by the input and output
	operator methods from the bound manipulator, may be used directly.
	Their advantage over direct use of the ostream.read or istream.write
	methods is that they correctly handle data input reversing and
	non-destructive output data reversing depending on the Binary_IO
	object Reversed mode.

@author		Bradford Castalia, UA/PIRL, 12/03

$Revision: 1.17 $ 
*/
class Binary_IO
{
public:
/*==============================================================================
	Types
*/
typedef std::istream& (* Reader) (std::istream&, char*, unsigned long);
typedef std::ostream& (* Writer) (std::ostream&, const char*, unsigned long);

typedef Data_Binder<Binary_IO>	  Binder;

/*==============================================================================
	Constants
*/
//!	Class identification name with source code version and date.
static const char*
	ID;

//!	External data order selection.
enum Data_Order
	{
	MSB,
	LSB
	};

//!	Convenience symbols.
#define REVERSED	true
#define NATIVE		false

/*==============================================================================
	Constructors
*/
/**	Construct a Binary_IO object based on data reversal specification.

	@param	reverse	true if data is to be reversed on IO; false
		otherwise.
*/
explicit Binary_IO
	(
	bool		reverse = false
	)
{reversed (reverse);}

/**	Construct a Binary_IO object based on the external data order.

	@param	data_order	A #Data_Order value of either
		MSB or LSB.
*/
explicit Binary_IO
	(
	Data_Order	data_order
	)
{IO (data_order);}

/*==============================================================================
	Accessors
*/
/**	Tests if data is reversed during I/O.

	@return	true if the data is reversed; false otherwise.
*/
bool reversed () const
{return Reversed;}

/**	Sets the data reversal mode.

	@param	reverse	true if the data is to be reversed; false otherwise.
*/
Binary_IO& reversed (bool reverse)
{
if ((Reversed = reverse))
	{
	read = &read_backwards;
	write = &write_backwards;
	}
else
	{
	read = &read_forwards;
	write = &write_forwards;
	}
return *this;
}

/**	Gets the external data ordering.

	@return	A Data_Order value of either MSB or LSB.
*/
Data_Order IO ()
{
if (Reversed)
	return low_endian_host () ? MSB : LSB;
return high_endian_host () ? MSB : LSB;
}

/**	Sets the external data ordering.

	@param	data_order	A Data_Order value of either MSB or LSB.
*/
Binary_IO& IO (Data_Order data_order)
{
return reversed ((data_order == MSB) ?
	low_endian_host () :
	high_endian_host ());
}

/*==============================================================================
	Manipulator interface
*/
/**	Functor that binds a data value to this Binary_IO.

	@param	value	A reference to a data value of arbitrary type.
	@return	The Binder object that contains a reference to this
		Binary_IO object and the data's address and amount.
	@see	Data_Binder
*/
template<typename T>
Binder operator() (T& value) const
{return Binder (*this, reinterpret_cast<char*>(&value), sizeof (T));}

/*------------------------------------------------------------------------------
	IO
*/
/*..............................................................................
	Primary get/put generic interface
*/
/**	Input from a stream to a value of any type.
*/
template<typename T>
Binary_IO& get (std::istream& stream, T& value)
{
read (stream, reinterpret_cast<char*>(&value), sizeof (T));
return *this;
}

/**	Output to a stream from a value of any type.
*/
template<typename T>
Binary_IO& put (std::ostream& stream, T& value)
{
write (stream, reinterpret_cast<const char*>(&value), sizeof (T));
return *this;
}

/**	Input from a stream to an array of values of any type.
*/
template<typename T>
Binary_IO& get (std::istream& stream, T* value, unsigned int amount)
{
if (sizeof (T) == 1)
	read (stream, reinterpret_cast<char*>(value), amount);
else
	{
	//	Read each array element
	while (amount--)
		{
		read (stream, reinterpret_cast<char*>(value), sizeof (T));
		++value;
		}
	}
return *this;
}

/**	Output to a stream from an array of values of any type.
*/
template<typename T>
Binary_IO& put (std::ostream& stream, T* value, unsigned int amount)
{
if (sizeof (T) == 1)
	write (stream, reinterpret_cast<const char*>(value), sizeof (T));
else
	{
	//	Write each array element
	while (amount--)
		{
		write (stream, reinterpret_cast<const char*>(value), sizeof (T));
		++value;
		}
	}
return *this;
}

/**	Input of 3 binary bytes to LSBs of an integer value.
*/
Binary_IO& get_3 (std::istream& stream, int& value)
{
static const int
	offset = (high_endian_host () ? 1 : 0);
read (stream, reinterpret_cast<char*>(&value) + offset, 3);
return *this;
}

/**	Output of 3 binary bytes from LSBs of an integer value.
*/
Binary_IO& put_3 (std::ostream& stream, const int& value)
{
static const int
	offset = (high_endian_host () ? 1 : 0);
write (stream, reinterpret_cast<const char*>(&value) + offset, 3);
return *this;
}

/*..............................................................................
	Virtual read/write interface
*/
/**	Reads some amount of data bytes into an address from a stream.

	The data is read in the correct order.

	@param	stream	The istream from which to read bytes.
	@param	data	The address (char*) to receive the data bytes.
	@param	amount	The number of data bytes to be read.
	@return	The stream.
*/
Reader
	read;

/**	Writes some amount of data bytes from an address into a stream.

	The data is written in the correct order.

	@param	stream	The ostream into which to write bytes.
	@param	data	The address (char*) from which to get the data bytes.
	@param	amount	The number of data bytes to be written.
	@return	The stream.
*/
Writer
	write;

/*..............................................................................
	Static read/write_forwards/backwards system IO interface
*/
protected:
/**	Reads some amount of data bytes into an address from a stream.

	The data is read in forwards (native) order.

	@param	stream	The istream from which to read bytes.
	@param	data	The address (char*) to receive the data bytes.
	@param	amount	The number of data bytes to be read.
	@return	The stream.
*/
static std::istream&
read_forwards (std::istream& stream, char* data, unsigned long amount)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< Binary_IO::read_forwards: "
	 << "    data @ " << (void*)data << ", amount = " << amount << endl;
#endif
return stream.read (data, amount);
}

/**	Reads some amount of data bytes into an address from a stream.

	The data is read in reverse order.

	@param	stream	The istream from which to read bytes.
	@param	data	The address (char*) to receive the data bytes.
	@param	amount	The number of data bytes to be read.
	@return	The stream.
*/
static std::istream&
read_backwards (std::istream& stream, char* data, unsigned long amount)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< Binary_IO::read_backwards: "
	 << "    data @ " << (void*)data << ", amount = " << amount << endl;
#endif
data += amount;
while (amount--)
	stream.get (*--data);
return stream;
}

/**	Writes some amount of data bytes from an address into a stream.

	The data is written in forward (native) order.

	@param	stream	The ostream into which to write bytes.
	@param	data	The address (char*) from which to get the data bytes.
	@param	amount	The number of data bytes to be written.
	@return	The stream.
*/
static std::ostream&
write_forwards (std::ostream& stream, const char* data, unsigned long amount)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< Binary_IO::write_forwards: "
	 << "    data @ " << (void*)data << ", amount = " << amount << endl;
#endif
return stream.write (data, amount);
}

/**	Writes some amount of data bytes from an address into a stream.

	The data is written in backwards (reverse) order.

	@param	stream	The ostream into which to write bytes.
	@param	data	The address (char*) from which to get the data bytes.
	@param	amount	The number of data bytes to be written.
	@return	The stream.
*/
static std::ostream&
write_backwards (std::ostream& stream, const char* data, unsigned long amount)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< Binary_IO::write_backwards: "
	 << "    data @ " << (void*)data << ", amount = " << amount << endl;
#endif
data += amount;
while (amount--)
	stream.put (*--data);
return stream;
}

//..............................................................................
private:

//!	Data value byte reversal flag.
bool
	Reversed;

};	//	class Binary_IO

/*=*****************************************************************************
	Input and Output subclases
*/
/**	<i>Binary_Input</i> is a subclass of Binary_IO that contains
	an istream reference.
*/
struct Binary_Input
:	public Binary_IO
{
explicit Binary_Input (std::istream& stream, bool reversed = false)
	:	Binary_IO (reversed),
		Stream (stream)
{}

explicit Binary_Input (std::istream& stream, Data_Order data_order)
	:	Binary_IO (data_order),
		Stream (stream)
{}

/**	Input of a value of any type.
*/
template<typename T>
Binary_Input& get (T& value)
{
Binary_IO::read (Stream, reinterpret_cast<char*>(&value), sizeof (T));
return *this;
}

/**	Input of an array of any type.
*/
template<typename T>
Binary_Input& get (T* value, unsigned int amount)
{
Binary_IO::get (Stream, value, amount);
return *this;
}

Binary_Input& get_3 (int& value)
{
Binary_IO::get_3 (Stream, value);
return *this;
}

std::istream&
	Stream;
};	//	class Binary_Input

/**	<i>Binary_Output</i> is a subclass of Binary_IO that contains
	an ostream reference.
*/
struct Binary_Output
:	public Binary_IO
{
explicit Binary_Output (std::ostream& stream, bool reversed = false)
	:	Binary_IO (reversed),
		Stream (stream)
{}

explicit Binary_Output (std::ostream& stream, Data_Order data_order)
	:	Binary_IO (data_order),
		Stream (stream)
{}

/**	Output of a value of any type.
*/
template<typename T>
Binary_Output& put (T& value)
{
Binary_IO::write (Stream, reinterpret_cast<const char*>(&value), sizeof (T));
return *this;
}

/**	Output of an array of any type.
*/
template<typename T>
Binary_Output& put (T* value, unsigned int amount)
{
Binary_IO::put (Stream, value, amount);
return *this;
}

Binary_Output& put_3 (const int& value)
{
Binary_IO::put_3 (Stream, value);
return *this;
}

std::ostream&
	Stream;
};	//	class Binary_Output

/*==============================================================================
	Binary_IO methods bound to the Binary_IO Binder.
*/
/**	Implements the stream input Binary_IO manipulator operator.
*/
inline std::istream&
operator>> (std::istream& stream, const Binary_IO::Binder& binder)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< operator>> (std::ostream& stream, const Binary_IO::Binder& binder)"
	 << endl
	 << "    binder.Data @ " << (void*)(binder.Data) << endl
	 << "    binder.Amount = " << binder.Amount << endl;
#endif
return binder.Bindor.read (stream, binder.Data, binder.Amount);
}

/**	Implements the stream output Binary_IO manipulator operator.
*/
inline std::ostream&
operator<< (std::ostream& stream, const Binary_IO::Binder& binder)
{
#ifdef BINARY_IO_DEBUG
cerr << ">-< operator<< (std::ostream& stream, const Binary_IO::Binder& binder)"
	 << endl
	 << "    binder.Data @ " << (void*)(binder.Data) << endl
	 << "    binder.Amount = " << binder.Amount << endl;
#endif
return binder.Bindor.write (stream, binder.Data, binder.Amount);
}

}	//	namespace PIRL
#endif
