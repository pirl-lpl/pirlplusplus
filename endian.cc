/*	endian - Manages the endian-ness of data.

PIRL CVS ID: $Id: endian.cc,v 1.13 2009/10/03 00:00:39 castalia Exp $

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

namespace PIRL
{
bool
host_is_high_endian ()
{
union
	{
	unsigned int	integer;
	unsigned char	bytes[sizeof (int)];
	}
	value;
value.integer  = 0;
value.bytes[0] = 1;
return (value.integer != 1);
}


void
reorder_bytes
	(
	unsigned char*			data,
	const unsigned long		amount
	)
{
register unsigned char
	datum,
	*end = data + amount;
while (data < --end)
	{
	datum    = *data;
	*data++  = *end;
	*end     = datum;
	}
}


void
reorder_bytes
	(
	unsigned char*			data,
	const unsigned long		groups,
	const unsigned int		size
	)
{
if (! data)
	return;
if (size == 1)
	{
	reorder_bytes (data, groups);
	return;
	}
register unsigned char
	datum,
	*end = data + (groups * size);
register unsigned int
	counter;
while (data < (end -= size))
	{
	for (counter = size;
		 counter--;)
		{
		datum    = *data;
		*data++  = *end;
		*end++   = datum;
		}
	(end -= size);
	}
}


void
swap_bytes
	(
	unsigned char*			data,
	const unsigned int		groups,
	const unsigned int		size
	)
{
if (! data)
	return;
if (size < 1)
	return;
register unsigned char
	datum,
	*end = data + (groups * size),
	*first,
	*last;
while (data < end)
	{
	first = data;
	data += size;
	last  = data;
	while (first < --last)
		{
		datum    = *first;
		*first++ = *last;
		*last    = datum;
		}
	}
}


}   //  PIRL namespace
