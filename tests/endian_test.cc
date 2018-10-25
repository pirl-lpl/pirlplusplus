/*	endian_test

PIRL CVS ID: $Id: endian_test.cc,v 1.6 2009/09/04 05:30:59 castalia Exp $

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
#include <iomanip>
#include <bitset>
#include <cstdlib>

#include "endian.hh"

using namespace std;
using namespace PIRL;


typedef union
	{
	unsigned char	bytes[sizeof (int)];
	unsigned int	integer;
	}
	int_bytes;

int
main
	(
	int		count,
	char	**arguments
	)
{
cout << "*** endian test" << endl;

int_bytes
	test_value;

cout << "high_endian_host = " << boolalpha << high_endian_host () << endl;
cout << sizeof (int) << " byte integer." << endl;
cout << "                      ";
for (count = 0;
	 count < sizeof (int);
	 count++)
	cout << "  byte " << count << " ";
cout << " " << right << setw (8 * sizeof (int)) << "integer"
	 << left << "  decimal" << endl;
for (int index = 0;
		 index < sizeof (int);
		 index++)
	{
	test_value.integer = 0;
	test_value.bytes[index] = 1;

	cout << setw (10) << left << test_value.integer;
	cout << "host order: ";
	for (count = 0;
		 count < sizeof (int);
		 count++)
		cout << bitset<8>(test_value.bytes[count]) << " ";
	cout << " " << bitset<8 * sizeof (int)>(test_value.integer) << "  "
		 << test_value.integer << endl;

	cout << setw (10) << left << MSB_value (test_value.integer);
	cout << " MSB order: ";
	for (count = 0;
		 count < sizeof (int);
		 count++)
		cout << bitset<8>(test_value.bytes[count]) << " ";
	cout << " " << bitset<8 * sizeof (int)>(test_value.integer) << "  "
		 << test_value.integer << endl;

	test_value.integer = 0;
	test_value.bytes[index] = 1;
	cout << setw (10) << left << LSB_value (test_value.integer);
	cout << " LSB order: ";
	for (count = 0;
		 count < sizeof (int);
		 count++)
		cout << bitset<8>(test_value.bytes[count]) << " ";
	cout << " " << bitset<8 * sizeof (int)>(test_value.integer) << "  "
		 << test_value.integer << endl;

	cout << endl;
	}
exit (0);
}

