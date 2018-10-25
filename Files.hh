/*	Files

PIRL CVS ID: $Id: Files.hh,v 1.11 2012/07/20 00:25:53 castalia Exp $

Copyright (C) 2006-2010 Arizona Board of Regents on behalf of the
Planetary Image Research Laboratory, Lunar and Planetary Laboratory at
the University of Arizona.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License, version 2, as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef	_Files_
#define	_Files_

#include	<string>
#include	<sys/types.h>	//	For off_t.

namespace PIRL
{
/*==============================================================================
	Constants
*/
//!	Module identification name with source code version and date.
extern const char
	*Files_ID;
	
//!	Host filesystem pathname delimiter ('/').
extern const char
	FILE_PATHNAME_DELIMITER;

//!	Filename extension delimiter ('.').
extern const char
	FILE_EXTENSION_DELIMITER;

/*==============================================================================
	Functions
*/
/**	Test if a file exists at a pathname.

	@param	pathname	The filesystem pathname to test.
	@return	true if the file exists; false otherwise.
*/
bool file_exists (const std::string& pathname);

/**	Test if a file at a pathname is readable.

	Readability is determined by the system's access method. The file
	must be readable by the real ID of the process.

	@param	pathname	The pathname to test.
	@return	true if the file is accessable and can be read; false otherwise.
	@throws	runtime_error	If the file attributes can not be obtained
		(MS/Windows only).
*/
bool file_is_readable (const std::string& pathname);

/**	Test if a file at a pathname is writeable.

	Writeability is determined by the system's access method. The file
	must be writeable by the real ID of the process.

	@param	pathname	The pathname to test.
	@return	true if the file is accessable and can be written; false
		otherwise.
	@throws	runtime_error	If the file attributes can not be obtained
		(MS/Windows only).
*/
bool file_is_writeable (const std::string& pathname);

/**	Test if a file at a pathname is a normal file.

	@param	pathname	The pathname to test.
	@return	true if the file is accessable and is a normal type of file;
		false otherwise.
*/
bool file_is_normal (const std::string& pathname);

/**	Test if a file at a pathname is a directory.

	@param	pathname	The pathname to test.
	@return	true if the file is accessable and is a directory; false
		otherwise.
*/
bool file_is_directory (const std::string& pathname);

/**	Test if a file at a pathname is a link file.

	@param	pathname	The pathname to test.
	@return	true if the file is accessable and is a link; false
		otherwise.
*/
bool file_is_link (const std::string& pathname);

/**	Get the size of a file.

	<b>N.B.</b>: For MS/Windows a directory always has a file size of
	zero.

	@param	pathname	The pathname to the file.
	@return	The size of the file in bytes. This will be -1 if the file
		could not be accessed. The return data type is off_t, the natural
		type for a file offset value for the host system.
*/
off_t file_size (const std::string& pathname);

/**	Test if a pathname is an absolute pathname.

	An absolute pathname begins with the #FILE_PATHNAME_DELIMITER character.

	@param	pathname	The pathname to test.
	@return	true if the pathname is an absolute pathname; false otherwise.
*/
bool file_is_absolute (const std::string& pathname);

/**	Clean a pathname of redundant segments.

	All sequences of multiple #FILE_PATHNAME_DELIMITER characters are
	replaced with a single delimiter. All sequences of
	#FILE_PATHNAME_DELIMITER-'.'-#FILE_PATHNAME_DELIMITER characters are
	replaced with a single delimiter. Any trailing #FILE_PATHNAME_DELIMITER
	is removed.

	@param	pathname	The pathname from which to obtain the extension.
	@return	The cleaned pathname. <b>N.B.</b>: The pathname string is
		modified in place.
*/
std::string& clean_pathname (std::string& pathname);

/**	Get the full pathname for a filename.

	If the filename is relative the {@link #CWD() current working directory}
	of the process is prepended.

	The directory pathname is {@link clean_pathname(std::string&)
	cleaned} before it is returned.

	@param	filename	The filename for which to obtain an absolute
		pathname.
	@return	The absolute pathname for the filename.
	@throws runtime_error	If the current working directory can not
		be obtained.
*/
std::string file_pathname (const std::string& filename);

/**	Get the full pathname for a filename.

	If the filename is relative the directory is prepended. If the
	directory is relative the {@link #CWD() current working directory}
	of the process is prepended to it.

	The directory pathname is {@link clean_pathname(std::string&)
	cleaned} before it is returned.

	@param	directory	The directory to prepend to the filename if
		it is not an absolute pathname.
	@param	filename	The filename for which to obtain an absolute
		pathname.
	@return	The absolute pathname for the filename.
	@throws runtime_error	If the current working directory can not
		be obtained.
*/
std::string file_pathname
	(const std::string& directory, const std::string& filename);

/**	Get the current working directory of the process.

	This is a convenience function for the system getcwd function.

	@return	The absolute pathname for the current working directory
		of the process.
	@throws runtime_error	If the current working directory can not
		be obtained.
*/
std::string CWD ();

/**	Get the leading directory portion of a pathname.

	The leading directory portion of a pathname is that substring
	preceding the last #FILE_PATHNAME_DELIMITER, or the empty string if
	it does not contain the delimiter. <b>N.B.</b>: If the pathname contains
	only one delimiter that is the first character, then the leading
	directory pathname is the delimiter (i.e. the filesystem root).

	@param	pathname	The pathname from which to obtain the directory
		pathname.
	@return	The directory pathname from the pathname.
*/
std::string file_directory (const std::string& pathname);

/**	Get the leading directory pathname portion of a pathname.

	The {@link #file_directory(const std::string&) leading directory}
	portion of the pathname is obtained. If it is not an {@link
	file_is_absolute(const std::string&) absolute pathname} the {@link
	#CWD() current working directory} of the process is prepended.

	The directory pathname is {@link clean_pathname(std::string&)
	cleaned} before it is returned.

	@param	pathname	The pathname from which to obtain the directory
		pathname.
	@return	The directory pathname from the pathname.
	@throws runtime_error	If the current working directory can not
		be obtained.
*/
std::string file_directory_pathname (const std::string& pathname);

/**	Get the final name portion of a pathname.

	The final name portion of a pathname is that substring following the
	last #FILE_PATHNAME_DELIMITER, or the entire string if it does not
	contain the delimiter.

	@param	pathname	The pathname from which to obtain the file's name.
	@return	The name of the file referenced by the pathname.
*/
std::string file_name (const std::string& pathname);

/**	Get the basename of a file's pathname.

	The basename is that portion of the path preceeding the last
	#FILE_EXTENSION_DELIMITER; or the entire string if there
	it does not contain the delimiter. <b>N.B.</b>: Any directory
	segments of the pathname are retained in the basename; these
	can be removed using the file_name method.

	@param	pathname	The pathname from which to obtain the basename.
	@return	The basename of the pathname.
*/
std::string file_basename (const std::string& pathname);

/**	Get a file's extension.

	The extension is that portion of the path following the last
	#FILE_EXTENSION_DELIMITER. This will be the empty string if no
	extension is present or the pathname ends with the delimiter.

	@param	pathname	The pathname from which to obtain the extension.
	@return	The extension of the file referenced by the pathname.
*/
std::string file_extension (const std::string& pathname);

/**	Get the user's home directory pathname.

	@return The pathname of the effective user home directory.
	@throws	runtime_error	If the user info can not be obtained.
*/
std::string home_directory_pathname ();

/**	Get the effective username.

	@return	The name of the effective user.
	@throws	runtime_error	If the user info can not be obtained (UNIX
		only).
*/
std::string username ();

/**	Get the effective user ID.

	@return	The effective user ID value.
*/
unsigned int UID ();

/**	Get the effective group ID.

	@return	The effective group ID value.
*/
unsigned int GID ();

/**	Get the name of the host system.

	@return	The name of the host system.
*/
std::string hostname ();

}   //  PIRL namespace
#endif
