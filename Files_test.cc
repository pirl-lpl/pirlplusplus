/*	Files_test

PIRL CVS ID: $Id: Files_test.cc,v 1.9 2010/11/16 02:55:30 castalia Exp $

Copyright (C) 2006-2007  Arizona Board of Regents on behalf of the Planetary
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
#include <stdexcept>
#include <sys/stat.h>
#include <cstdlib>
using namespace std;

#include "Files.hh"
using namespace PIRL;

#ifdef _WIN32
//	MS/Windows implementation system dependencies.
#include <windows.h>		
#endif

int
main
	(
	int		count,
	char	**argument
	)
{
cout << "*** Files test" << endl
	 << "    " << Files_ID << endl;
int
	Tests_Total = 0,
	Tests_Passed = 0;
bool
	test_links = true,
	passed = false;
string
	name;

//	hostname
++Tests_Total;
name = hostname ();
passed = ! name.empty ();
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "hostname () = \"" << name << '"' << endl;

try
	{
	//	username
	++Tests_Total;
	name = username ();
	passed = ! name.empty ();
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "username () = \"" << name << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from username ()" << endl
		 << except.what () << endl;
	}

//	UID
unsigned int
	ID_value;
++Tests_Total;
passed = ((ID_value = UID ()) != (unsigned int)-1);
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "UID () = " << ID_value << endl;

//	GID
++Tests_Total;
passed = ((ID_value = GID ()) != (unsigned int)-1);
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "GID () = " << ID_value << endl;

//	home_directory_pathname
++Tests_Total;
try
	{
	name = home_directory_pathname ();
	passed = ! name.empty ();
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "home_directory_pathname () = \"" << name << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from home_directory_pathname ()" << endl
		 << except.what () << endl;
	}

//	CWD
++Tests_Total;
string
	cwd;
try
	{
	cwd = CWD ();
	passed = ! cwd.empty ();
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "CWD () = \"" << cwd << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from CWD ()" << endl
		 << except.what () << endl;
	cwd =
		FILE_PATHNAME_DELIMITER + string ("path") +
		FILE_PATHNAME_DELIMITER + "to";
	}

//	file_is_absolute
++Tests_Total;
passed = file_is_absolute (cwd);
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_is_absolute (" << cwd << ") = " << boolalpha << passed << endl;

string
	basename ("File"),
	extension ("ext"),
	test_filename (basename + FILE_EXTENSION_DELIMITER + extension),
	test_directory (basename + FILE_EXTENSION_DELIMITER + "dir"),
	test_link (basename + FILE_EXTENSION_DELIMITER + "link"),
	pathname,
	path;

//	file_is_absolute
++Tests_Total;
passed = ! file_is_absolute (test_filename);
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_is_absolute (" << test_filename << ") = " << (! passed) << endl;

//	file_exists; false
++Tests_Total;
unlink (test_filename.c_str ());
passed = ! file_exists (test_filename);
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_exists (" << test_filename << ") for non-existant file = "
	 << (! passed) << endl;

ofstream
	test_file (test_filename.c_str ());
test_file << test_filename;
test_file.close ();
if (test_file)
	{
	//	file_exists; true
	++Tests_Total;
	passed = file_exists (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_exists (" << test_filename << ") for existing file = "
		 << passed << endl;

	//	file_is_readable
	++Tests_Total;
	passed = file_is_readable (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_readable (" << test_filename << ") = "
		 << passed << endl;

	//	file_is_writeable
	++Tests_Total;
	passed = file_is_writeable (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_writeable (" << test_filename << ") = "
		 << passed << endl;

	//	file_is_normal
	++Tests_Total;
	passed = file_is_normal (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_normal (" << test_filename << ") = "
		 << passed << endl;

	//	file_is_directory; false
	++Tests_Total;
	passed = ! file_is_directory (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_directory (" << test_filename << ") = "
		 << (! passed) << endl;

	//	file_is_link; false
	++Tests_Total;
	passed = ! file_is_link (test_filename);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_link (" << test_filename << ") = "
		 << (! passed) << endl;

	//	file_size
	++Tests_Total;
	long
		size = file_size (test_filename);
	passed = (size == test_filename.size ());
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_size (" << test_filename << ") = "
		 << size << endl;
	}
else
	{
	cout << "!!! Unable to create a test file." << endl;
	goto Done;
	}

#ifdef _WIN32
RemoveDirectory (test_directory.c_str ());
if (!CreateDirectory (test_directory.c_str (), NULL))
	cout << "!!! Unable to create a test directory. (" << test_directory.c_str () << ")" << endl;

#else

rmdir (test_directory.c_str ());
if (mkdir (test_directory.c_str (), S_IRWXU))
	cout << "!!! Unable to create a test directory." << endl;
#endif

else
	{
	//	file_exists; true
	++Tests_Total;
	passed = file_exists (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_exists (" << test_directory << ") for existing directory = "
		 << passed << endl;

	//	file_is_readable
	++Tests_Total;
	passed = file_is_readable (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_readable (" << test_directory << ") = "
		 << passed << endl;

	//	file_is_writeable
	++Tests_Total;
	passed = file_is_writeable (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_writeable (" << test_directory << ") = "
		 << passed << endl;

	//	file_is_normal; false
	++Tests_Total;
	passed = ! file_is_normal (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_normal (" << test_directory << ") = "
		 << (! passed) << endl;

	//	file_is_directory; true
	++Tests_Total;
	passed = file_is_directory (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_directory (" << test_directory << ") = "
		 << passed << endl;

	//	file_is_link; false
	++Tests_Total;
	passed = ! file_is_link (test_directory);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_is_link (" << test_directory << ") = "
		 << (! passed) << endl;

	//	file_size
	++Tests_Total;
	long
		size = file_size (test_directory);
	passed = (size > 0);
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_size (" << test_directory << ") = "
		 << size << endl;
	}

//	Link file.

unlink (test_link.c_str ());

#ifdef _WIN32
char
	temp_string[MAX_PATH] = {0};
if (LOBYTE (LOWORD (GetVersion ())) < 6)
	{
	//	OS Version < Vista.
	test_links = false;
	cout << "!!! Links only supported on Windows Vista or higher." << endl;
	}
else
	{
	wsprintf (temp_string,
		"/C mklink %s %s", test_link.c_str (), test_directory.c_str ());
	long
		status = (long)ShellExecute
			(NULL, NULL, "CMD.EXE", temp_string, NULL, SW_HIDE);
	/*
		>>> WARNING <<< Testing revealed that an immediate check for the
		successful excecution of the command would fail; FlushFileBuffers
		was ineffective. Thus the delay to let the dust settle. Sigh.
	*/
	Sleep (500);
	passed = (status > 32);
	}

#else
passed = (symlink (test_directory.c_str (), test_link.c_str ()) == 0);
#endif
if (passed)
	{
	if (test_links)
		{
		//	file_exists; true
		++Tests_Total;
		passed = file_exists (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_exists (" << test_link << ") = "
			 << passed << endl;

		//	file_is_readable
		++Tests_Total;
		passed = file_is_readable (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_is_readable (" << test_link << ") = "
			 << passed << endl;

		//	file_is_writeable
		++Tests_Total;
		passed = file_is_writeable (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_is_writeable (" << test_link << ") = "
			 << passed << endl;

		//	file_is_normal; false
		++Tests_Total;
		passed = ! file_is_normal (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_is_normal (" << test_link << ") = "
			 << (! passed) << endl;

		//	file_is_directory; true
		++Tests_Total;
		passed = file_is_directory (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_is_directory (" << test_link << ") = "
			 << passed << endl;

		//	file_is_link; true
		++Tests_Total;
		passed = file_is_link (test_link);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_is_link (" << test_link << ") = "
			 << passed << endl;

		//	file_size
		++Tests_Total;
		long
			size = file_size (test_directory);
		passed = (size > 0);
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_size (" << test_directory << ") = "
			 << size << endl;
		}
	}
else
	cout << "!!! Unable to create a " << test_link << " test link to the "
			<< test_directory << " directory." << endl;

if (test_links)
	{
	unlink (test_link.c_str ());
	#ifdef _WIN32
	wsprintf (temp_string,
		"/C mklink %s %s", test_link.c_str (), test_filename.c_str ());
	long
		status = (long)ShellExecute
			(NULL, NULL, "CMD.EXE", temp_string, NULL, SW_HIDE);
	/*
		>>> WARNING <<< Testing revealed that an immediate check for the
		successful excecution of the command would fail; FlushFileBuffers
		was ineffective. Thus the delay to let the dust settle. Sigh.
	*/
	Sleep (500);
	passed = (status > 32);

	#else
	passed = (symlink (test_filename.c_str (), test_link.c_str ()) == 0);
	#endif
	if (passed)
		{
		//	file_size
		++Tests_Total;
		long
			size = file_size (test_link);
		passed = (size == test_filename.size ());
		if (passed)
			++Tests_Passed;
		cout << (passed ? "PASS: " : "FAIL: ")
			 << "file_size (" << test_link << ") = "
			 << size << endl;
		}
	else
		cout << "!!! Unable to create a " <<  test_link
				<< "test link to the " << test_filename << " file." << endl;
	}

//	file_pathname
++Tests_Total;
path = string ("path") + FILE_PATHNAME_DELIMITER + "to";
name =
	cwd +
	FILE_PATHNAME_DELIMITER + path +
	FILE_PATHNAME_DELIMITER + test_filename;
try
	{
	pathname = file_pathname (path, test_filename);
	passed = name == pathname;
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_pathname (" << path << ", " << test_filename << ") = \""
		 << pathname << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from file_pathname ("
			<< path << ", " << test_filename << ")" << endl
		 << except.what () << endl;
	}

//	file_pathname
++Tests_Total;
name = cwd + FILE_PATHNAME_DELIMITER + test_filename;
try
	{
	pathname = file_pathname (cwd, test_filename);
	passed = name == pathname;
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_pathname (" << cwd << ", " << test_filename << ") = \""
		 << pathname << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from file_pathname ("
			<< cwd << ", " << test_filename << ")" << endl
		 << except.what () << endl;
	}

//	file_pathname
++Tests_Total;
name = cwd + FILE_PATHNAME_DELIMITER + test_filename;
try
	{
	pathname = file_pathname (test_filename);
	passed = name == pathname;
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_pathname (" << test_filename << ") = \""
		 << pathname << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from file_pathname ("
			<< test_filename << ")" << endl
		 << except.what () << endl;
	pathname = name;
	}

//	file_directory; empty
++Tests_Total;
pathname = file_directory (test_filename);
passed = pathname.empty ();
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_directory (" << test_filename << ") = \""
	 << pathname << '"' << endl;

//	file_directory; non-empty
++Tests_Total;
path = string ("path") + FILE_PATHNAME_DELIMITER + "to";
pathname = path + FILE_PATHNAME_DELIMITER + test_filename;
name = file_directory (pathname);
passed = name == path;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_directory (" << pathname << ") = \"" << name << '"' << endl;

//	file_directory; root only
++Tests_Total;
path = FILE_PATHNAME_DELIMITER;
pathname = path + test_filename;
name = file_directory (pathname);
passed = name == path;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_directory (" << pathname << ") = \"" << name << '"' << endl;

//	file_directory_pathname
++Tests_Total;
try
	{
	name = file_directory_pathname (test_filename);
	passed = name == cwd;
	if (passed)
		++Tests_Passed;
	cout << (passed ? "PASS: " : "FAIL: ")
		 << "file_directory_pathname (" << test_filename << ") = \""
		 << name << '"' << endl;
	}
catch (runtime_error except)
	{
	cout << "!!! Exception from file_directory_pathname ("
			<< test_filename << ")" << endl
		 << except.what () << endl;
	}

//	file_name
++Tests_Total;
pathname = cwd + FILE_PATHNAME_DELIMITER + test_filename;
name = file_name (pathname);
passed = name == test_filename;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_name (" << pathname << ") = \"" << name << '"' << endl;

//	file_basename
++Tests_Total;
name = file_basename (test_filename);
passed = name == basename;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_basename (" << test_filename << ") = \"" << name << '"' << endl;

//	file_extension
++Tests_Total;
name = file_extension (test_filename);
passed = name == extension;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "file_extension (" << test_filename << ") = \"" << name << '"' << endl;

//	clean_pathname
++Tests_Total;
name =
	FILE_PATHNAME_DELIMITER + string ("path") +
	FILE_PATHNAME_DELIMITER + "to" +
	FILE_PATHNAME_DELIMITER + "filename";
pathname =
	FILE_PATHNAME_DELIMITER;
pathname +=
	FILE_PATHNAME_DELIMITER + string ("path") +
	FILE_PATHNAME_DELIMITER + '.' +
	FILE_PATHNAME_DELIMITER + "to" +
	FILE_PATHNAME_DELIMITER + "filename";
basename = pathname;
clean_pathname (pathname);
passed = name == pathname;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "clean_pathname (" << basename << ") = " << pathname << endl;

++Tests_Total;
pathname += FILE_PATHNAME_DELIMITER;
basename = pathname;
clean_pathname (pathname);
passed = name == pathname;
if (passed)
	++Tests_Passed;
cout << (passed ? "PASS: " : "FAIL: ")
	 << "clean_pathname (" << basename << ") = " << pathname << endl;


Done:
cout << endl
	 << "Checks: " << Tests_Total << endl
	 << "Passed: " << Tests_Passed << endl;
exit (Tests_Total - Tests_Passed);
}
