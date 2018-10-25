/*	Dimensions

PIRL CVS ID: $Id: Dimensions.hh,v 1.27 2011/02/18 02:29:28 castalia Exp $

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
*/

#ifndef _Dimensions_
#define _Dimensions_

#include	<iosfwd>


namespace PIRL
{
/*==============================================================================
	Types
*/
#ifndef COORDINATE_TYPE
#define COORDINATE_TYPE int
#endif
#ifndef	DIMENSIONS_TYPE
#define	DIMENSIONS_TYPE unsigned COORDINATE_TYPE
#endif
//!	The integer data type of a coordinate value.
typedef COORDINATE_TYPE	Coordinate_Type;
//!	The integer data type of a dimension value.
typedef DIMENSIONS_TYPE	Dimensions_Type;


/**	Rounds a floating point number to the nearest integer value.

	The nearest integer value is found by adding 0.5 to the floating
	point number and truncating the result to the Coordinate_Type integer
	value. For negative numbers 0.5 is subtracted. Thus a floating point
	number exactly halfway beteen two integer values is rounded to the
	absolute larger integer with the sign preserved.

	@param	number	The floating point (double) number to be rounded.
	@return	The integer value of the rounded number.
*/
inline Coordinate_Type Round (double number)
	{return (Coordinate_Type)(number > 0 ? (number + 0.5) : (number - 0.5));}

//******************************************************************************
/**	A <i>Point_2D</i> holds 2-dimensional position information.

	@author		Bradford Castalia, UA/PIRL
	@version	$Revision: 1.27 $
*/
struct Point_2D
{
//!	Class identification name with source code version and date.
static const char* const
	ID;


//!	The horizontal (x-axis) position of the Point_2D.
Coordinate_Type
	X;
//!	The vertical (y-axis) position of the Point_2D.
Coordinate_Type
	Y;

/*==============================================================================
	Constructors:
*/
//!	Constructs a Point_2D at position 0,0.
Point_2D ();

/**	Constructs a Point_2D at position x,y.

	@param	x	The horizontal (x-axis) position of the Point_2D.
	@param	y	The vertical (y-axis) position of the Point_2D.
*/
Point_2D (const Coordinate_Type& x,const Coordinate_Type& y);

/**	Constructs a Point_2D from another Point_2D.

	@param	point	A Point_2D to be copied.
*/
Point_2D (const Point_2D& point);

/*==============================================================================
	Accessors:
*/
/**	Set the position of this Point_2D.

	@param	x	The horizontal (x-axis) position of the Point_2D.
	@param	y	The vertical (y-axis) position of the Point_2D.
	@return	This Point_2D.
*/
inline Point_2D& position (const Coordinate_Type& x,const Coordinate_Type& y)
	{
	X = x;
	Y = y;
	return *this;
	}

/**	Set the position of this Point_2D.

	@param	point	A Point_2D whose coordinates are to be assigned to
		this Point_2D.
	@return	This Point_2D.
*/
inline Point_2D& position (const Point_2D& point)
	{
	X = point.X;
	Y = point.Y;
	return *this;
	}

/**	Assign the position of another Point_2D to this Point_2D.

	@param	point	A Point_2D whose coordinates are to be assigned to
		this Point_2D.
*/
inline Point_2D& operator= (const Point_2D& point)
	{
	if (this != &point)
		{
		X = point.X;
		Y = point.Y;
		}
	return *this;
	}

/**	Set the horizontal (x-axis) position.

	@param	x_position	The horizontal (x-axis) position of the Point_2D.
	@return	This Point_2D.
*/
inline Point_2D& x (const Coordinate_Type& x_position)
	{X = x_position; return *this;}

/**	Get the horizontal (x-axis) position.

	@return	The horizontal (x-axis) position of the Point_2D.
*/
inline Coordinate_Type x () const
	{return X;}

/**	Set the vertical (y-axis) position.

	@param	y_position	The vertical (y-axis) position of the Point_2D.
	@return	This Point_2D.
*/
inline Point_2D& y (const Coordinate_Type& y_position)
	{Y = y_position; return *this;}

/**	Get the vertical (y-axis) position.

	@return	The vertical (y-axis) position of the Point_2D.
*/
inline Coordinate_Type y () const
	{return Y;}

/**	Test if this Point_2D is equal to another Point_2D.

	The two Point_2Ds are equal if both their X and Y coordinates are equal.

	@param	point	The Point_2D to which this Point_2D is to be compared.
	@return	true if the two Point_2Ds are equal; false otherwise.
*/
inline bool operator== (const Point_2D& point) const
	{return X == point.X && Y == point.Y;}

/**	Test if this Point_2D is not equal to another Point_2D.

	The two Point_2Ds are not equal if either their X or Y coordinates are
	not equal.

	@param	point	The Point_2D to which this Point_2D is to be compared.
	@return	true if the two Point_2Ds are not equal; false otherwise.
*/
inline bool operator!= (const Point_2D& point) const
	{return X != point.X || Y != point.Y;}

/**	Test for all zero coordinate values.

	@return	true if any coordinate value is non-zero; false otherwise.
	@see	is_null()
*/
inline operator bool ()
	{return X != 0 || Y != 0;}

/**	Test for all zero coordinate values.

	@return true if both coordinates are zero; false otherwise.
	@see	operator bool()
*/
inline bool is_null ()
	{return X == 0 && Y == 0;}

/*==============================================================================
	Manipulators:
*/
/**	Add an offset.

	@param	offset	A Point_2D that provides the offset values.
	@return	This Point_2D with its values offset.
*/
inline Point_2D& operator+= (const Point_2D& offset)
	{X += offset.X; Y += offset.Y; return *this;}

/**	Subtract an offset.

	@param	offset	A Point_2D that provides the offset values.
	@return	This Point_2D with its values offset.
*/
inline Point_2D& operator-= (const Point_2D& offset)
	{X -= offset.X; Y -= offset.Y; return *this;}

/**	Multiply by a factor.

	The new coordinate values will be rounded to the nearest
	Coordinate_Type values.

	@param	factor	A factor by which to multiply the Point_2D coordinates.
	@return	This Point_2D with its values changed.
*/
inline Point_2D& operator*= (double factor)
	{X = Round (X * factor); Y = Round (Y * factor); return *this;}

/**	Divide by a factor.

	The new coordinate values will be rounded to the nearest
	Coordinate_Type values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Coordinate_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Coordinate_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	factor	A factor by which to divide the Point_2D dimensions.
	@return	This Point_2D with its values changed.
*/
Point_2D& operator/= (double factor);

};	//	End of Point_2D class.

/**	Add two points.

	@param	point_1 A Point_2D.
	@param	point_2	A Point_2D.
	@return	A Point_2D in which the dimensions are the sum of the
		dimensions of the specified points.
*/
inline Point_2D operator+ (const Point_2D& point_1, const Point_2D& point_2)
	{return Point_2D (point_1) += point_2;}

/**	Subtract one point from another.

	@param	point_1	A Point_2D.
	@param	point_2	A Point_2D.
	@return	A Point_2D in which the dimensions are the difference of the
		dimensions of the specified points.
*/
inline Point_2D operator- (const Point_2D& point_1, const Point_2D& point_2)
	{return Point_2D (point_1) -= point_2;}

/**	Get the negation of a point.

	@param	point A Point_2D.
	@return	A Point_2D in which the dimensions are the negation of the
		dimensions of the specified points.
*/
inline Point_2D operator- (const Point_2D& point)
	{return Point_2D (-point.X, -point.Y);}

/**	Multiply a point by a factor.

	@param	point	A Point_2D.
	@param	factor	A floating point (double) number.
	@return	A Point_2D in which the dimensions are the dimensions of the
		specified point multiplied by the factor and {@link Round(double)
		rounded}.
*/
inline Point_2D operator* (const Point_2D& point, double factor)
	{return Point_2D (point) *= factor;}

/**	Multiply a point by a factor.

	@param	factor	A floating point (double) number.
	@param	point	A Point_2D.
	@return	A Point_2D in which the dimensions are the dimensions of the
		specified point multiplied by the factor and {@link Round(double)
		rounded}.
*/
inline Point_2D operator* (double factor, const Point_2D& point)
	{return Point_2D (point) *= factor;}

/**	Divide a point by a factor.

	@param	point	A Point_2D.
	@param	factor	A floating point (double) number.
	@return	A Point_2D in which the dimensions are the dimensions of the
		specified point divided by the factor and {@link Round(double)
		rounded}.
*/
inline Point_2D operator/ (const Point_2D& point, double factor)
	{return Point_2D (point) /= factor;}

/**	Print a Point_2D description to an output stream.

	@param	stream	The ostream where the Point_2D will be printed.
	@param	point	The Print_2D to be printed.
	@return	The stream that was written.
*/
std::ostream& operator<< (std::ostream& stream, const Point_2D& point);

//******************************************************************************
/**	A <i>Size_2D</i> holds 2-dimensional size information.

	@author		Bradford Castalia, UA/PIRL
	@version	$Revision: 1.27 $
*/
struct Size_2D
{
//!	The Width of the Size_2D.
Dimensions_Type
	Width;
//!	The Height of the Size_2D.
Dimensions_Type
	Height;

/*==============================================================================
	Constructors:
*/
/**	Constructs an empty Size_2D.

	Both Width and Height are zero.
*/
Size_2D ();

/**	Constructs a Size_2D with width,height size.

	@param	width	The Width of the Size_2D.
	@param	height	The Height of the Size_2D.
*/
Size_2D (const Dimensions_Type& width, const Dimensions_Type& height);

/**	Constructs a Size_2D of equal Width and Height.

	@param	side	The length of each side. Both Width and Height will
		be set to this value.
*/
Size_2D (const Dimensions_Type& side);

/**	Constructs a Size_2D from another Size_2D.

	@param	size	A Size_2D to be copied.
*/
Size_2D (const Size_2D& size);

/*==============================================================================
	Accessors:
*/
/**	Set the size of this Size_2D.

	@param	width	The Width of the Size_2D.
	@param	height	The Height of the Size_2D.
	@return	This Size_2D.
*/
inline Size_2D& size
	(const Dimensions_Type& width, const Dimensions_Type& height)
	{
	Width  = width;
	Height = height;
	return *this;
	}

/**	Set the size of this Size_2D.

	@param	size	A Size_2D to have its dimensions assigned to this
		Size_2D.
	@return	This Size_2D.
*/
inline Size_2D& size (const Size_2D& size)
	{
	Width  = size.Width;
	Height = size.Height;
	return *this;
	}

/**	Assign the dimensions of another Size_2D to this Size_2D.

	@param	size	A Size_2D to have its dimensions assigned to this
		Size_2D.
*/
inline Size_2D& operator= (const Size_2D& size)
	{
	if (this != &size)
		{
		Width  = size.Width;
		Height = size.Height;
		}
	return *this;
	}

/**	Set the Width of the Size_2D.

	@param	width	The Width of the Size_2D.
	@return	This Size_2D.
*/
inline Size_2D& width (const Dimensions_Type& width)
	{Width = width; return *this;}

/**	Get the Width of the Size_2D.

	@return	The Width of the Size_2D.
*/
inline Dimensions_Type width () const
	{return Width;}

/**	Set the Height of the Size_2D.

	@param	height	The Height of the Size_2D.
	@return	This Size_2D.
*/
inline Size_2D& height (const Dimensions_Type& height)
	{Height = height; return *this;}

/**	Get the Height of the Size_2D.

	@return	The Height of the Size_2D.
*/
inline Dimensions_Type height () const
	{return Height;}

/**	Get the area of this Size_2D.

	@return	The area (Width * Height) of the Size_2D.
*/
inline unsigned long long area () const
	{return (long long)Width * Height;}

/**	Test if this Size_2D is equal to another Size_2D.

	The two Size_2Ds are equal if both their Width and Height dimensions
	are equal.

	@param	size	The Size_2D to which this Size_2D is to be compared.
	@return	true if the two Size_2Ds are equal; false otherwise.
*/
inline bool operator== (const Size_2D& size) const
	{return Width == size.Width && Height == size.Height;}

/**	Test if this Size_2D is not equal to another Size_2D.

	The two Size_2Ds are not equal if either their Width or Height
	dimensions are not equal.

	@param	size	The Size_2D to which this Size_2D is to be compared.
	@return	true if the two Size_2Ds are not equal; false otherwise.
*/
inline bool operator!= (const Size_2D& size) const
	{return Width != size.Width || Height != size.Height;}

/**	Test for all zero dimension values.

	@return	true if any dimension value is non-zero; false otherwise.
*/
inline operator bool ()
	{return Width != 0 || Height != 0;}

/**	Test for any zero dimension values.

	@return true if any dimension is zero; false otherwise.
*/
inline bool is_empty ()
	{return Width == 0 || Height == 0;}

/*==============================================================================
	Manipulators:
*/
/**	Add a size amount.

	@param	size	A Size_2D that provides the amount values.
	@return	This Size_2D with the amount added to its values.
*/
inline Size_2D& operator+= (const Size_2D& size)
	{Width += size.Width; Height += size.Height; return *this;}

/**	Subtract a size amount.

	<b>N.B.</b>: If the size Dimensions_Type values are an unsigned type
	(as they are by default) and a size amount to be subtracted is greater
	than the corresponding dimension value, the result will be zero.

	@param	size	A Size_2D that provides the amount values.
	@return	This Size_2D with the amount subtracted to its values.
*/
Size_2D& operator-= (const Size_2D& size);

/**	Multiply by a factor.

	The new size values will be rounded to the nearest Dimensions_Types.

	@param	factor	A factor by which to multiply the Size_2D dimensions.
	@return	This Size_2D with its values changed.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
Size_2D& operator*= (double factor);

/**	Divide by a factor.

	The new dimension values will be rounded to the nearest Dimensions_Type
	values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Dimensions_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Dimensions_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	factor	A factor by which to divide the Size_2D dimensions.
	@return	This Size_2D with its values changed.
	@throws invalid_argument	If the factor is negative and the size
		values Dimensions_Type are an unsigned type (as they are by
		default).
*/
Size_2D& operator/= (double factor);

};	//	End of Size_2D class.

/**	Add two sizes.

	@param	size_1 A Size_2D.
	@param	size_2	A Size_2D.
	@return	A Size_2D in which the dimensions are the sum of the
		dimensions of the specified sizes.
*/
inline Size_2D operator+ (const Size_2D& size_1, const Size_2D& size_2)
	{return Size_2D (size_1) += size_2;}

/**	Subtract one size from another.

	<b>N.B.</b>: If the size Dimensions_Type values are an unsigned type
	(as they are by default) and a size amount to be subtracted is greater
	than the corresponding dimension value, the result will be zero.

	@param	size_1	A Size_2D.
	@param	size_2	A Size_2D.
	@return	A Size_2D in which the dimensions are the difference of the
		dimensions of the specified sizes.
*/
inline Size_2D operator- (const Size_2D& size_1, const Size_2D& size_2)
	{return Size_2D (size_1) -= size_2;}

/**	Multiply a size by a factor.

	@param	size	A Size_2D.
	@param	factor	A floating point (double) number.
	@return	A Size_2D in which the dimensions are the dimensions of the
		specified size multiplied by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Size_2D operator* (const Size_2D& size, double factor)
	{return Size_2D (size) *= factor;}

/**	Multiply a size by a factor.

	@param	factor	A floating point (double) number.
	@param	size	A Size_2D.
	@return	A Size_2D in which the dimensions are the dimensions of the
		specified size multiplied by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Size_2D operator* (double factor, const Size_2D& size)
	{return Size_2D (size) *= factor;}

/**	Divide a size by a factor.

	@param	size	A Size_2D.
	@param	factor	A floating point (double) number.
	@return	A Size_2D in which the dimensions are the dimensions of the
		specified size divided by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Size_2D operator/ (const Size_2D& size, double factor)
	{return Size_2D (size) /= factor;}

/**	Print a Size_2D description to an output stream.

	@param	stream	The ostream where the Size_2D will be printed.
	@param	size	The Size_2D to be printed.
	@return	The stream that was written.
*/
std::ostream& operator<< (std::ostream& stream, const Size_2D& size);

//******************************************************************************
/**	A <i>Rectangle</i> is a position with a size.

	The Rectangle's position is based in a Point_2D being at the upper
	left corner of the Rectangle, and its size is based in a Size_2D with
	the X-axis Width increasing to the right and the Y-axis Height
	increasing downward (raster order).

	@author		Bradford Castalia, UA/PIRL
	@version	$Revision: 1.27 $
	@see	Point_2D
	@see	Size_2D
*/
struct Rectangle
:	public Point_2D,
	public Size_2D
{
/*==============================================================================
	Constructors:
*/
/**	Constructs an empty Rectangle.

	The position is 0,0 and the size is 0,0.
*/
Rectangle ();

/**	Constructs a Rectangle from an x,y position and width,height size.

	@param	x	The horizontal (x-axis) position.
	@param	y	The vertical (y-axis) position.
	@param	width	The Width of the Rectangle.
	@param	height	The Height of the Rectangle.
*/
Rectangle
	(
	const Coordinate_Type	x,
	const Coordinate_Type	y,
	const Dimensions_Type	width = 0,
	const Dimensions_Type	height = 0
	);

/**	Constructs a Rectangle from a position and a size.

	@param	position	A Point_2D.
	@param	size		A Size_2D.
*/
Rectangle (const Point_2D& position, const Size_2D& size);

/**	Constructs a Rectangle from a size at position 0,0.

	@param	size		A Size_2D.
*/
Rectangle (const Size_2D& size);

/**	Constructs a Rectangle as a copy of another Rectangle.

	@param	rectangle	A Rectangle to be copied.
*/
Rectangle (const Rectangle& rectangle);

/*==============================================================================
	Accessors:
*/
/**	Set the position of this Rectangle.

	@param	x	The horizontal (x-axis) position of the Rectangle.
	@param	y	The vertical (y-axis) position of the Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& position (const Coordinate_Type& x,const Coordinate_Type& y)
	{Point_2D::position (x, y); return *this;}

/**	Set the position of this Rectangle.

	@param	point	A Point_2D whose dimensions are to be assigned to
		this Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& position (const Point_2D& point)
	{Point_2D::position (point); return *this;}

/**	Assign the position of the Rectangle from a Point_2D.

	@param	point	A Point_2D for the position of the Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& operator= (const Point_2D& point)
	{Point_2D::operator= (point); return *this;}

/**	Get the Rectangle position.

	@return	A Point_2D with the Rectangle position. <b>N.B.</b>: Changing
		this Point_2D will not change the position of the Rectangle.
*/
inline Point_2D position () const
	{return Point_2D (X, Y);}

/**	Convert the Rectangle to its corresponding Point_2D.

	@return	A Point_2D with the Rectangle position. 
*/
inline operator Point_2D () const
	{return position ();}

/**	Set the size of this Rectangle.

	@param	width	The Width of the Rectangle.
	@param	height	The Height of the Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& size
	(const Dimensions_Type& width, const Dimensions_Type& height)
	{Size_2D::size (width, height); return *this;}

/**	Set the size of this Rectangle.

	@param	size	A Size_2D to have its dimensions assigned to this
		Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& size (const Size_2D& size)
	{Size_2D::size (size); return *this;}

/**	Assign the size of the Rectangle from a Size_2D.

	@param	size	A Size_2D for the size of the Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& operator= (const Size_2D& size)
	{Size_2D::operator= (size); return *this;}

/**	Get the Rectangle size.

	@return	A Size_2D with the Rectangle size. <b>N.B.</b>: Changing
		this size will not change the size of the Rectangle.
*/
inline Size_2D size () const
	{return Size_2D (Width, Height);}

/**	Convert the Rectangle to its corresponding Size_2D.

	@return	A Size_2D with the Rectangle size.
*/
inline operator Size_2D () const
	{return size ();}

/**	Assign the position and size of another Rectangle to this Rectangle.

	@param	rectangle	A Rectangle whose position and size are to be
		assigned to this Rectangle.
	@return	This Rectangle.
*/
inline Rectangle& operator= (const Rectangle& rectangle)
	{
	if (this != &rectangle)
		{
		X = rectangle.X;
		Y = rectangle.Y;
		Width  = rectangle.Width;
		Height = rectangle.Height;
		}
	return *this;
	}

/**	Test if this Rectangle is equal to another Rectangle.

	The two Rectangles are equal if both their position and size
	dimensions are equal.

	@param	rectangle	The Rectangle to which this Rectangle is to be
		compared.
	@return	true if the two Rectangles are equal; false otherwise.
*/
inline bool operator== (const Rectangle& rectangle) const
	{return Point_2D::operator== ((const Point_2D)rectangle) &&
			 Size_2D::operator== ((const Size_2D)rectangle);}

/**	Test if this Rectangle is not equal to another Rectangle.

	The two Rectangles are not equal if either their position or size
	dimensions are not equal.

	@param	rectangle	The Rectangle to which this Rectangle is to be
		compared.
	@return	true if the two Rectangles are not equal; false otherwise.
*/
inline bool operator!= (const Rectangle& rectangle) const
	{return Point_2D::operator!= ((const Point_2D)rectangle) ||
			 Size_2D::operator!= ((const Size_2D)rectangle);}

/**	Test for all zero dimension values.

	@return	true if any dimension value is non-zero; false otherwise.
*/
inline operator bool ()
	{return Point_2D::operator bool () || Size_2D::operator bool ();}

/*==============================================================================
	Manipulators:
*/
/**	Add an offset.

	@param	offset	A Point_2D that provides the offset values.
	@return	This Rectangle with its values offset.
*/
inline Rectangle& operator+= (const Point_2D& offset)
	{Point_2D::operator+= (offset); return *this;}

/**	Add a size amount.

	@param	size	A Size_2D that provides the amount values.
	@return	This Rectangle with the amount added to its size values.
*/
inline Rectangle& operator+= (const Size_2D& size)
	{Size_2D::operator+= (size); return *this;}

/**	Add another Rectangle's point coordinate offset and size amount.

	@param	rectangle	A Rectangle.
	@return	This Rectangle with its point coordinate offset by the
		other Rectangle's point coordinate values, and the other Rectangle's
		size amount added to its size dimensions.
*/ 
inline Rectangle& operator+= (const Rectangle& rectangle)
	{
	Point_2D::operator+= (static_cast<Point_2D>(rectangle));
	Size_2D::operator+= (static_cast<Size_2D>(rectangle));
	return *this;
	}

/**	Subtract an offset.

	@param	offset	A Point_2D that provides the offset values.
	@return	This Rectangle with its values offset.
*/
inline Rectangle& operator-= (const Point_2D& offset)
	{Point_2D::operator-= (offset); return *this;}

/**	Subtract a size amount.

	<b>N.B.</b>: If the size Dimensions_Type values are an unsigned type
	(as they are by default) and a size amount to be subtracted is greater
	than the corresponding dimension value, the result will be zero.

	@param	size	A Size_2D that provides the amount values.
	@return	This Rectangle with the amount subtracted to its values.
*/
inline Rectangle& operator-= (const Size_2D& size)
	{Size_2D::operator-= (size); return *this;}

/**	Subtract another Rectangle's point coordinate offset and size amount.

	@param	rectangle	A Rectangle.
	@return	This Rectangle with its point coordinate offset by the
		other Rectangle's point coordinate values, and the other Rectangle's
		size amount subtracted from its size dimensions.
*/ 
inline Rectangle& operator-= (const Rectangle& rectangle)
	{
	Point_2D::operator-= (static_cast<Point_2D>(rectangle));
	Size_2D::operator-= (static_cast<Size_2D>(rectangle));
	return *this;
	}

/**	Multiply by a factor.

	The new size values will be rounded to the nearest Dimensions_Types.

	@param	factor	A factor by which to multiply the Size_2D dimensions.
	@return	This Rectangle with its values changed.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Rectangle& operator*= (double factor)
	{Size_2D::operator*= (factor); return *this;}

/**	Divide by a factor.

	The new size dimensions will be rounded to the nearest Dimensions_Type
	values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Dimensions_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Dimensions_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	factor	A factor by which to divide the Size_2D dimensions.
	@return	This Rectangle with its values changed.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Rectangle& operator/= (double factor)
	{Size_2D::operator/= (factor); return *this;}

/**	Take the intersection with another Rectangle.

	The intersection of two Rectangles is the overlapping area of both.

	@param	rectangle	The Rectangle to intersect with this Rectangle.
	@return	This Rectangle with its position and size set to the
		intersection with the other Rectangle. If the Rectangles do not
		intersect this will result in a Rectangle with no area (Width
		and Height both zero) but the position will be unchanged.
*/
Rectangle& operator&= (const Rectangle& rectangle);

/**	Take the union with another Rectangle.

	The union of two Rectangles is the bounding area of both.

	@param	rectangle	The Rectangle to unite with this Rectangle.
	@return	This Rectangle with its position and size set to the
		union - i.e. the bounding box - with the other Rectangle.
*/
Rectangle& operator|= (const Rectangle& rectangle);

};	//	End of Rectangle class.

/**	Add a Point_2D offset to a Rectangle.

	@param	rectangle	A Rectangle.
	@param	point		A Point_2D.
	@return	A Rectangle in which the coordinate point dimensions are the
		Point_2D have been added to the coordinate point of the Rectangle.
*/
inline Rectangle operator+ (const Rectangle& rectangle, const Point_2D& point)
	{return Rectangle (rectangle) += point;}

/**	Add a Point_2D offset to a Rectangle.

	@param	point		A Point_2D.
	@param	rectangle	A Rectangle.
	@return	A Rectangle in which the coordinate point dimensions of the
		Point_2D have been added to the coordinate point of the Rectangle.
*/
inline Rectangle operator+ (const Point_2D& point, const Rectangle& rectangle)
	{return Rectangle (rectangle) += point;}

/**	Add two Rectangles.

	@param	rectangle_1	A Rectangle.
	@param	rectangle_2	A Rectangle.
	@return	A Rectangle in which the coordinate point dimensions and the size
		amounts of the two Rectangles have been added.
*/
inline Rectangle operator+
	(const Rectangle& rectangle_1, const Rectangle& rectangle_2)
	{return Rectangle (rectangle_1) += rectangle_2;}

/**	Subtract a Point_2D offset from a Rectangle.

	@param	rectangle	A Rectangle.
	@param	point		A Point_2D.
	@return	A Rectangle in which the coordinate point dimensions of the
		Point_2D have been subtracted from the coordinate point of the
		Rectangle.
*/
inline Rectangle operator- (const Rectangle& rectangle, const Point_2D& point)
	{return Rectangle (rectangle) -= point;}

/**	Subtract two Rectangles.

	@param	rectangle_1	A Rectangle.
	@param	rectangle_2	A Rectangle.
	@return	A Rectangle in which the coordinate point dimensions and the size
		amounts of the two Rectangles have been added.
*/
inline Rectangle operator-
	(const Rectangle& rectangle_1, const Rectangle& rectangle_2)
	{return Rectangle (rectangle_1) -= rectangle_2;}

/**	Get the negation of a Rectangle.

	@param	rectangle A Rectangle.
	@return	A Rectangle in which the coordinate point dimensions are the
		negation of the Rectangle's coordinate point; the size is the same.
*/
inline Rectangle operator- (const Rectangle& rectangle)
	{return Rectangle (-rectangle.X, -rectangle.Y,
		rectangle.Width, rectangle.Height);}

/**	Add a size amount to a Rectangle.

	@param	rectangle A Rectangle.
	@param	size	A Size_2D.
	@return	A Rectangle in which the size amount has been added to the
		Rectangle size dimensions; the coordinate point is the same.
*/
inline Rectangle operator+ (const Rectangle& rectangle, const Size_2D& size)
	{return Rectangle (rectangle) += size;}

/**	Add a size amount to a Rectangle.

	@param	size	A Size_2D.
	@param	rectangle A Rectangle.
	@return	A Rectangle in which the size amount has been added to the
		negation of the Rectangle size dimensions; the coordinate point
		is the same.
*/
inline Rectangle operator+ (const Size_2D& size, const Rectangle& rectangle)
	{return Rectangle (rectangle) += size;}

/**	Subtract a size amount from a Rectangle.

	<b>N.B.</b>: If the size Dimensions_Type values are an unsigned type
	(as they are by default) and a size amount to be subtracted is greater
	than the corresponding dimension value, the result will be zero.

	@param	size	A Size_2D.
	@param	rectangle A Rectangle.
	@return	A Rectangle in which the size amount has been subtracted from
		the Rectangle size dimensions; the coordinate point is the same.
*/
inline Rectangle operator- (const Rectangle& rectangle, const Size_2D& size)
	{return Rectangle (rectangle) -= size;}

/**	Multiply a Rectangle by a factor.

	@param	rectangle	A Rectangle.
	@param	factor	A floating point (double) number.
	@return	A Size_2D in which the dimensions are the dimensions of the
		specified size multiplied by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Rectangle operator* (const Rectangle& rectangle, double factor)
	{return Rectangle (rectangle) *= factor;}

/**	Multiply a Rectangle by a factor.

	@param	factor	A floating point (double) number.
	@param	rectangle	A Rectangle.
	@return	A Rectangle in which the size dimensions are the dimensions
		of the specified Rectangle multiplied by the factor and {@link
		Round(double) rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Rectangle operator* (double factor, const Rectangle& rectangle)
	{return Rectangle (rectangle) *= factor;}

/**	Divide a Rectangle by a factor.

	The new size dimensions will be rounded to the nearest Dimensions_Type
	values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Dimensions_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Dimensions_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	rectangle	A Rectangle.
	@param	factor	A floating point (double) number.
	@return	A Rectangle in which the dimensions are the dimensions of the
		specified Rectangle divided by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Rectangle operator/ (const Rectangle& rectangle, double factor)
	{return Rectangle (rectangle) /= factor;}

/**	Get the intersection of two Rectangles.

	@param	rectangle_1	A Rectangle.
	@param	rectangle_2	A Rectangle.
	@return	A Rectangle with its position and size set to the
		intersection of the two Rectangles. If the Rectangles do not
		intersect this will result in a Rectangle with no area (Width
		and Height both zero) but the position of the first Rectangle.
*/
inline Rectangle operator&
	(const Rectangle& rectangle_1, const Rectangle& rectangle_2)
	{return Rectangle (rectangle_1) &= rectangle_2;}

/**	Get the union of two Rectangles.

	@param	rectangle_1	A Rectangle.
	@param	rectangle_2	A Rectangle.
	@return	A Rectangle with its position and size set to the
		union - i.e. the bounding box - of the two Rectangles.
*/
inline Rectangle& operator|
	(const Rectangle& rectangle_1, const Rectangle& rectangle_2)
	{return Rectangle (rectangle_1) |= rectangle_2;}

/**	Print a Rectangle description to an output stream.

	@param	stream	The ostream where the Rectangle will be printed.
	@param	rectangle	The Rectangle to be printed.
	@return	The stream that was written.
*/
std::ostream& operator<< (std::ostream& stream, const Rectangle& rectangle);

//******************************************************************************
/**	A <i>Cube</i> is a Rectangle with depth.

	A Cube does not have a Z-dimension position. Instead it has a Depth
	with the position of the Cube having an implicit Z-coordinate of
	zero; i.e. the facing surface of the Cube defined by its Rectangle is
	at the Z-coordinate origin with the Z-axis Depth increasing away from
	the observer (right handed coordinate system).

	@author		Bradford Castalia, UA/PIRL
	@version	$Revision: 1.27 $
	@see	Rectangle
*/
struct Cube
:	public Rectangle
{
//!	The Depth of the Cube.
Dimensions_Type
	Depth;

/*==============================================================================
	Constructors:
*/
/**	Constructs an empty Cube.

	The position is 0,0; the size is 0,0; the Depth is 0.
*/
Cube ();

/**	Constructs a Cube from an x,y position, width,height size, and depth.

	@param	x	The horizontal (x-axis) position.
	@param	y	The vertical (y-axis) position.
	@param	width	The Width of the Cube.
	@param	height	The Height of the Cube.
	@param	depth	The Depth of the Cube.
*/
Cube
	(
	const Coordinate_Type	x,
	const Coordinate_Type	y,
	const Dimensions_Type	width = 0,
	const Dimensions_Type	height = 0,
	const Dimensions_Type	depth = 0
	);

/**	Constructs a Cube from a position and a size.

	The Cube will have a Depth of 1.

	@param	position	A Point_2D.
	@param	size		A Size_2D.
*/
Cube (const Point_2D& position, const Size_2D& size);

/**	Constructs a Cube from a size.

	The Cube will have a position of 0,0 and a Depth of 1.

	@param	size		A Size_2D.
*/
Cube (const Size_2D& size);

/**	Constructs a Cube from a Rectangle.

	The Cube will have a Depth of 1.

	@param	rectangle	A Rectangle.
*/
Cube (const Rectangle& rectangle);

/**	Constructs a Cube as a copy of another Cube.

	@param	cube	A Cube to be copied.
*/
Cube (const Cube& cube);

/*==============================================================================
	Accessors:
*/
/**	Set the position of this Cube.

	@param	x	The horizontal (x-axis) position of the Cube.
	@param	y	The vertical (y-axis) position of the Cube.
	@return	This Cube.
*/
inline Cube& position (const Coordinate_Type& x,const Coordinate_Type& y)
	{Point_2D::position (x, y); return *this;}

/**	Set the position of this Cube.

	@param	point	A Point_2D whose dimensions are to be assigned as
		the position of this Cube.
	@return	This Cube.
*/
inline Cube& position (const Point_2D& point)
	{Point_2D::position (point); return *this;}

/**	Assign the position of the Cube from a Point_2D.

	@param	point	A Point_2D for the position of the Cube.
	@return	This Cube.
*/
inline Cube& operator= (const Point_2D& point)
	{Point_2D::operator= (point); return *this;}

/**	Set the size of this Cube.

	@param	width	The Width of the Cube.
	@param	height	The Height of the Cube.
	@return	This Cube.
*/
inline Cube& size
	(const Dimensions_Type& width, const Dimensions_Type& height)
	{Size_2D::size (width, height); return *this;}

/**	Set the size of this Cube.

	@param	size	A Size_2D to have its dimensions assigned to this
		Cube.
	@return	This Cube.
*/
inline Cube& size (const Size_2D& size)
	{Size_2D::size (size); return *this;}

/**	Set the Depth of this Cube.

	@param	depth	The Depth of this Cube.
	@return	This Cube.
*/
inline Cube& depth (Dimensions_Type depth)
	{Depth = depth; return *this;}

/**	Get the Depth of this Cube.

	@return	The Depth of this Cube.
*/
inline Dimensions_Type depth () const
	{return Depth;}

/**	Assign the size of the Cube from a Size_2D.

	@param	size	A Size_2D for the size of the Cube.
	@return	This Cube.
*/
inline Cube& operator= (const Size_2D& size)
	{Size_2D::operator= (size); return *this;}

/**	Set the dimensions of this Cube from a Rectangle.

	@param	rectangle	A Rectangle to have its dimensions assigned to
		this Cube.
	@return	This Cube.
*/
inline Cube& dimensions (const Rectangle& rectangle)
	{Rectangle::operator= (rectangle); return *this;}

/**	Set the dimensions of this Cube from a Rectangle.

	@param	rectangle	A Rectangle to have its dimensions assigned to
		this Cube.
	@return	This Cube.
*/
inline Cube& operator= (const Rectangle& rectangle)
	{Rectangle::operator= (rectangle); return *this;}

/**	Assign the dimensions of another Cube to this Cube.

	@param	cube	A Cube whose dimensions are be assigned
		to this Cube.
	@return	This Cube.
*/
inline Cube& operator= (const Cube& cube)
	{
	if (this != &cube)
		{
		X = cube.X;
		Y = cube.Y;
		Width  = cube.Width;
		Height = cube.Height;
		Depth = cube.Depth;
		}
	return *this;
	}

/**	Get the volume of this Cube.

	@return	The volume (Width * Height * Depth) of the Cube.
*/
inline unsigned long long volume () const
	{return area () * Depth;}

/**	Test if this Cube is equal to another Cube.

	The two Cubes are equal if both their Depth and Rectangle dimensions
	are equal.

	@param	cube	The Cube to which this Cube is to be compared.
	@return	true if the two Cubes are equal; false otherwise.
*/
inline bool operator== (const Cube& cube) const
	{return Depth == cube.Depth &&
		Rectangle::operator== ((const Rectangle)cube);}

/**	Test if this Cube is not equal to another Cube.

	The two Cubes are not equal if either their Depth or Rectangle
	dimensions are not equal.

	@param	cube	The Cube to which this Cube is to be compared.
	@return	true if the two Cubes are not equal; false otherwise.
*/
inline bool operator!= (const Cube& cube) const
	{return Depth != cube.Depth ||
		Rectangle::operator!= ((const Rectangle)cube);}

/**	Test for all zero dimension values.

	@return	true if any dimension value is non-zero; false otherwise.
*/
inline operator bool ()
	{return Depth != 0 || Rectangle::operator bool ();}

/**	Test for any zero dimension values.

	@return true if any dimension is zero; false otherwise.
*/
inline bool is_empty ()
	{return Depth == 0 || Size_2D::is_empty ();}

/*==============================================================================
	Manipulators:
*/
/**	Add an amount to the Cube Depth.

	<B>N.B.</B>: If the Depth Dimensions_Type is unsigned (the default)
	and the amount is negative, the resulting Depth will not be less
	than zero.

	@param	amount	An integer amount to add to the Depth.
	@return	This Cube with the amount applied.
*/
Cube& operator+= (int amount);

/**	Add another Cube's point coordinate offset, and depth and size amount.

	@param	cube	A Cube.
	@return	This Cube with its point coordinate offset by the other
		Cube's point coordinate values, its Depth increased the other
		Cube's Depth, and the other Cube's size amount added to its size
		dimensions.
*/ 
inline Cube& operator+= (const Cube& cube)
	{
	Rectangle::operator+= (static_cast<Rectangle>(cube));
	operator+= (cube.Depth);
	return *this;
	}

/**	Subtract an amount from the Cube Depth.

	<B>N.B.</B>: If the Depth Dimensions_Type is unsigned (the default)
	and the amount is positive, the resulting Depth will not be less
	than zero.

	@param	amount	An integer amount to subtract from the Depth.
	@return	This Cube with the amount applied.
*/
inline Cube& operator-= (int amount)
	{operator+= (-amount); return *this;}

/**	Subtract another Cube's point coordinate and depth offset and size amount.

	@param	cube	A Cube.
	@return	This Cube with its point coordinate offset negatively by the
		other Cube's point coordinate values, its Depth offset negatively
		by the other Cube's Depth, and the other Cube's size amount
		subtracted from its size dimensions.
*/ 
inline Cube& operator-= (const Cube& cube)
	{
	Rectangle::operator-= (static_cast<Rectangle>(cube));
	operator-= (cube.Depth);
	return *this;
	}

/**	Multiply by a factor.

	The new size values will be rounded to the nearest Dimensions_Types.

	@param	factor	A factor by which to multiply the dimensions.
	@return	This Cube with its dimension values changed.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Cube& operator*= (double factor)
	{
	Rectangle::operator*= (factor);
	Depth = Round (Depth * factor);
	return *this;
	}

/**	Divide by a factor.

	The new size dimensions will be rounded to the nearest Dimensions_Type
	values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Dimensions_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Dimensions_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	factor	A factor by which to divide the dimensions.
	@return	This Cube with its dimension values changed.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
Cube& operator/= (double factor);

/**	Take the intersection with another Cube.

	The intersection of the Depths of two Cubes is the minimum Depth; i.e.
	the two Cubes are assumed to be aligned at the zero X-Y plane.

	@param	cube	The Cube to intersect with this Cube.
	@return	This Cube with its position, size and depth set to the
		intersection with the other Cube. If the Cubes do not intersect
		this will result in an {@link is_empty() emtpy} Cube (Width,
		Height and Depth zero) but the position will be unchanged.
*/
Cube& operator&= (const Cube& cube);

/**	Take the union with another Cube.

	The union of the Depths of two Cubes is the maximum Depth; i.e.
	the two Cubes are assumed to be aligned at the zero X-Y plane.

	@param	cube	The Cube to unite with this Cube.
	@return	This Cube with its position, size and depth set to the
		union - i.e. the bounding volume - with the other Cube.
*/
Cube& operator|= (const Cube& cube);

};	//	End of Cube class.

/**	Add an amount to a Cube Depth.

	@param	cube	A Cube.
	@param	amount	An integer amount to add to the Depth.
	@return	A Cube in which the amount has been added to the
		Depth of the other cube.
	@see Cube::operator+=(int)
*/
inline Cube operator+ (const Cube& cube, int amount)
	{return Cube (cube) += amount;}

/**	Subtract an amount from a Cube Depth.

	@param	cube	A Cube.
	@param	amount	An integer amount to substract from the Depth of
		the other cube.
	@return	A Cube in which the amount has been subtracted from the
		Depth of the other cube.
	@see Cube::operator-=(int)
*/
inline Cube operator- (const Cube& cube, int amount)
	{return Cube (cube) -= amount;}

/**	Add two Cubes.

	@param	cube_1	A Cube.
	@param	cube_2	A Cube.
	@return	A Cube in which the coordinate point dimensions and the size
		and depth amounts of the two Cubes have been added.
	@see Cube::operator+=(const Cube&)
*/
inline Cube operator+
	(const Cube& cube_1, const Cube& cube_2)
	{return Cube (cube_1) += cube_2;}

/**	Subtract two Cubes.

	@param	cube_1	A Cube.
	@param	cube_2	A Cube.
	@return	A Cube in which the coordinate point dimensions and the size
		and depth amounts of the two Cubes have been subtracted.
	@see Cube::operator-=(const Cube&)
*/
inline Cube operator-
	(const Cube& cube_1, const Cube& cube_2)
	{return Cube (cube_1) -= cube_2;}

/**	Multiply a Cube by a factor.

	@param	cube	A Cube.
	@param	factor	A floating point (double) number.
	@return	A Cube in which the dimensions are the dimensions of the
		specified cube multiplied by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Cube operator* (const Cube& cube, double factor)
	{return Cube (cube) *= factor;}

/**	Multiply a Cube by a factor.

	@param	factor	A floating point (double) number.
	@param	cube	A Cube.
	@return	A Cube in which the dimensions are the dimensions of the
		specified cube multiplied by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Cube operator* (double factor, const Cube& cube)
	{return Cube (cube) *= factor;}

/**	Divide a Cube by a factor.

	The new size dimensions will be rounded to the nearest Dimensions_Type
	values.

	<b>N.B.</b>: Divide by zero is handled as a special case. If the
	coordinate value was zero it will remain zero. If the Dimensions_Type
	has an infinity value (determined by numeric_types) that is used, or
	its negative if the coordinate value is negative. Otherwise, if the
	Dimensions_Type is signed the type's max value is used, or the min
	value if the coordinate value is negative; for an unsigned value the
	max value is used.

	@param	cube	A Cube.
	@param	factor	A floating point (double) number.
	@return	A Cube in which the dimensions are the dimensions of the
		specified Cube divided by the factor and {@link Round(double)
		rounded}.
	@throws invalid_argument	If the factor is negative and the size values
		Dimensions_Type are an unsigned type (as they are by default).
*/
inline Cube operator/ (const Cube& cube, double factor)
	{return Cube (cube) /= factor;}

/**	Get the intersection of two Cubes.

	The intersection the Depths of two Cubes is the minimum Depth; i.e.
	the two Cubes are assumed to be aligned at the zero X-Y plane.

	@param	cube_1	A Cube.
	@param	cube_2	A Cube.
	@return	A Cube with its position, size and depth set to the
		intersection with the other Cube. If the Cubes do not intersect
		this will result in an emtpy Cube (Width, Height and Depth zero)
		but the position of the first Cube.
*/
inline Cube operator&
	(const Cube& cube_1, const Cube& cube_2)
	{return Cube (cube_1) &= cube_2;}

/**	Get the union of two Cubes.

	The union of the Depths of two Cubes is the maximum Depth; i.e.
	the two Cubes are assumed to be aligned at the zero X-Y plane.

	@param	cube_1	A Cube.
	@param	cube_2	A Cube.
	@return	A Cube with its position, size and depth set to the
		union - i.e. the bounding volume - of the two Cubes.
*/
inline Cube& operator|
	(const Cube& cube_1, const Cube& cube_2)
	{return Cube (cube_1) |= cube_2;}

/**	Print a Cube description to an output stream.

	@param	stream	The ostream where the Cube will be printed.
	@param	cube	The Cube to be printed.
	@return	The stream that was written.
*/
std::ostream& operator<< (std::ostream& stream, const Cube& cube);


}	//	namespace PIRL
#endif
