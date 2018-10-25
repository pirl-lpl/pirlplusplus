/*	Dimensions_test

PIRL CVS ID: $Id: Dimensions_test.cc,v 1.9 2011/02/18 01:38:08 castalia Exp $

Copyright (C) 2010  Arizona Board of Regents on behalf of the Planetary
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
#include	"Dimensions.hh"
using namespace PIRL;

#include	"Utility/Checker.hh"
using namespace idaeim;

#include	<iostream>
#include 	<iomanip>
#include	<cstdlib>
#include	<stdexcept>
using namespace std;


int
main
	(
	int		count,
	char**	arguments
	)
{
cout << "*** Dimensions test" << endl;

Checker
	checker;
ostringstream
	listing,
	expected_listing;

if (count > 1 &&
	*arguments[1] == '-' &&
	*(arguments[1] + 1) == 'v')
	checker.Verbose = true;

if (checker.Verbose)
	cout << "----- Constructors" << endl;

Point_2D
	point (1, 2);
checker.check ("point.x ()",
	1, point.x ());
checker.check ("point.y ()",
	2, point.y ());

Size_2D
	size (3, 4);
checker.check ("size.width ()",
	(Dimensions_Type)3, size.width ());
checker.check ("size.height ()",
	(Dimensions_Type)4, size.height ());

Rectangle
	rectangle (5, 6, 7, 8);
checker.check ("rectangle.x ()",
	5, rectangle.x ());
checker.check ("rectangle.y ()",
	6, rectangle.y ());
checker.check ("rectangle.width ()",
	(Dimensions_Type)7, rectangle.width ());
checker.check ("rectangle.height ()",
	(Dimensions_Type)8, rectangle.height ());

Cube
	cube (9, 10, 11, 12, 13);
checker.check ("cube.x ()",
	9, cube.x ());
checker.check ("cube.y ()",
	10, cube.y ());
checker.check ("cube.width ()",
	(Dimensions_Type)11, cube.width ());
checker.check ("cube.height ()",
	(Dimensions_Type)12, cube.height ());
checker.check ("cube.depth ()",
	(Dimensions_Type)13, cube.depth ());

cout << endl;
if (checker.Verbose)
	cout << "----- operator<<" << endl;
listing << point;
expected_listing << "1x, 2y";
checker.check ("listing << point",
	expected_listing, listing);

listing << size;
expected_listing << "3w, 4h";
checker.check ("listing << size",
	expected_listing, listing);

listing << rectangle;
expected_listing << "5x, 6y, 7w, 8h";
checker.check ("listing << rectangle",
	expected_listing, listing);

listing << cube;
expected_listing << "9x, 10y, 11w, 12h, 13d";
checker.check ("listing << cube",
	expected_listing, listing);


cout << endl;
if (checker.Verbose)
	cout << "----- operator=" << endl;

point = cube;
Point_2D
	expected_point (9, 10);
checker.check ("point = cube", expected_point, point);

size = cube;
Size_2D
	expected_size (11, 12);
checker.check ("size = cube", expected_size, size);

rectangle = cube;
Rectangle
	expected_rectangle (9, 10, 11, 12);
checker.check ("rectangle = cube", expected_rectangle, rectangle);

point = Point_2D (1, 2);
rectangle = point;
expected_rectangle = Rectangle (1, 2, 11, 12);
checker.check ("rectangle = point", expected_rectangle, rectangle);

cube = point;
Cube
	expected_cube (1, 2, 11, 12, 13);
checker.check ("cube = point", expected_cube, cube);

size = Size_2D (3, 4);
expected_rectangle = Rectangle (1, 2, 3, 4);
rectangle = size;
checker.check ("rectangle = size", expected_rectangle, rectangle);

cube = size;
expected_cube = Cube (1, 2, 3, 4, 13);
checker.check ("cube = size", expected_cube, cube);


cout << endl;
if (checker.Verbose)
	cout << "----- operator bool and null/is_empty" << endl;

bool
	test = point;
listing.str ("");
listing << "point " << point << " is true?";
checker.check (listing.str (),
	true, test);
test = point.is_null ();
listing.str ("");
listing << "point " << point << " ! is_null?";
checker.check (listing.str (),
	false, test);

point = Point_2D (0, 1);
test = point;
listing.str ("");
listing << "point " << point << " is true?";
checker.check (listing.str (),
	true, test);
test = point.is_null ();
listing.str ("");
listing << "point " << point << " ! is_null?";
checker.check (listing.str (),
	false, test);

point = Point_2D (0, 0);
test = point;
listing.str ("");
listing << "point " << point << " is false?";
checker.check (listing.str (),
	false, test);
test = point.is_null ();
listing.str ("");
listing << "point " << point << " is_null?";
checker.check (listing.str (),
	true, test);

test = size;
listing.str ("");
listing << "size " << size << " is true?";
checker.check (listing.str (),
	true, test);
test = size.is_empty ();
listing.str ("");
listing << "size " << size << " ! is_empty?";
checker.check (listing.str (),
	false, test);

size = Size_2D (0, 1);
test = size;
listing.str ("");
listing << "size " << size << " is true?";
checker.check (listing.str (),
	true, test);
test = size.is_empty ();
listing.str ("");
listing << "size " << size << " is_empty?";
checker.check (listing.str (),
	true, test);

size = Size_2D (0, 0);
test = size;
listing.str ("");
listing << "size " << size << " is false?";
checker.check (listing.str (),
	false, test);
test = size.is_empty ();
listing.str ("");
listing << "size " << size << " is_empty?";
checker.check (listing.str (),
	true, test);

test = rectangle;
listing.str ("");
listing << "rectangle " << rectangle << " is true?";
checker.check (listing.str (),
	true, test);
test = rectangle.is_empty ();
listing.str ("");
listing << "rectangle " << rectangle << " ! is_empty?";
checker.check (listing.str (),
	false, test);

rectangle = Rectangle (0, 0, 0, 1);
test = rectangle;
listing.str ("");
listing << "rectangle " << rectangle << " is true?";
checker.check (listing.str (),
	true, test);
test = rectangle.is_empty ();
listing.str ("");
listing << "rectangle " << rectangle << " is_empty?";
checker.check (listing.str (),
	true, test);

rectangle = Rectangle (1, 0, 0, 0);
test = rectangle;
listing.str ("");
listing << "rectangle " << rectangle << " is true?";
checker.check (listing.str (),
	true, test);
test = rectangle.is_empty ();
listing.str ("");
listing << "rectangle " << rectangle << " is_empty?";
checker.check (listing.str (),
	true, test);

rectangle = Rectangle (0, 0, 0, 0);
test = rectangle;
listing.str ("");
listing << "rectangle " << rectangle << " is false?";
checker.check (listing.str (),
	false, test);
test = rectangle.is_empty ();
listing.str ("");
listing << "rectangle " << rectangle << " is_empty?";
checker.check (listing.str (),
	true, test);

test = cube;
listing.str ("");
listing << "cube " << cube << " is true?";
checker.check (listing.str (),
	true, test);
test = cube.is_empty ();
listing.str ("");
listing << "cube " << cube << " ! is_empty?";
checker.check (listing.str (),
	false, test);

cube = Cube ();
test = cube;
listing.str ("");
listing << "cube " << cube << " is false?";
checker.check (listing.str (),
	false, test);
test = cube.is_empty ();
listing.str ("");
listing << "cube " << cube << " is_empty?";
checker.check (listing.str (),
	true, test);

cube.depth (1);
test = cube;
listing.str ("");
listing << "cube " << cube << " is true?";
checker.check (listing.str (),
	true, test);
test = cube.is_empty ();
listing.str ("");
listing << "cube " << cube << " is_empty?";
checker.check (listing.str (),
	true, test);


cout << endl;
if (checker.Verbose)
	cout << "----- Arithmetic operators" << endl;

Point_2D
	point_1 (1, 2),
	point_2 (3, 4);
point = point_1 + point_2;
expected_point = Point_2D (4, 6);
listing.str ("");
listing << "point " << point_1 << " + point " << point_2;
checker.check (listing.str (),
	expected_point, point);
	
point = point_1 - point_2;
expected_point = Point_2D (-2, -2);
listing.str ("");
listing << "point " << point_1 << " - point " << point_2;
checker.check (listing.str (),
	expected_point, point);
	
point = -point_1;
expected_point = Point_2D (-1, -2);
listing.str ("");
listing << "-point " << point_1;
checker.check (listing.str (),
	expected_point, point);

point = point_1 * 3.4;
expected_point = Point_2D (3, 7);
listing.str ("");
listing << "point " << point_1 << " 3.4";
checker.check (listing.str (),
	expected_point, point);

Size_2D
	size_1 (1, 2),
	size_2 (3, 4);
size = size_2 - size_1;
expected_size = Size_2D (2, 2);
listing.str ("");
listing << "size " << size_2 << " - size " << size_1;
checker.check (listing.str (),
	expected_size, size);

size = size_1 - size_2;
expected_size = Size_2D (0, 0);
listing.str ("");
listing << "size " << size_1 << " - size " << size_2;
checker.check (listing.str (),
	expected_size, size);

size = size_1 * 3.4;
expected_size = Size_2D (3, 7);
listing.str ("");
listing << "size " << size_1 << " * 3.4";
checker.check (listing.str (),
	expected_size, size);

test = false;
try {size = size_1 * -1.0;}
catch (invalid_argument except)
	{
	test = true;
	if (checker.Verbose)
		cout << "----- Negative factor exception" << endl
			 << except.what () << endl;
	}
listing.str ("");
listing << "size " << size_1 << " * -1.0";
checker.check (listing.str (),
	true, test);

Rectangle
	rectangle_1 (5, 6, 7, 8);
listing.str ("");
listing << "rectangle " << rectangle_1 << " += point " << point_1;
rectangle_1 += point_1;
expected_rectangle = Rectangle (6, 8, 7, 8);
checker.check (listing.str (),
	expected_rectangle, rectangle_1);

rectangle   = Rectangle (0, 5, 10, 11);
rectangle_1 = Rectangle (5, 0, 11, 10);
listing.str ("");
listing << "rectangle " << rectangle << " &= rectangle " << rectangle_1;
rectangle &= rectangle_1;
expected_rectangle = Rectangle (5, 5, 5, 5);
checker.check (listing.str (),
	expected_rectangle, rectangle);

rectangle   = Rectangle (5, 5, 5, 5);
rectangle_1 = Rectangle (10, 10, 10, 10);
listing.str ("");
listing << "rectangle " << rectangle << " &= rectangle " << rectangle_1;
rectangle &= rectangle_1;
expected_rectangle = Rectangle (5, 5, 0, 0);
checker.check (listing.str (),
	expected_rectangle, rectangle);

rectangle   = Rectangle (0, 5, 15, 25);
rectangle_1 = Rectangle (1, 2, 20, 30);
listing.str ("");
listing << "rectangle " << rectangle << " |= rectangle " << rectangle_1;
rectangle |= rectangle_1;
expected_rectangle = Rectangle (0, 2, 21, 30);
checker.check (listing.str (),
	expected_rectangle, rectangle);

cube = Cube (1, 2, 3, 4, 5);
listing.str ("");
listing << "cube " << cube << " *= 3.1";
cube *= 3.1;
expected_cube = Cube (1, 2, 9, 12, 16);
checker.check (listing.str (),
	expected_cube, cube);

cube   = Cube (0, 5, 15, 25, 35);
Cube
	cube_1 (1, 2, 20, 30, 40);
listing.str ("");
listing << "cube " << cube << " |= " << cube_1;
cube |= cube_1;
expected_cube = Cube (0, 2, 21, 30, 40);
checker.check (listing.str (),
	expected_cube, cube);


cout << endl
	 << "Checks: " << checker.Checks_Total << endl
	 << "Passed: " << checker.Checks_Passed << endl;
exit (0);
}
