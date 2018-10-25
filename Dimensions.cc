/*	Dimensions

PIRL CVS ID: $Id: Dimensions.cc,v 1.16 2011/02/18 02:29:28 castalia Exp $

Copyright (C) 2010  Arizona Board of Regents on behalf of the
Planetary Image Research Laboratory, Lunar and Planetary Laboratory at
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

*******************************************************************************/

#include	"Dimensions.hh"


#include	<iostream>
#include	<limits>
using std::numeric_limits;
#include	<sstream>
using std::ostringstream;
#include	<iomanip>
using std::endl;
#include	<stdexcept>
using std::invalid_argument;


namespace PIRL
{
const char* const
	Point_2D::ID =
		"PIRL::Dimensions ($Revision: 1.16 $ $Date: 2011/02/18 02:29:28 $)";


//	Provides stringification of #defined names.
#define AS_STRING(name)			STRINGIFIED(name)
#define STRINGIFIED(name)		#name

/*******************************************************************************
	Point_2D
*/
Point_2D::Point_2D ()
	:	X (0),
		Y (0)
{}


Point_2D::Point_2D
	(
	const Coordinate_Type&	x,
	const Coordinate_Type&	y
	)
	:	X (x),
		Y (y)
{}


Point_2D::Point_2D
	(
	const Point_2D&	point
	)
	:	X (point.X),
		Y (point.Y)
{}


Point_2D&
Point_2D::operator/=
	(
	double	factor
	)
{
if (factor == 0)
	{
	if (X != 0)
		{
		if (numeric_limits<Coordinate_Type>::has_infinity)
			X = (X > 0) ?
				numeric_limits<Coordinate_Type>::infinity () :
			   -numeric_limits<Coordinate_Type>::infinity ();
		else
		if (numeric_limits<Coordinate_Type>::is_signed)
			X = (X > 0) ?
				numeric_limits<Coordinate_Type>::max () :
				numeric_limits<Coordinate_Type>::min ();
		else
			X = numeric_limits<Coordinate_Type>::max ();
		}
	if (Y != 0)
		{
		if (numeric_limits<Coordinate_Type>::has_infinity)
			Y = (Y > 0) ?
				numeric_limits<Coordinate_Type>::infinity () :
			   -numeric_limits<Coordinate_Type>::infinity ();
		else
		if (numeric_limits<Coordinate_Type>::is_signed)
			Y = (Y > 0) ?
				numeric_limits<Coordinate_Type>::max () :
				numeric_limits<Coordinate_Type>::min ();
		else
			Y = numeric_limits<Coordinate_Type>::max ();
		}
	}
else
	{
	X = Round (X / factor);
	Y = Round (Y / factor);
	}
return *this;
}


std::ostream&
operator<<
	(
	std::ostream&		stream,
	const Point_2D&		point
	)
{
stream << point.X << "x, " << point.Y << "y";
return stream;
}

/*******************************************************************************
	Size_2D
*/
Size_2D::Size_2D ()
	:	Width (0),
		Height (0)
{}


Size_2D::Size_2D
	(
	const Dimensions_Type&	width
	)
	:	Width (width),
		Height (width)
{}


Size_2D::Size_2D
	(
	const Dimensions_Type&	width,
	const Dimensions_Type&	height
	)
	:	Width (width),
		Height (height)
{}


Size_2D::Size_2D
	(
	const Size_2D&	size
	)
	:	Width (size.Width),
		Height (size.Height)
{}


Size_2D&
Size_2D::operator-=
	(
	const Size_2D&	size
	)
{
if (! numeric_limits<Dimensions_Type>::is_signed &&
	Width >= size.Width)
	Width -= size.Width;
else
	Width = 0;
if (! numeric_limits<Dimensions_Type>::is_signed &&
	Height >= size.Height)
	Height -= size.Height;
else
	Height = 0;
return *this;
}

Size_2D&
Size_2D::operator*=
	(
	double	factor
	)
{
if (factor < 0 &&
	! numeric_limits<Dimensions_Type>::is_signed)
	{
	ostringstream
		message;
	message
		<< Point_2D::ID << endl
		<< "Can't multiply Size_2D dimensions by a "
			<< factor << " factor because "
			<< AS_STRING (DIMENSIONS_TYPE) " can't be negative.";
	throw invalid_argument (message.str ());
	}
Width = Round (Width * factor);
Height = Round (Height * factor);
return *this;
}


Size_2D&
Size_2D::operator/=
	(
	double	factor
	)
{
if (factor < 0 &&
	! numeric_limits<Dimensions_Type>::is_signed)
	{
	ostringstream
		message;
	message
		<< Point_2D::ID << endl
		<< "Can't divide Size_2D dimensions by a "
			<< factor << " factor because "
			<< AS_STRING (DIMENSIONS_TYPE) " can't be negative.";
	throw invalid_argument (message.str ());
	}
if (factor == 0)
	{
	if (Width != 0)
		{
		if (numeric_limits<Dimensions_Type>::has_infinity)
			Width = (Width > 0) ?
				numeric_limits<Dimensions_Type>::infinity () :
			   -numeric_limits<Dimensions_Type>::infinity ();
		else
		if (numeric_limits<Dimensions_Type>::is_signed)
			Width = (Width > 0) ?
				numeric_limits<Dimensions_Type>::max () :
				numeric_limits<Dimensions_Type>::min ();
		else
			Width = numeric_limits<Dimensions_Type>::max ();
		}
	if (Height != 0)
		{
		if (numeric_limits<Dimensions_Type>::has_infinity)
			Height = (Height > 0) ?
				numeric_limits<Dimensions_Type>::infinity () :
			   -numeric_limits<Dimensions_Type>::infinity ();
		else
		if (numeric_limits<Dimensions_Type>::is_signed)
			Height = (Height > 0) ?
				numeric_limits<Dimensions_Type>::max () :
				numeric_limits<Dimensions_Type>::min ();
		else
			Height = numeric_limits<Dimensions_Type>::max ();
		}
	}
else
	{
	Width = Round (Width / factor);
	Height = Round (Height / factor);
	}
return *this;
}


std::ostream&
operator<<
	(
	std::ostream&		stream,
	const Size_2D& 		size
	)
{
stream << size.Width << "w, " << size.Height << "h";
return stream;
}

/*******************************************************************************
	Rectangle
*/
Rectangle::Rectangle ()
	:	Point_2D (),
		Size_2D ()
{}


Rectangle::Rectangle
	(
	const Size_2D&	size
	)
	:	Point_2D (),
		Size_2D (size)
{}


Rectangle::Rectangle
	(
	const Point_2D&	position,
	const Size_2D&	size
	)
	:	Point_2D (position),
		Size_2D (size)
{}


Rectangle::Rectangle
	(
	const Coordinate_Type	x,
	const Coordinate_Type	y,
	const Dimensions_Type	width,
	const Dimensions_Type	height
	)
	:	Point_2D (x, y),
		Size_2D (width, height)
{}


Rectangle::Rectangle
	(
	const Rectangle&	rectangle
	)
	:	Point_2D (rectangle.X, rectangle.Y),
		Size_2D (rectangle.Width, rectangle.Height)
{}


Rectangle&
Rectangle::operator&=
	(
	const Rectangle&	rectangle
	)
{
long long
	this_right_edge  = static_cast<long long>(X) + Width,
	this_bottom_edge = static_cast<long long>(Y) + Height,
	that_right_edge  = static_cast<long long>(rectangle.X) + rectangle.Width,
	that_bottom_edge = static_cast<long long>(rectangle.Y) + rectangle.Height;
if (this_right_edge  <= rectangle.X ||
	that_right_edge  <= X ||
	this_bottom_edge <= rectangle.Y ||
	that_bottom_edge <= Y)
	//	No intersection.
	 Width =
	Height = 0;
else
	{
	if (X < rectangle.X)
		X = rectangle.X;
	if (Y < rectangle.Y)
		Y = rectangle.Y;
	if (this_right_edge  > that_right_edge)
		this_right_edge  = that_right_edge;
	if (this_bottom_edge > that_bottom_edge)
		this_bottom_edge = that_bottom_edge;
	Width  = (Dimensions_Type)(this_right_edge  - X);
	Height = (Dimensions_Type)(this_bottom_edge - Y);
	}
return *this;
}


Rectangle&
Rectangle::operator|=
	(
	const Rectangle&	rectangle
	)
{
long long
	this_right_edge  = static_cast<long long>(X) + Width,
	this_bottom_edge = static_cast<long long>(Y) + Height,
	that_right_edge  = static_cast<long long>(rectangle.X) + rectangle.Width,
	that_bottom_edge = static_cast<long long>(rectangle.Y) + rectangle.Height;
if (X > rectangle.X)
	X = rectangle.X;
if (Y > rectangle.Y)
	Y = rectangle.Y;
if (this_right_edge  < that_right_edge)
	this_right_edge  = that_right_edge;
if (this_bottom_edge < that_bottom_edge)
	this_bottom_edge = that_bottom_edge;
Width  = (Dimensions_Type)(this_right_edge  - X);
Height = (Dimensions_Type)(this_bottom_edge - Y);
return *this;
}


//	Output operator.

std::ostream&
operator<<
	(
	std::ostream&		stream,
	const Rectangle& 	rectangle
	)
{
stream << (Point_2D)rectangle << ", " << (Size_2D)rectangle;
return stream;
}

/*******************************************************************************
	Cube
*/
Cube::Cube ()
	:	Rectangle (),
		Depth (0)
{}


Cube::Cube
	(
	const Size_2D&	size
	)
	:	Rectangle (size),
		Depth (1)
{}


Cube::Cube
	(
	const Point_2D&	position,
	const Size_2D&	size
	)
	:	Rectangle (position, size),
		Depth (1)
{}


Cube::Cube
	(
	const Rectangle&	rectangle
	)
	:	Rectangle (rectangle),
		Depth (1)
{}


Cube::Cube
	(
	const Coordinate_Type	x,
	const Coordinate_Type	y,
	const Dimensions_Type	width,
	const Dimensions_Type	height,
	const Dimensions_Type	depth
	)
	:	Rectangle (x, y, width, height),
		Depth (depth)
{}


Cube::Cube
	(
	const Cube&	cube
	)
	:	Rectangle (cube.X, cube.Y, cube.Width, cube.Height),
		Depth (cube.Depth)
{}


Cube&
Cube::operator+=
	(
	int		offset
	)
{
if (offset < 0 &&
	(Dimensions_Type)(-offset) > Depth &&
	! numeric_limits<Dimensions_Type>::is_signed)
	Depth = 0;
else
	Depth += offset;
return *this;
}


Cube&
Cube::operator/=
	(
	double	factor
	)
{
Rectangle::operator/= (factor);
if (factor == 0)
	{
	if (Depth != 0)
		{
		if (numeric_limits<Dimensions_Type>::has_infinity)
			Depth = (Depth > 0) ?
				numeric_limits<Dimensions_Type>::infinity () :
			   -numeric_limits<Dimensions_Type>::infinity ();
		else
		if (numeric_limits<Dimensions_Type>::is_signed)
			Depth = (Depth > 0) ?
				numeric_limits<Dimensions_Type>::max () :
				numeric_limits<Dimensions_Type>::min ();
		else
			Depth = numeric_limits<Dimensions_Type>::max ();
		}
	}
else
	Depth = Round (Depth / factor);
return *this;
}


Cube&
Cube::operator&=
	(
	const Cube&	cube
	)
{
Rectangle::operator&= (static_cast<Rectangle>(cube));
if (Rectangle::is_empty ())
	Depth = 0;
else
if (Depth > cube.Depth)
	Depth = cube.Depth;
return *this;
}


Cube&
Cube::operator|=
	(
	const Cube&	cube
	)
{
Rectangle::operator|= (static_cast<Rectangle>(cube));
if (Depth < cube.Depth)
	Depth = cube.Depth;
return *this;
}


std::ostream&
operator<<
	(
	std::ostream&	stream,
	const Cube& 	cube
	)
{
stream << (Rectangle)cube << ", " << cube.Depth << "d";
return stream;
}


}	//	namespace PIRL
