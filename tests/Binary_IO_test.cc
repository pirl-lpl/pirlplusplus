/*	Binary_IO_test

PIRL CVS ID: $Id: Binary_IO_test.cc,v 1.9 2010/11/16 02:09:57 castalia Exp $

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
#include <cstdlib>
using namespace std;

#include "Binary_IO.hh"
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

Binary_IO
	binary;


void
write_test
	(
	const char	*filename,
	bool		verbose = false
	)
{
ofstream
	output (filename, ios::out | ios::binary);
if (! output)
	{
	cout << "Unable to open output file: \"" << filename << "\"" << endl;
	exit (1);
	}

cout << ">>> Writing file \"" << filename << "\"" << endl;

if (verbose)
cout << sizeof (char)
	 << " byte char value =  "
	 << static_cast<unsigned int>(test_char) << endl;
output << binary (test_char);

if (verbose)
cout << sizeof (short int)
	 << " byte short int value =  "
	 << test_short_int << endl;
output << binary (test_short_int);

if (verbose)
cout << sizeof (int)
	 << " byte int value =  "
	 << test_int << endl;
output << binary (test_int);

if (verbose)
cout << sizeof (unsigned int)
	 << " byte unsigned int value = "
	 << test_unsigned_int << endl;
output << binary (test_unsigned_int);

if (verbose)
cout << sizeof (long int)
	 << " byte long int value =  "
	 << test_long_int << endl;
output << binary (test_long_int);

if (verbose)
cout << sizeof (float)
	 << " byte float value =  "
	 << test_float << endl;
output << binary (test_float);

if (verbose)
cout << sizeof (double)
	 << " byte double value = "
	 << test_double << endl;
output << binary (test_double);

output.close ();
cout << "<<< Wrote file \"" << filename << "\"" << endl;
}


int
read_result
	(
	const char	*filename,
	bool		verbose = true
	)
{
bool
	passed;
int
	result = 0;
ifstream
	input (filename, ios::in | ios::binary);
if (! input)
	{
	cout << "Unable to open input file: \"" << filename << "\"" << endl;
	exit (1);
	}

cout << ">>> Reading file \"" << filename << "\"" << endl;

result_char = 0;
input >> binary (result_char);
if (passed = (result_char == test_char))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (char) << " byte char value =  "
	 << static_cast<unsigned int>(result_char) << " - "
	 << static_cast<unsigned int>(test_char) << endl;

result_short_int = 0;
input >> binary (result_short_int);
if (passed = (result_short_int == test_short_int))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (short int) << " byte short int value =  "
	 << result_short_int << " - " << test_short_int << endl;

result_int = 0;
input >> binary (result_int);
if (passed = (result_int == test_int))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (int) << " byte int value =  "
	 << result_int << " - " << test_int << endl;

result_unsigned_int = 0;
input >> binary (result_unsigned_int);
if (passed = (result_unsigned_int == test_unsigned_int))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (unsigned int) << " byte unsigned int value =  "
	 << result_unsigned_int << " - " << test_unsigned_int << endl;

result_long_int = 0;
input >> binary (result_long_int);
if (passed = (result_long_int == test_long_int))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (long int) << " byte long int value =  "
	 << result_long_int << " - " << test_long_int << endl;

result_float = 0.0;
input >> binary (result_float);
if (passed = (result_float == test_float))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (float) << " byte float value =  "
	 << result_float << " - " << test_float << endl;

result_double = 0.0;
input >> binary (result_double);
if (passed = (result_double == test_double))
	++result;
if (verbose)
cout << (passed ? "PASS: " : "FAIL: ")
	 << sizeof (double) << " byte double value =  "
	 << result_double << " - " << test_double << endl;

input.close ();

#define	TOTAL_READ_TESTS	7
cout << "<<< Read file \"" << filename << "\": Passed "
	 << result << " of " << TOTAL_READ_TESTS << " cases" << endl;
return result;
}


int
main
	(
	int		count,
	char	**argument
	)
{
cout << "*** Binary_IO test" << endl;
bool
	MSB_host = high_endian_host ();
cout << "=== Host is " << (MSB_host ? "high" : "low") << " endian." << endl;
enum MODES
	{
	AUTO				= 0,
	INPUT				= (1 << 0),
	OUTPUT				= (1 << 1),
	INPUT_REORDERED		= (1 << 2),
	OUTPUT_REORDERED	= (1 << 3)
	};
int
	mode	     = AUTO,
	Tests_Total  = 0,
	Tests_Passed = 0;
char
	*default_filename = const_cast<char*>("Binary_IO.test"),
	*input_filename  = default_filename,
	*output_filename = input_filename;

for (int arg = 1;
		 arg < count;
		 )
	{
	if (*argument[arg] == '-')
		{
		switch (*(argument[arg] + 1))
			{
			case 'I':
			case 'i':
				mode |= INPUT;
				if (strchr (argument[arg], 'R') ||
					strchr (argument[arg], 'r'))
					mode |= INPUT_REORDERED;
				if (++arg < count &&
					*argument[arg] != '-')
					input_filename = argument[arg++];
				break;
			case 'O':
			case 'o':
				mode |= OUTPUT;
				if (strchr (argument[arg], 'R') ||
					strchr (argument[arg], 'r'))
					mode |= OUTPUT_REORDERED;
				if (++arg < count &&
					*argument[arg] != '-')
					output_filename = argument[arg++];
				break;
			default:
				cout << "Unknown option: " << argument[arg] << endl;
				exit (1);
			}
		}
	else
		{
		if (! (mode & INPUT))
			{
			mode |= INPUT;
			input_filename = argument[arg++];
			}
		else if (! (mode * OUTPUT))
			{
			mode |= OUTPUT;
			output_filename = argument[arg++];
			}
		else
			{
			cout << "Both input - \"" << input_filename << "\"" << endl
				 << "and output - \"" << output_filename << "\"" << endl
				 << "filenames have been specified!?" << endl;
			exit (1);
			}
		}
	}

if (mode == AUTO)
	{
	bool
		passed;
	cout << "--- Interface test" << endl;
//	binary.reversed (true);

	ofstream
		output (default_filename, ios::out | ios::binary);
	if (! output)
		{
		cout << "Unable to open output file: \"" << default_filename << "\"" << endl;
		exit (1);
		}
	cout << ">>> Writing file \"" << default_filename << "\" reversed" << endl;
	Binary_Output
		b_out (output, true);

	cout << "--- output << b_out (test_int): "
		 << sizeof (int) << " byte int value =  " << test_int << endl;
	output << b_out (test_int);

	cout << "--- b_out.put (output, (const char*)(&test_int), sizeof (test_int))" << endl;
	b_out.write (output, (const char*)(&test_int), sizeof (test_int));

	cout << "--- b_out.put (test_int)" << endl;
	b_out.put (test_int);

	cout << "--- b_out.put_3 (test_int)" << endl;
	b_out.put_3 (test_int);

	cout << "--- b_out.put (array[of test_int], 3)" << endl;
	int
		array[3] = {test_int, test_int, test_int};
	b_out.put (array, 3);

	output.close ();
	cout << "<<< Wrote file \"" << default_filename << "\"" << endl;

	ifstream
		input (default_filename, ios::in | ios::binary);
	if (! input)
		{
		cout << "Unable to open input file: \"" << default_filename << "\"" << endl;
		exit (1);
		}
	cout << ">>> Reading file \"" << default_filename << "\" reversed" << endl;
	Binary_Input
		b_in (input, true);

	cout << "--- input >> b_in (result_int)" << endl;
	result_int = 0;
	input >> b_in (result_int);
	passed = (result_int == test_int);
	cout << (passed ? "PASS: " : "FAIL: ")
		 << sizeof (int) << " byte int value =  "
		 << result_int << " - " << test_int << endl;
	++Tests_Total;
	if (passed)
		++Tests_Passed;

	cout << "--- b_in.get (input, (char*)(&result_int), sizeof (result_int))" << endl;
	result_int = 0;
	b_in.read (input, (char*)(&result_int),sizeof (test_int) );
	passed = (result_int == test_int);
	cout << (passed ? "PASS: " : "FAIL: ")
		 << sizeof (int) << " byte int value =  "
		 << result_int << " - " << test_int << endl;

	cout << "--- b_in.get (result_int)" << endl;
	b_in.get (result_int);
	passed = (result_int == test_int);
	cout << (passed ? "PASS: " : "FAIL: ")
		 << sizeof (int) << " byte int value =  "
		 << result_int << " - " << test_int << endl;
	++Tests_Total;
	if (passed)
		++Tests_Passed;

	result_int = 0;
	cout << "--- b_in.get_3 (result_int)" << endl;
	b_in.get_3 (result_int);
	passed = (result_int == test_int);
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "3 byte int value =  "
		 << result_int << " - " << test_int << endl;
	++Tests_Total;
	if (passed)
		++Tests_Passed;

	array[0] = array[1] = array[0] = 0;
	cout << "--- b_in.get (array, 3)" << endl;
	b_in.get (array, 3);
	for (int index = 0; index < 3; index++)
		{
		passed = (array[index] == test_int);
		cout << (passed ? "PASS: " : "FAIL: ")
			 << sizeof (int) << " byte int value =  "
			 << array[index] << " - " << test_int << endl;
		++Tests_Total;
		if (passed)
			++Tests_Passed;
		}

	input.close ();
	cout << "<<< Read file \"" << default_filename << "\"\n";
	}

//	Native IO
binary.reversed (false);
//cout << "--- binary (42): " << binary (42) << endl;
if (mode == AUTO ||
	(mode & OUTPUT  &&
	! (mode & OUTPUT_REORDERED)))
	{
	cout << endl << "--- Using native binary IO" << endl;
	write_test (output_filename, true);
	}
if (mode == AUTO ||
	(mode & INPUT &&
	! (mode & INPUT_REORDERED)))
	{
	if (mode != AUTO)
		cout << endl << "--- Using native binary IO" << endl;
	Tests_Passed += read_result (input_filename);
	Tests_Total += TOTAL_READ_TESTS;
	}

//	Reveresed IO
binary.reversed (true);
if (mode == AUTO ||
	mode & OUTPUT_REORDERED)
	{
	cout << endl << "--- Using reversed binary IO" << endl;
	write_test (output_filename, mode != AUTO);
	}
if (mode == AUTO ||
	mode & INPUT_REORDERED)
	{
	if (mode != AUTO)
		cout << endl << "--- Using reversed binary IO" << endl;
	Tests_Passed += read_result (input_filename);
	Tests_Total += TOTAL_READ_TESTS;
	}

if (mode == AUTO)
	{
	cout << endl << "--- Using reversed output" << endl;
	write_test (output_filename);
	cout << "    with native input >>> Should FAIL <<<" << endl;
	binary.reversed (false);
	Tests_Passed += TOTAL_READ_TESTS - (read_result (input_filename) - 1);
	Tests_Total += TOTAL_READ_TESTS;
	}

cout << endl
	 << "Checks: " << Tests_Total << endl
	 << "Passed: " << Tests_Passed << endl;
exit (Tests_Total - Tests_Passed);
}
