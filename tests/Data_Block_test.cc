/*	Data_Block_test

PIRL CVS ID: $Id: Data_Block_test.cc,v 1.10 2010/11/16 02:09:57 castalia Exp $

Copyright (C) 2003  Arizona Board of Regents on behalf of the Planetary
Image Research Laboratory, Lunar and Planetary Laboratory at the
University of Arizona.

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License, version 2.1,
as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <cstring>
#include <string.h>
#include <exception>
#include <stdexcept>
#include <cstdlib>
using namespace std;

#include "Data_Block.hh"
#include "endian.hh"
using namespace PIRL;

char
	test_char = 1,
	result_char;
short int
	test_short_int = 1,
	result_short_int;
int
	test_int = 1,
	result_int;
unsigned int
	test_unsigned_int = 1,
	result_unsigned_int;
long int
	test_long_int = 1,
	result_long_int;
float
	test_float = 1.1f,
	result_float;
double
	test_double = 1.1,
	result_double;

Data_Block
	block;

char
	store[1024];

enum
	{
	CHAR,
	SHORT_INT,
	INT,
	UNSIGNED_INT,
	LONG_INT,
	FLOAT,
	DOUBLE,
	INT_ARRAY
	};

static const Data_Block::Index
	sizes[] =
		{
		sizeof (char),
		sizeof (short int),
		sizeof (int),
		sizeof (unsigned int),
		sizeof (long int),
		sizeof (float),
		sizeof (double),
		3 * sizeof (int),
		0
		},
	offsets[] =
		{
		0,
		sizeof (char),
		sizeof (char) + sizeof (short int),
		sizeof (char) + sizeof (short int) + sizeof (int),
		sizeof (char) + sizeof (short int) + sizeof (int)
			+ sizeof (unsigned int),
		sizeof (char) + sizeof (short int) + sizeof (int)
			+ sizeof (unsigned int) + sizeof (long int),
		sizeof (char) + sizeof (short int) + sizeof (int)
			+ sizeof (unsigned int) + sizeof (long int)
			+ sizeof (float),
		sizeof (char) + sizeof (short int) + sizeof (int)
			+ sizeof (unsigned int) + sizeof (long int)
			+ sizeof (float) + sizeof (double),
		sizeof (char) + sizeof (short int) + sizeof (int)
			+ sizeof (unsigned int) + sizeof (long int)
			+ sizeof (float) + sizeof (double) + (3 * sizeof (int)),
		0
		};
static const Data_Block::Index
	indexed_counts[][2] =
		{
		{INT_ARRAY, 3},
		{0,0}
		};
static const int
	total_elements = ((sizeof (sizes) / sizeof (Data_Block::Index)) - 1);


void
put_test ()
{
cout << ">>> Put" << endl;

cout << CHAR << ": " << sizeof (char)
	 << " byte char value =  "
	 << static_cast<unsigned int>(test_char) << endl;
block.put (test_char, CHAR);

cout << SHORT_INT << ": " << sizeof (short int)
	 << " byte short int value =  "
	 << test_short_int << endl;
block.put (test_short_int, SHORT_INT);

cout << INT << ": " << sizeof (int)
	 << " byte int value =  "
	 << test_int << endl;
block.put (test_int, INT);

cout << UNSIGNED_INT << ": " << sizeof (unsigned int)
	 << " byte unsigned int value = "
	 << test_unsigned_int << endl;
block.put (test_unsigned_int, UNSIGNED_INT);

cout << LONG_INT << ": " << sizeof (long int)
	 << " byte long int value =  "
	 << test_long_int << endl;
block.put (test_long_int, LONG_INT);

cout << FLOAT << ": " << sizeof (float)
	 << " byte float value =  "
	 << test_float << endl;
block.put (test_float, FLOAT);

cout << DOUBLE << ": " << sizeof (double)
	 << " byte double value = "
	 << test_double << endl;
block.put (test_double, DOUBLE);

cout << "<<< Put" << endl;
}


int
get_result ()
{
bool
	passed;
int
	result = 0;
cout << ">>> Get" << endl;

result_char = 0;
block.get (result_char, CHAR);
if (passed = (result_char == test_char))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << CHAR << ": " << sizeof (char)
	 << " byte char value =  "
	 << static_cast<unsigned int>(result_char) << " - "
	 << static_cast<unsigned int>(test_char) << endl;

result_short_int = 0;
block.get (result_short_int, SHORT_INT);
if (passed = (result_short_int == test_short_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << SHORT_INT << ": " << sizeof (short int)
	 << " byte short int value =  "
	 << result_short_int << " - " << test_short_int << endl;

result_int = 0;
block.get (result_int, INT);
if (passed = (result_int == test_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << INT << ": " << sizeof (int)
	 << " byte int value =  "
	 << result_int << " - " << test_int << endl;

result_unsigned_int = 0;
block.get (result_unsigned_int, UNSIGNED_INT);
if (passed = (result_unsigned_int == test_unsigned_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << UNSIGNED_INT << ": " << sizeof (unsigned int)
	 << " byte unsigned int value =  "
	 << result_unsigned_int << " - " << test_unsigned_int << endl;

result_long_int = 0;
block.get (result_long_int, LONG_INT);
if (passed = (result_long_int == test_long_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << LONG_INT << ": " << sizeof (long int)
	 << " byte long int value =  "
	 << result_long_int << " - " << test_long_int << endl;

result_float = 0.0;
block.get (result_float, FLOAT);
if (passed = (result_float == test_float))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << FLOAT << ": " << sizeof (float)
	 << " byte float value =  "
	 << result_float << " - " << test_float << endl;

result_double = 0.0;
block.get (result_double, DOUBLE);
if (passed = (result_double == test_double))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << DOUBLE << ": " << sizeof (double)
	 << " byte double value =  "
	 << result_double << " - " << test_double << endl;

//	Different host and data amounts.
cout << "--- Different host and data amounts" << endl;
cout << "    "
	 << sizeof (result_long_int) << " byte host, "
	 << sizeof (test_short_int) << " byte data" << endl;
result_long_int = 0;
block.get (result_long_int, SHORT_INT);
if (passed = (result_long_int == test_short_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << result_long_int << " - " << test_short_int << endl;

cout << "    "
	 << sizeof (result_short_int) << " byte host, "
	 << sizeof (test_long_int) << " byte data" << endl;
result_short_int = 0;
block.get (result_short_int, LONG_INT);
if (passed = (result_short_int == test_long_int))
	++result;
cout << (passed ? "PASS: " : "FAIL: ")
	 << result_short_int << " - " << test_long_int << endl;

#define	TOTAL_GET_TESTS	9
cout << "<<< Get: Passed " << result
		<< " of " << TOTAL_GET_TESTS << " cases" << endl;
return result;
}


void
Done
	(
	int		total,
	int		passed
	)
{
cout << endl
	 << "Checks: " << total << endl
	 << "Passed: " << passed << endl;
exit (total - passed);
}


int
main
	(
	int		count,
	char	**argument
	)
try
{
cout << "*** Data_Block test" << endl
	 << "    " << Data_Block::ID << endl;

bool
	MSB_host = high_endian_host (),
	result,
	passed;
Data_Block::Data_Order
	order;
Data_Block::Index
	*array,
	index;
Data_Block::Value_List
	value_list;
int
	Tests_Total = 0,
	Tests_Passed = 0,
	number;
void*
	address;

cout << "=== Host is " << (MSB_host ? "high" : "low") << " endian." << endl;

cout << endl << "--- Empty Data_Block" << endl;
++Tests_Total;
address = block.data ();
if (passed = (address == NULL))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.data () = " << address << " - "
	 << (void*)NULL << endl;

++Tests_Total;
number = block.elements ();
if (passed = (number == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.elements () = " << number << " - 0" << endl;


//	Structure

cout << endl << "--- Data_Block structure" << endl;

block.data (store);
++Tests_Total;
address = block.data ();
if (passed = (address == store))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.data () = " << address << " - "
	 << (void*)store << endl;

block.element_sizes (sizes);
block.array_indexed_counts (indexed_counts);

++Tests_Total;
number = block.elements ();
if (passed = (number == total_elements))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.elements () = " << number << " - " << total_elements << endl;
	if (! passed)
	Done (Tests_Total, Tests_Passed);

array = block.element_offsets ();
++Tests_Total;
if (passed = (memcmp (array, offsets,
			(total_elements + 1) * sizeof (Data_Block::Index)) == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.element_offsets () = ";
for (int count = 0;
		 count <= total_elements;
		 count++)
	{
	cout << array[count] << '/' << offsets[count];
	if (count != total_elements)
		cout << ", ";
	}
cout << endl;

number = block.count_of (0);
++Tests_Total;
if (passed = (number == 1))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (0) = " << number << " - 1" << endl;

number = block.count_of (INT_ARRAY);
++Tests_Total;
if (passed = (number == indexed_counts[0][1]))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (" << INT_ARRAY << ") = " << number << " - "
	 << indexed_counts[0][1] << endl;


//	Native order
cout << endl << "--- Using native put data order" << endl;
block.native (true);
++Tests_Total;
if (passed = block.native ())
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.native () = " << boolalpha << passed << endl;
order = block.data_order ();
++Tests_Total;
if (passed = (MSB_host ?
		(order == Data_Block::MSB) : (order == Data_Block::LSB)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.data_order () = "
	 << (order == Data_Block::MSB ? "MSB" : "LSB") << endl;

put_test ();
cout << endl << "--- Using native get data order" << endl;
result_unsigned_int = block.get<int> (INT);
++Tests_Total;
if (passed = (result_unsigned_int == test_unsigned_int))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.get<int> (INT) = "
	 << result_unsigned_int << " - " << test_unsigned_int << endl;
result_double = block.get<double> (DOUBLE);
++Tests_Total;
if (passed = (result_double == test_double))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.get<double> (DOUBLE) = "
	 << result_double << " - " << test_double << endl;


Tests_Passed += get_result ();
Tests_Total += TOTAL_GET_TESTS;
cout << endl << "--- Using reversed get data order >>> Should FAIL <<<" << endl;
block.native (false);
Tests_Passed += TOTAL_GET_TESTS - (get_result () - 1);
Tests_Total += TOTAL_GET_TESTS;

//	Reveresed order
cout << endl << "--- Using reversed put data order" << endl;
block.native (false);
++Tests_Total;
if (passed = ! block.native ())
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.native () = " << (! passed) << endl;
order = block.data_order ();
++Tests_Total;
if (passed = (MSB_host ?
		(order == Data_Block::LSB) : (order == Data_Block::MSB)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.data_order () = "
	 << (order == Data_Block::MSB ? "MSB" : "LSB") << endl;

put_test ();
cout << endl << "--- Using native get data order >>> Should FAIL <<<" << endl;
block.native (true);
Tests_Passed += TOTAL_GET_TESTS - (get_result () - 1);
Tests_Total += TOTAL_GET_TESTS;
cout << endl << "--- Using reversed get data order" << endl;
block.native (false);
Tests_Passed += get_result ();
Tests_Total += TOTAL_GET_TESTS;

//	Array
int
	test_array[3] = {3, 5, 7},
	result_array[3];
cout << endl << "--- Array[3] @ " << test_array
	 << " native put data order" << endl;
block.native (true);
block.put (test_array, INT_ARRAY);
cout << endl << "--- Array[3] native get data order" << endl;
result_array[0] = result_array[1] = result_array[2] = 0;
block.get (result_array, INT_ARRAY);
for (int index = 0;
		 index < 3;
		 index++)
	{
	++Tests_Total;
	if (passed = (test_array[index] == result_array[index]))
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "result = " << result_array[index] << " - "
		 << "test = " << test_array[index] << endl;
	}

cout << endl << "--- block.get (INT_ARRAY, result_int, 1)" << endl;
block.get (result_int, INT_ARRAY, 1);
++Tests_Total;
if (passed = (test_array[1] == result_int))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "result = " << result_int << " - "
	 << "test = " << test_array[1] << endl;


//	Utilities
cout << endl << "--- Utilities" << endl;
array = new Data_Block::Index[total_elements + 1];
memcpy (array, block.element_offsets (),
	(total_elements + 1) * sizeof (Data_Block::Index));

Data_Block::offsets_to_sizes (array, total_elements);
++Tests_Total;
if (passed = (memcmp (array, sizes,
			(total_elements + 1) * sizeof (Data_Block::Index)) == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "offsets_to_sizes = ";
for (int count = 0;
		 count <= total_elements;
		 count++)
	{
	cout << array[count] << '/' << sizes[count];
	if (count != total_elements)
		cout << ", ";
	}
cout << endl;
if (! passed)
	Done (Tests_Total, Tests_Passed);

Data_Block::sizes_to_offsets (array, total_elements);
++Tests_Total;
if (passed = (memcmp (array, offsets,
			(total_elements + 1) * sizeof (Data_Block::Index)) == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "sizes_to_offsets = ";
for (int count = 0;
		 count <= total_elements;
		 count++)
	{
	cout << array[count] << '/' << offsets[count];
	if (count != total_elements)
		cout << ", ";
	}
cout << endl;

//	Value_List
cout << endl << "--- Value_List" << endl;
value_list = block.element_offsets_list ();
++Tests_Total;
if (passed = (value_list.size () == (total_elements + 1)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "Offsets Value_List size = " << value_list.size () << " - "
	 << (total_elements + 1) << endl;

++Tests_Total;
if (passed = (memcmp (&value_list[0], offsets,
			(total_elements + 1) * sizeof (Data_Block::Index)) == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.element_offsets_list () = ";
for (int count = 0;
		 count <= total_elements;
		 count++)
	{
	if (value_list.size () > count)
		cout << value_list[count] << '/' << offsets[count];
	else
		cout << "-/" << offsets[count];
	if (count != total_elements)
		cout << ", ";
	}
cout << endl;


//	Insert and delete

cout << endl << "--- Insert element at existing element offset:" << endl;
result = block.insert_element (1, 10, 10);
++Tests_Total;
if (passed = (result == false))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.insert_element (1, 10, 10) = " << result << " - "
	 << false << endl;

number = total_elements + 1;
value_list = block.element_offsets_list ();
++Tests_Total;
if (passed = (value_list.size () == (number + 1)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "Offsets Value_List size = " << value_list.size () << " - "
	 << (number + 1) << endl;

if (passed)
	{
	++Tests_Total;
	passed = (value_list[0] == 0 &&
			  value_list[1] == 1);
	for (int count = 2;
			 count <= number &&
			 	passed;
			 count++)
		passed = (value_list[count] == (offsets[count - 1] + 10));
	if (passed)
		++Tests_Passed;
	}
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.element_offsets_list () = ";
for (int count = 0;
		 count <= number;
		 count++)
	{
	if (count < 2)
		{
		if (value_list.size () > count)
			cout << value_list[count] << '/' << offsets[count];
		else
			cout << "-/" << offsets[count];
		}
	else
		{
		if (value_list.size () > count)
			cout << value_list[count] << '/' << (offsets[count - 1] + 10);
		else
			cout << "-/" << (offsets[count - 1] + 10);
		}
	if (count != number)
		cout << ", ";
	}
cout << endl;

number = block.count_of (1);
++Tests_Total;
if (passed = (number == 10))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (1) = " << number << " - 10" << endl;


cout << endl << "--- Delete inserted element" << endl;
block.delete_element (1);
value_list = block.element_offsets_list ();
++Tests_Total;
if (passed = (value_list.size () == (total_elements + 1)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "Offsets Value_List size = " << value_list.size () << " - "
	 << (total_elements + 1) << endl;

++Tests_Total;
if (passed = (memcmp (&value_list[0], offsets,
			(total_elements + 1) * sizeof (Data_Block::Index)) == 0))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.element_offsets_list () = ";
for (int count = 0;
		 count <= total_elements;
		 count++)
	{
	if (value_list.size () > count)
		cout << value_list[count] << '/' << offsets[count];
	else
		cout << "-/" << offsets[count];
	if (count != total_elements)
		cout << ", ";
	}
cout << endl;


cout << endl << "--- Insert element at end-of-elements" << endl;
result = block.insert_element (offsets[total_elements], 10, 10);
++Tests_Total;
if (passed = (result == false))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.insert_element (" << offsets[total_elements] << ", 10, 10) = "
	 << result << " - " << false << endl;

number = total_elements + 1;
value_list = block.element_offsets_list ();
++Tests_Total;
if (passed = (value_list.size () == (number + 1)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "Offsets Value_List size = " << value_list.size () << " - "
	 << (number + 1) << endl;

if (passed)
	{
	++Tests_Total;
	if (passed =
			(memcmp (&value_list[0], offsets,
				(total_elements + 1) * sizeof (Data_Block::Index)) == 0 &&
			value_list[number] == (offsets[total_elements] + 10)))
		++Tests_Passed;
	}
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.element_offsets_list () = ";
for (int count = 0;
		 count < number;
		 count++)
	{
	if (value_list.size () > count)
		cout << value_list[count] << '/' << offsets[count];
	else
		cout << "-/" << offsets[count];
		cout << ", ";
	}
if (value_list.size () > number)
	cout << value_list[number];
else
	cout << '-';
cout << '/' << (offsets[total_elements] + 10) << endl;

number = block.count_of (total_elements);
++Tests_Total;
if (passed = (number == 10))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (" << total_elements << ") = " << number
	 << " - 10" << endl;

number = block.count_of (INT_ARRAY);
++Tests_Total;
if (passed = (number == indexed_counts[0][1]))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (" << INT_ARRAY << ") = " << number
	 << " - " << indexed_counts[0][1] << endl;


cout << endl << "--- Insert element just before end-of-elements" << endl;
number = block.elements ();
index = block.size () - 1;
result = block.insert_element (index, 20, 2);
++Tests_Total;
if (passed = (result == true))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.insert_element (" << index << ", 20, 2) = "
	 << result << " - " << true << endl;

++Tests_Total;
if (passed = (block.elements () == (number + 1)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.elements () = " << block.elements () << " - "
	 << (number + 1) << endl;

++Tests_Total;
if (passed = (block.size () == (index + 20)))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.size () = " << block.size () << " - " << (index + 20) << endl;

index = block.count_of (number - 1);
++Tests_Total;
if (passed = (index == 1))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (" << number << ") = " << index << " - 1" << endl;

index = block.count_of (number);
++Tests_Total;
if (passed = (index == 2))
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "block.count_of (" << number << ") = " << index << " - 2" << endl;


Done (Tests_Total, Tests_Passed);
}

catch (invalid_argument except)
	{
	cerr << "Invalid argument: " << except.what () << endl;
	exit (-1);
	}
catch (length_error except)
	{
	cerr << "Length error: " << except.what () << endl;
	exit (-1);
	}
catch (out_of_range except)
	{
	cerr << "Out-of-range: " << except.what () << endl;
	exit (-1);
	}
catch (logic_error except)
	{
	cerr << "Logic error: " << except.what () << endl;
	exit (-1);
	}
catch (exception except)
	{
	cerr << "Exception: " << except.what () << endl;
	exit (-1);
	}
catch (...)
	{
	cerr << "Unknown exception." << endl;
	exit (-1);
	}
