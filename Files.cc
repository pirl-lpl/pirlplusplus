/*	Files

PIRL CVS ID: $Id: Files.cc,v 2.4 2010/11/21 20:33:26 castalia Exp $

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
/*
Note: the Windows implementations of several functions were derived from 
the opensource Qt framework as model code. The Qt framework can be downloaded
from http://qt.nokia.com; the model file was corelib/io/qfsfileengine_win.cpp.
*/

#include	"Files.hh"

#include	<string>
using std::string;

#include	<stdexcept>
using std::runtime_error;

#include	<sstream>
using std::ostringstream;
#include	<iomanip>
using std::endl;

#ifdef _WIN32
//	MS/Windows implementation system dependencies.
#include <windows.h>		
#include <Lmcons.h>			//	For username length
#include <Aclapi.h>			//	For Windows file permissions
#include <sys\types.h>
#include <Userenv.h>		//	For home_dir()
#include <ostream>
#include <sys\stat.h>
#define stat _stat			//	stat struct equivalence.
#include <direct.h>			//	For CWD()
#define getcwd _getcwd		//	getcwd() equivalence.
#define MAXPATHLEN 5000

#else
//	Unix implementation system dependencies.
#include	<sys/stat.h>	//	For stat()
#include	<unistd.h>		//	For getcwd()
#include	<sys/param.h>	//	For MAXPATHLEN
#include	<pwd.h>			//	For getpwuid
#include	<sys/types.h>	//	For uid_t
#endif

#ifndef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISLNK
#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#endif


#if defined (DEBUG)
/*******************************************************************************
	DEBUG controls
*/
#define DEBUG_ALL			-1
#define DEBUG_FILES			(1 << 0)

#include	<iostream>
using std::clog;

#endif	//	DEBUG


namespace PIRL
{
/*==============================================================================
	Constants
*/
const char
	*Files_ID =
		"PIRL::Files ($Revision: 2.4 $ $Date: 2010/11/21 20:33:26 $)";


#define PATHNAME_DELIMITER_UNIX		'/'
#define PATHNAME_DELIMITER_WIN32	'\\'

#ifndef PATHNAME_DELIMITER
#ifdef _WIN32
#define PATHNAME_DELIMITER		PATHNAME_DELIMITER_WIN32
#else
#define PATHNAME_DELIMITER		PATHNAME_DELIMITER_UNIX
#endif
#endif

const char
	FILE_PATHNAME_DELIMITER			= PATHNAME_DELIMITER;

#ifndef EXTENSION_DELIMITER
#define EXTENSION_DELIMITER		'.'
#endif
const char
	FILE_EXTENSION_DELIMITER		= EXTENSION_DELIMITER;

#ifdef _WIN32
const char
	*LINK_FILE_EXTENSION			= "link";
#endif

/*==============================================================================
	Functions
*/
bool
file_exists
	(
	const std::string&	pathname
	)
{
#ifdef _WIN32
int
	fileAttr = GetFileAttributes (pathname.c_str ());
return (fileAttr != INVALID_FILE_ATTRIBUTES);

#else
struct stat
	file_status;
return ! stat (pathname.c_str (), &file_status);
#endif
}


bool
file_is_normal
  (
  const std::string&	pathname
  )
{
struct stat
	file_status;
if (stat (pathname.c_str (), &file_status) == 0)
	return S_ISREG (file_status.st_mode);
return false;
}


bool
file_is_directory
  (
  const std::string&	pathname
  )
{
#ifdef _WIN32
int
	fileAttr = GetFileAttributes (pathname.c_str ());
return
	(fileAttr != INVALID_FILE_ATTRIBUTES && 
	(fileAttr & FILE_ATTRIBUTE_DIRECTORY));

#else
struct stat
	file_status;
if (stat (pathname.c_str (), &file_status) == 0)
	return S_ISDIR (file_status.st_mode);
return false;
#endif
}


bool
file_is_link
  (
  const std::string&	pathname
  )
{
struct stat
	file_status;

#ifdef _WIN32
/*
	lstat isn't provided for Windows.
	There is no special check available for a link file in Windows,
	so just check for a valid file and the special extension.
*/
if (GetFileAttributes (pathname.c_str ()) != INVALID_FILE_ATTRIBUTES)
	return file_extension (pathname) == LINK_FILE_EXTENSION;

#else
if (lstat (pathname.c_str (), &file_status) == 0)
	return S_ISLNK (file_status.st_mode);
#endif

return false;
}


off_t
file_size
	(
	const std::string&	pathname
	)
{
struct stat
	file_status;
if (stat (pathname.c_str (), &file_status) == 0)
	return file_status.st_size;
return -1;
}


bool
file_is_absolute
  (
  const std::string&	pathname
  )
{
#ifdef _WIN32
int
	length = pathname.length ();
if (length)
	{
	if (length > 2 &&
		((pathname[0] >= 'A' && pathname[0] <= 'Z') ||
		 (pathname[0] >= 'a' && pathname[0] <= 'z')) && 
		  pathname[1] == ':' && 
		 (pathname[2] == PATHNAME_DELIMITER_WIN32 ||
		  pathname[2] == PATHNAME_DELIMITER_UNIX))
		/*
			pathname with drive + Win32 backslash or UNIX forward slash:
				C:\dir\file.txt or C:/dir/file.txt
		*/
		return true;

	return
		/*
			pathname based from root: \directory\file.txt or /directory/file.txt
			or mapped network pathname: \\CEREBRUS\directory\file.txt
		*/
		pathname[0] == PATHNAME_DELIMITER_WIN32 ||
		pathname[0] == PATHNAME_DELIMITER_UNIX;
	}
return false;

#else
return
	pathname.length () &&
	pathname[0] == FILE_PATHNAME_DELIMITER;
#endif
}


std::string&
clean_pathname
	(
	std::string&	pathname
	)
{
#if ((DEBUG) & DEBUG_FILES)
clog << ">>> clean_pathname:" << endl
	 << "    " << pathname << endl;
#endif
string::size_type
	index;
static char
	delim_dot_delim[] =
		{FILE_PATHNAME_DELIMITER, '.', FILE_PATHNAME_DELIMITER, 0};
for (index = pathname.find (delim_dot_delim);
	 index != string::npos;
	 index = pathname.find (delim_dot_delim, index))
	pathname.replace (index, 3, 1, FILE_PATHNAME_DELIMITER);
#if ((DEBUG) & DEBUG_FILES)
clog << "    " << pathname << endl;
#endif

static char
	delim_delim[] =
		{FILE_PATHNAME_DELIMITER, FILE_PATHNAME_DELIMITER, 0};
for (index = pathname.find (delim_delim);
	 index != string::npos;
	 index = pathname.find (delim_delim, index))
	pathname.replace (index, 2, 1, FILE_PATHNAME_DELIMITER);

if (pathname.length () &&
	pathname[pathname.length () - 1] == FILE_PATHNAME_DELIMITER)
	pathname.erase (pathname.length () - 1);

#if ((DEBUG) & DEBUG_FILES)
clog << "    " << pathname << endl
	 << "<<< clean_pathname" << endl;
#endif
return pathname;
}


std::string
CWD ()
{
char
	pathname[MAXPATHLEN];
if (! getcwd (pathname, MAXPATHLEN))
	{
	ostringstream
		message;
	message << Files_ID << endl
			<< "Unable to obtain the current working directory pathname.";
	throw runtime_error (message.str ());
	}
return pathname;
}


std::string
file_pathname
	(
	const std::string&	filename
	)
{
string
	pathname;
if (file_is_absolute (filename))
	pathname = filename;
else
	pathname = CWD () + FILE_PATHNAME_DELIMITER + filename;
return clean_pathname (pathname);
}


std::string
file_pathname
	(
	const std::string&	directory,
	const std::string&	filename
	)
{
string
	pathname;
if (file_is_absolute (filename))
	pathname = filename;
else
	{
	if (file_is_absolute (directory))
		pathname = directory + FILE_PATHNAME_DELIMITER + filename;
	else
		pathname =
			CWD () + FILE_PATHNAME_DELIMITER +
			directory + FILE_PATHNAME_DELIMITER + filename;
	}
return clean_pathname (pathname);
}


std::string
file_directory
	(
	const std::string&	pathname
	)
{
string
	directory;
string::size_type
	index = pathname.rfind (FILE_PATHNAME_DELIMITER);
if (index != string::npos)
	{
	if (index == 0)
		directory = FILE_PATHNAME_DELIMITER;
	else
		directory = pathname.substr (0, index);
	}
return directory;
}


std::string
file_directory_pathname
	(
	const std::string&	pathname
	)
{
string
	directory = file_directory (pathname);
if (directory.empty ())
	directory = CWD ();
else if (! file_is_absolute (directory))
	directory = CWD () + FILE_PATHNAME_DELIMITER + directory;
return clean_pathname (directory);
}


std::string
file_name
	(
	const std::string&	pathname
	)
{
string::size_type
	index = pathname.rfind (FILE_PATHNAME_DELIMITER);
if (index != string::npos)
	return pathname.substr (++index);
return pathname;
}


std::string
file_basename
	(
	const std::string&	pathname
	)
{
string::size_type
	index = pathname.rfind (FILE_EXTENSION_DELIMITER);
if (index != string::npos)
	return pathname.substr (0, index);
return pathname;
}


std::string
file_extension
	(
	const std::string&	pathname
	)
{
string::size_type
	index = pathname.rfind (FILE_EXTENSION_DELIMITER);
if (index != string::npos)
	return pathname.substr (++index);
return "";
}


#ifndef DOXYGEN_PROCESSING
namespace
{
string
	*host_name = NULL,
	*user_name = NULL,
	*home_dir  = NULL;
	
#ifdef _WIN32
bool	
	got_user_info = false;	
TRUSTEE 
	trustee;
//	>>> WARNING <<< This array must be static!
char
	trustee_name[100];
enum
	{
	read_mask  = 0x00000001,
	write_mask = 0x00000002
	};	


void
get_user_info ()
{
if (! got_user_info)
	{
	HANDLE
		process = ::GetCurrentProcess();
	HANDLE
		token = 0;
	if (::OpenProcessToken (process, TOKEN_QUERY, &token)) 
		{
		char
			path[MAXPATHLEN] = {0};
		unsigned long
			size = MAXPATHLEN;
		if (GetUserProfileDirectory (token, path, &size))
			{
			home_dir = new string (path);	

			if (::GetTokenInformation
				(token, TokenUser, trustee_name, sizeof (trustee_name), &size))
				{
				BuildTrusteeWithSid
					(&trustee, ((TOKEN_USER *)trustee_name)->User.Sid);
				got_user_info = true;
				}
			}
		::CloseHandle (token);
		}
	if (! got_user_info)
		{
		ostringstream
			message;
		message << Files_ID << endl
				<< "Couldn't obtain account information for the current user.";
		throw runtime_error (message.str ());	
		}
	}
}


int
file_attributes
	(
	const string&	pathname
	)
{
get_user_info ();

PSID
	pOwner = 0,
	pGroup = 0;
PACL
	pDacl = NULL;
PSECURITY_DESCRIPTOR
	pSD;
if (GetNamedSecurityInfo
		((char *)pathname.c_str(),
		SE_FILE_OBJECT,
		OWNER_SECURITY_INFORMATION |
		GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION,
		&pOwner,
		&pGroup,
		&pDacl,
		0,
		&pSD)
		!= ERROR_SUCCESS)
	{
	ostringstream
		message;
	message
		<< Files_ID << endl
		<< "Couldn't get security info for file \"" << pathname << "\".";
	throw runtime_error (message.str ());
	}
			 
ACCESS_MASK
	access_mask;
if (GetEffectiveRightsFromAcl (pDacl, &trustee, &access_mask) != ERROR_SUCCESS)
	{
	ostringstream
		message;
	message
		<< Files_ID << endl
		<< "Couldn't get effective rights in the access control list" << endl
		<< "for file \"" << pathname << "\".";
	throw runtime_error (message.str ());
	}
return access_mask;
}


#else
uid_t
	effective_user_ID  = (uid_t)-1;
gid_t
	effective_group_ID = (gid_t)-1;


void
get_password_entry_info ()
{
uid_t
	UID = ::geteuid ();
struct passwd
	*password_entry = ::getpwuid (UID);
if (! password_entry)
	{
	ostringstream
		message;
	message << Files_ID << endl
			<< "Unable to obtain a password entry for effective UID "
				<< UID << '.';
	throw runtime_error (message.str ());
	}
user_name = new string (password_entry->pw_name);
home_dir  = new string (password_entry->pw_dir);
}
#endif

}
#endif


bool
file_is_readable
  (
  const std::string&	pathname
  )
{
#ifdef _WIN32
return
	file_exists (pathname) &&
	(file_attributes (pathname) & read_mask);

#else
struct stat
	file_status;
return
	(stat (pathname.c_str (), &file_status) == 0 &&
	(            (file_status.st_mode & S_IROTH) ||
		(        (file_status.st_mode & S_IRGRP) &&
	(unsigned int)file_status.st_gid == (unsigned int)GID ()) ||
		(        (file_status.st_mode & S_IRUSR) &&
	(unsigned int)file_status.st_uid == (unsigned int)UID ())));
#endif
}


bool
file_is_writeable
  (
  const std::string&	pathname
  )
{
#ifdef _WIN32
return
	file_exists (pathname) &&
	(file_attributes (pathname) & write_mask);

#else
struct stat
	file_status;
return
	(stat (pathname.c_str (), &file_status) == 0 &&
	(            (file_status.st_mode & S_IWOTH) ||
		(        (file_status.st_mode & S_IWGRP) &&
	(unsigned int)file_status.st_gid == (unsigned int)GID ()) ||
		(        (file_status.st_mode & S_IWUSR) &&
	(unsigned int)file_status.st_uid == (unsigned int)UID ())));
#endif
}


std::string
home_directory_pathname ()
{
if (! home_dir)
	{
	#ifdef _WIN32
	get_user_info ();

	#else
	get_password_entry_info ();
	#endif
	}
return string (*home_dir);
}


std::string
username ()
{
if (! user_name)
	{
	#ifdef _WIN32
	char
		name[UNLEN+1] = {0};
	unsigned long
		name_length = sizeof (name);
	GetUserName (name, &name_length);
	user_name = new string (name);

	#else
	get_password_entry_info ();
	#endif
	}
return string (*user_name);
}


unsigned int
UID ()
{
#ifdef _WIN32
return 0;

#else
if (effective_user_ID == (uid_t)-1)
	effective_user_ID = geteuid ();
return (unsigned int)effective_user_ID;
#endif
}


unsigned int
GID ()
{
#ifdef _WIN32
return 0;

#else
if (effective_group_ID == (gid_t)-1)
	effective_group_ID = getegid ();
return (unsigned int)effective_group_ID;
#endif
}


std::string
hostname ()
{
if (! host_name)
	{
	#ifdef _WIN32
	//	Initialize the Windows Socket DLL.
	WSADATA
		wsa_Data;
	int
		wsa_ReturnCode = WSAStartup (0x101, &wsa_Data);
	#endif

	char
		name[256] = {0};	//	Linux, Solaris and OS X say this is long enough.
	gethostname (name, sizeof (name));
	host_name = new string (name);
	}
return string (*host_name);
}

}   //  PIRL namespace
