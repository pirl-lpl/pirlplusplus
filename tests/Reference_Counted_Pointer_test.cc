/*	Reference_Counted_Pointer_test

PIRL CVS ID: $Id: Reference_Counted_Pointer_test.cc,v 1.8 2010/05/28 01:12:25 castalia Exp $

Copyright (C) 2009  Arizona Board of Regents on behalf of the Planetary
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

#include "../Reference_Counted_Pointer.hh"
using namespace PIRL;

#include	<iostream>
#include	<iomanip>
#include	<string>
#include	<cstdlib>
using namespace std;


int
main
	(
	int		count,
	char	**arguments
	)
{
cout << "*** Reference_Counted_Pointer test" << endl;

int
	Checks = 0,
	Passed = 0;
bool
	result;

string
	*a_string = new string ("a_string");
cout << "Pointer @ " << hex << (void*)a_string << dec
		<< " to a_string: \"" << *a_string << '"' << endl;

cout << endl
	 << "Reference_Counted_Pointer<string>"
	 	" *RCP = new Reference_Counted_Pointer<string>(a_string)" << endl;

Reference_Counted_Pointer<string>
	*RCP = new Reference_Counted_Pointer<string>(a_string);
Checks++;
if (result = (**RCP == "a_string"))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": **RCP == \"a_string\"" << endl;
Checks++;
if (result = ((*RCP)->length () == 8))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": (*RCP)->length () == 8" << endl;
Checks++;
if (result = (RCP->reference_count () == 1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP->reference_count () == 1" << endl;

cout << endl
	 << "Reference_Counted_Pointer<string>"
	 	" *RCP_copy = new Reference_Counted_Pointer<string>(*RCP)" << endl;
Reference_Counted_Pointer<string>
	*RCP_copy = new Reference_Counted_Pointer<string>(*RCP);
Checks++;
if (result = (**RCP_copy == "a_string"))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": **RCP_copy == \"a_string\"" << endl;
Checks++;
if (result = (RCP->reference_count () == 2))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP->reference_count () == 2" << endl;
Checks++;
if (result = (RCP_copy->reference_count () == 2))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_copy->reference_count () == 2" << endl;
Checks++;
if (result = (*RCP == *RCP_copy))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP == RCP_copy" << endl;


delete RCP;
Checks++;
if (result = (RCP_copy->reference_count () == 1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_copy->reference_count () == 1 after delete RCP" << endl;

cout << endl
	 << "Reference_Counted_Pointer<string> rcp" << endl;
Reference_Counted_Pointer<string>
	rcp;
Checks++;
if (! (result = rcp))
	Passed++;
cout << (result ? "!!! FAIL" : "*** PASS")
		<< ": rcp is false after default constructor" << endl;
a_string = new string ("a_string");
cout << "Pointer @ " << hex << (void*)a_string << dec
		<< " to a_string: \"" << *a_string << '"' << endl;
rcp = a_string;
Checks++;
if (result = rcp)
	Passed++;
cout << (rcp ? "*** PASS" : "!!! FAIL")
		<< ": rcp is true after rcp = a_string" << endl;
Checks++;
if (result = (rcp != *RCP))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp != *RCP" << endl;
Checks++;
if (result = (rcp == a_string))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp == a_string" << endl;
Checks++;
string
	*another_string = new string ("another_string");
if (result = (rcp != another_string))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp != another_string" << endl;
Checks++;
if (result = (*rcp == "a_string"))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": *rcp == \"a_string\"" << endl;
Checks++;
if (result = (rcp->length () == 8))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp->length () == 8" << endl;
Checks++;
if (result = (rcp.reference_count () == 1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp.reference_count () == 1" << endl;
Checks++;
if (result = (rcp == a_string))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp == a_string" << endl;
string
	*pointer = rcp;
Checks++;
if (result = (pointer == a_string))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": string* pointer == a_string" << endl;


if (rcp.reference_count () != 1)
	exit (1);
cout << endl
	 << "RCP = new Reference_Counted_Pointer<string>(rcp)" << endl;
RCP = new Reference_Counted_Pointer<string> (rcp);
Checks++;
if (result = (RCP->reference_count () == 2))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP->reference_count () == 2" << endl;
rcp = *RCP;
Checks++;
if (result = (rcp.reference_count () == 3))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp.reference_count () == 3 after rcp = *RCP" << endl;
if (rcp.reference_count () != 3)
	cout << "          rcp.reference_count () = "
			<< rcp.reference_count () << endl;


cout << endl
	 << "Reference_Counted_Pointer<string>(a_string) RCP_0" << endl
	 << "Reference_Counted_Pointer<string>(a_string) RCP_1" << endl;
a_string = new string ("a_string");
Reference_Counted_Pointer<string>
	RCP_0 (a_string),
	RCP_1 (a_string);
Checks++;
if (result = (RCP_0 == RCP_1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_0 == RCP_1" << endl;
Checks++;
if (result = RCP_0.is_distinct_from (RCP_1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_0.is_distinct_from (RCP_1)" << endl;
Checks++;
rcp = RCP_0;
if (result = (rcp.reference_count () == 2))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp.reference_count () == 2 after rcp = RCP_0" << endl;
if (rcp.reference_count () != 2)
	cout << "          rcp.reference_count () = "
			<< rcp.reference_count () << endl;
Checks++;
if (result = (RCP_0.reference_count () == 2))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_0.reference_count () == 2 after rcp = RCP_0" << endl;
if (RCP_0.reference_count () != 2)
	cout << "          RCP_0.reference_count () = "
			<< RCP_0.reference_count () << endl;
Checks++;
if (result = (! rcp.is_distinct_from (RCP_0)))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": ! rcp.is_distinct_from (RCP_0)" << endl;
Checks++;
if (result = rcp.is_distinct_from (RCP_1))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": rcp.is_distinct_from (RCP_1)" << endl;
Checks++;
RCP_1 = rcp;
if (result = (! rcp.is_distinct_from (RCP_1)))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": ! rcp.is_distinct_from (RCP_1) after RCP_1 = rcp" << endl;
Checks++;
if (result = (! rcp.is_distinct_from (RCP_0)))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": ! rcp.is_distinct_from (RCP_0)" << endl;
Checks++;
if (result = (! RCP_0.is_distinct_from (RCP_1)))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": ! RCP_0.is_distinct_from (RCP_1)" << endl;
Checks++;
if (result = (RCP_0.reference_count () == 3))
	Passed++;
cout << (result ? "*** PASS" : "!!! FAIL")
		<< ": RCP_0.reference_count () == 3" << endl;
if (RCP_0.reference_count () != 3)
	cout << "          RCP_0.reference_count () = "
			<< RCP_0.reference_count () << endl;



cout << endl
	 << "Checks: " << Checks << endl
	 << "Passed: " << Passed << endl;

exit ((Checks == Passed) ? 0 : 1);
}

