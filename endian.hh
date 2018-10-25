/*	endian - Manages the endian-ness of data.

PIRL CVS ID: $Id: endian.hh,v 1.18 2009/10/03 00:00:39 castalia Exp $

Copyright (C) 2003, 2004, 2005  Arizona Board of Regents 
on behalf of the Planetary Image Research Laboratory, 
Lunar and Planetary Laboratory at the University of Arizona.

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
#ifndef _endian_h
#define _endian_h

namespace PIRL
{
/**	Tests if the host system is high-endian.

	A system is high-endian if native multi-byte binary data is ordered
	most significant byte (MSB) first. In this case the MSB will be at
	the lowest address of each datum's bytes, with each subsequent byte
	at a higher address until the least significant byte (LSB) is at
	the highest address.

	@return	true if the host system is high-endian; false otherwise.
*/
bool host_is_high_endian ();

/**	Tests if the host system is high-endian.

	@see host_is_high_endian()
*/
inline bool high_endian_host ()	{return host_is_high_endian ();}

/**	Tests if the host system is big-endian.

	@see host_is_high_endian()
*/
inline bool big_endian_host ()	{return host_is_high_endian ();}

/**	Tests if the host system is low-endian.

	@see host_is_high_endian()
*/
inline bool low_endian_host ()	{return ! host_is_high_endian ();}

/**	Tests if the host system is little-endian.

	@see host_is_high_endian()
*/
inline bool little_endian_host ()	{return ! host_is_high_endian ();}

/**	Reorders data bytes.

	The bytes are mirrored, in place. The first byte is swapped with
	the last byte, the second byte is swapped with the next-to-last
	byte, etc. until all byte pairs have been swapped.

	@param	data	A pointer (unsigned char*) to the first byte to be
		reordered. <b>WARNING</b>: This argument is not checked for NULL.
	@param	amount	The number of bytes to be reordered.
*/
void reorder_bytes (unsigned char* data, const unsigned long amount);

/**	Reorders data bytes in groups.

	The byte groups are mirrored, in place. The first group is swapped with
	the last group, the second group is swapped with the next-to-last
	group, etc. until all group pairs have been swapped. The bytes within
	each group are not swapped; they stay in the same order.

	@param	data	A pointer (unsigned char*) to the first byte to be
		reordered.
	@param	groups	The number of groups to be reordered.
	@param	size	The number of bytes per group.
*/
void reorder_bytes (unsigned char* data, const unsigned long groups,
	const unsigned int size);

/**	Swap groups of data bytes.

	The byte groups are swapped in place. The first byte of each group
	is swapped with the last byte in the group, the second byte is
	swapped with the next-to-last byte in the group, etc. until all
	bytes within the group have been reorderd. All byte groups are
	swapped in the same way.

	@param	data	A pointer (unsigned char*) to the first byte to be
		reordered.
	@param	groups	The number of groups to be reordered. If this is
		zero nothing is done.
	@param	size	The number of bytes per group. If this is less than
		two nothing is done.
*/
void
swap_bytes
	(
	unsigned char*			data,
	const unsigned int		groups,
	const unsigned int		size
	);

/**	A MSB (high-endian) data value is coerced to/from native byte order.

	If the host system is high-endian the data is left unchanged;
	otherwise, it is reordered. Therefore, if the argument value is in
	MSB order the result is in native order; it the argument value is
	in native order the result in in MSB order. 

	<b>N.B.</b>: The data value is reordered in place. Make a copy
	first if the original datum is to remain unchanged; e.g.:

	<code>
	int tmp_value, value = 1;<br>
	cout << MSB_value (tmp_value = value)<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<< " = MSB_value (" << value << ')' << endl;
	</code>

	@param	value	A datum to be coerced to MSB byte order. The datum
		should be a primitive type, otherwise the results are undefined.
	@return	The value (reference) in MSB order.
	@see	high_endian_host()
	@see	reorder_bytes(unsigned char*, const unsigned long, const unsigned int)
*/
template<typename T>
T&
MSB_native
	(
	T&	value
	)
{
if (! host_is_high_endian ())
	reorder_bytes
		(reinterpret_cast<unsigned char*>(&value), sizeof (T));
return value;
}
//!	Backwards compatibility with obsolete name.
#define MSB_value	MSB_native

/**	A LSB (low-endian) data value is coerced to/from native byte order.

	If the host system is low-endian the data is left unchanged;
	otherwise, it is reordered. Therefore, if the argument value is in
	LSB order the result is in native order; it the argument value is
	in native order the result in in LSB order. 

	<b>N.B.</b>: The data value is reordered in place. Make a copy
	first if the original datum is to remain unchanged; e.g.:

	<code>
	int tmp_value, value = 1;<br>
	cout << LSB_value (tmp_value = value)<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<< " = LSB_value (" << value << ')' << endl;
	</code>

	@param	value	A datum to be coerced to LSB byte order. The datum
		should be a primitive type, otherwise the results are undefined.
	@return	The value (reference) in LSB order.
	@see	high_endian_host()
	@see	reorder_bytes(unsigned char*, const unsigned long, const unsigned int)
*/
template<typename T>
T&
LSB_native
	(
	T&	value
	)
{
if (host_is_high_endian ())
	reorder_bytes
		(reinterpret_cast<unsigned char*>(&value), sizeof (T));
return value;
}
//!	Backwards compatibility with obsolete name.
#define LSB_value	LSB_native


}   //  PIRL namespace
#endif
