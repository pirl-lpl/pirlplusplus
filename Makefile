#	PIRL C++ Class Library
#
#	gmake syntax
#
#	CVS ID: $Id: Makefile,v 2.10 2012/08/23 04:57:49 castalia Exp $

TITLE					:=	PIRL++: C++ Class Library
R := R
CVS_REVISION			:=	$Revision: 2.10 $

VERSION					:=	$(shell grep '^[0-9]' VERSION)

INSTALL_DIR				?=	.
DOCS_DIR				?=	$(INSTALL_DIR)

DIR_PERMISSIONS			?=	a=rx,ug+w,g+s
SOURCE_PERMISSIONS		?=	a=r,ug+w
BIN_PERMISSIONS			?=	a=rx,ug+w

#	Host system architecture identification.
empty :=
space := $(empty) $(empty)
MACHINE					:=	$(subst $(space),_,$(shell uname -m))
PLATFORM				:=	$(subst $(space),_,$(shell uname -p))
OS						:=  $(subst $(space),_,$(shell uname -s))
KERNEL_RELEASE			:=	$(subst .,$(space),$(shell uname -r))
ifeq ($(PLATFORM),unknown)
PLATFORM = $(MACHINE)
endif
ifneq (,$(findstring CYGWIN, $(OS)))
OS := WIN
endif

#===============================================================================
#	Apple Mac (Darwin) "fat Mac" (universal) binaries.
#
#	On a Darwin OS that is not a Power_Macintosh MACHINE the FAT_MAC
#	architectures will be included in the fat Mac binaries. If FAT_MAC is
#	not defined only the host system architecture will be supported by the
#	binaries that are built.
#
#	N.B. Darwin systems that support 64-bit x86_64 architectures may report
#	themselves as i386; these systems will still build x86_64 binaries
#	unless coerced to use the i386 architecture at compile/link time.
ifeq ($(OS),Darwin)
ifneq ($(MACHINE),Power_Macintosh)
ifeq ($(firstword $(KERNEL_RELEASE)),10)
#	For kernel version 10 with PowerPC (ppc) support.
FAT_MAC			?=	-arch x86_64 \
					-arch i386 \
					-arch ppc
else
#	Other OS on intel without ppc support.
FAT_MAC			?=	-arch x86_64
endif
endif
endif


#	Files definitions.
ifeq ($(OS),WIN)
FILE_PATHNAME_DELIMITER 	?=	\\\\
else
FILE_PATHNAME_DELIMITER 	?= 	/
endif

#	Dynamic/shared library filename extension.
ifeq ($(OS),Darwin)
OBJECT_EXT				:=	o
STATIC_LIBRARY_EXT		:=	a
SHARED_LIBRARY_EXT		:=	dylib
else ifeq ($(OS),WIN)
OBJECT_EXT				:=	obj
STATIC_LIBRARY_EXT		:=	lib
SHARED_LIBRARY_EXT		:= 	dll
else
OBJECT_EXT				:=	o
STATIC_LIBRARY_EXT		:=	a
SHARED_LIBRARY_EXT		:=	so
endif


#	The library:

LIBRARY_NAME			:=	libPIRL++$(64)
SHARED_LIBRARY			:=	$(LIBRARY_NAME).$(SHARED_LIBRARY_EXT)
STATIC_LIBRARY 			:=	$(LIBRARY_NAME).$(STATIC_LIBRARY_EXT)

#	Source files:

SOURCES					:=	endian.cc \
							Binary_IO.cc \
							Data_Block.cc \
							Cache.cc \
							Files.cc \
							Dimensions.cc

OBJECTS					:=	$(SOURCES:%.cc=%.$(OBJECT_EXT)$(64))
ifeq ($(OS),WIN)
SHARED_LIBRARY_OBJECTS	:= 	$(OBJECTS)
else
SHARED_LIBRARY_OBJECTS	:=	$(OBJECTS:%.o$(64)=%.slo$(64))
endif

#	Include files:

HEADERS					:=	$(SOURCES:%.cc=%.hh) \
							Reference_Counted_Pointer.hh

#	>>> WARNING <<< Do not use the token name "INCLUDE"
#	which is reserved by MSVC for specifying its include file search path.
INCLUDES					:=	-I.

#	Libraries:

ifeq ($(OS),WIN)
LIBRARIES				:=	LIBCMT.LIB \
							KERNEL32.LIB \
							USER32.LIB \
							GDI32.LIB \
							ADVAPI32.LIB \
							SHELL32.LIB \
							WSOCK32.LIB \
							USERENV.LIB
endif


#	Compiler flags:

#	Source code compiler.
ifeq ($(OS),WIN)
COMPILER				?=	cl.exe

else
ifeq ($(strip $(OS)),Darwin)
ifneq ($(MACHINE),Power_Macintosh)
#	For kernel version 10 with PowerPC support.
ifeq ($(firstword $(KERNEL_RELEASE)),15)
COMPILER				?=	clang++
else
COMPILER          ?= g++
#CPPFLAGS				+= -stdlib=libstdc++
#LDFLAGS				+= -stdlib=libstdc++
endif
endif
endif
endif

COMPILER				?=	g++
CXX						:=	$(COMPILER)

#	Preprocessor flags:

ifeq ($(OS),WIN)
CPPFLAGS				+=	/D "_WIN32" /EHsc /MT
endif
 
#	DEBUG control
DEBUG					=	(DEBUG_ALL)

		CPPFLAGS		+=	$(INCLUDES)
debug:	CPPFLAGS		+=	-DDEBUG='$(DEBUG)'

ifeq ($(OS),WIN)
#	Optimized compiliation
OPTIMIZED				?=	/O2
#	Compiled with debug symbols
DEBUG_SYMBOLS			?=	/Od 
else
OPTIMIZED				?=	-O
DEBUG_SYMBOLS			?=	-g
endif
		OPTIMIZATION	?=	$(OPTIMIZED)
debug:	OPTIMIZATION	=	$(DEBUG_SYMBOLS)

Files.o:		CPPFLAGS	+=	-DPATHNAME_DELIMITER="'$(FILE_PATHNAME_DELIMITER)'"
Files.obj:		CPPFLAGS	+=	-DPATHNAME_DELIMITER="'$(FILE_PATHNAME_DELIMITER)'"
Files.slo:		CPPFLAGS	+=	-DPATHNAME_DELIMITER="'$(FILE_PATHNAME_DELIMITER)'"
Files.o_64:		CPPFLAGS	+=	-DPATHNAME_DELIMITER="'$(FILE_PATHNAME_DELIMITER)'"
Files.slo_64:	CPPFLAGS	+=	-DPATHNAME_DELIMITER="'$(FILE_PATHNAME_DELIMITER)'"


ifeq ($(OS),WIN)
CXXFLAGS				+=	/W2 /nologo
else
CXXFLAGS				+=	-Wall
CXXFLAGS__64			+=	-Wall
endif
CXXFLAGS__64			:=	-mcpu=v9 -m64
CXXFLAGS				+=	$(OPTIMIZATION) $(CXXFLAGS_$(64)) $(FAT_MAC)
ifneq ($(OS),WIN)
$(SHARED_LIBRARY):	CXXFLAGS	+=	-fPIC
endif

LDFLAGS__64				:=	-mcpu=v9 -m64
LDFLAGS					+=	$(LDFLAGS_$(64)) $(FAT_MAC)
ifeq ($(OS),Darwin)
$(SHARED_LIBRARY):	LDFLAGS		+=	-dynamiclib \
									-single_module \
									-headerpad_max_install_names \
									-current_version $(VERSION)
else ifneq ($(OS),WIN)
$(SHARED_LIBRARY):	LDFLAGS		+=	-shared
endif

#	64-bit sparc platform.
ifeq ($(PLATFORM),sparc)
#		To avoid infinite recursion, don't define if 64 was defined.
ifndef 64
64_BIT		=	_64
endif
else
ifdef 64
$(error The host system is not a sparc platform.)
endif
endif


#	Targets:

all:			.BEGIN \
				.all \
				.END
.all:			.static_library .shared_library
    ifdef 64_BIT
	@echo
	@echo  "+++ 64-bit
	@$(MAKE) --no-print-directory 64=_64 .all
    endif

ifdef 64_BIT
64_bit:
	@$(MAKE) --no-print-directory 64=_64
endif

debug:				.static_library .shared_library

lib:				static_library
static_lib:			static_library
library:			static_library
static_library:		.BEGIN \
					.static_library \
					.END
.static_library:	.BEGIN_STATIC $(STATIC_LIBRARY)
ifeq ($(OS),WIN)
$(STATIC_LIBRARY):	$(OBJECTS)
	lib.exe $(OBJECTS) /OUT:$(STATIC_LIBRARY)
else
$(STATIC_LIBRARY):	$(OBJECTS)
	rm -f $(STATIC_LIBRARY)
	ar qs $(STATIC_LIBRARY) $(OBJECTS)
endif

shared_lib:			.shared_library
shared_library:		.BEGIN \
					.shared_library \
					.END
.shared_library:	.BEGIN_SHARED $(SHARED_LIBRARY)
$(SHARED_LIBRARY):	$(SHARED_LIBRARY_OBJECTS)
    ifeq ($(OS),WIN)
	$(CXX) /LD /nologo $(SHARED_LIBRARY_OBJECTS) $(LIBRARIES) /Fe$(SHARED_LIBRARY)
    else
	$(CXX) $(LDFLAGS) -o $@ $(SHARED_LIBRARY_OBJECTS)
    endif

install:			.BEGIN \
					.BEGIN_INSTALL .install \
					.docs_install \
					.END
.install:			.headers_install .bin_install

headers_install:	.BEGIN \
					.headers_install \
					.END
.headers_install:
	-install -d -m $(DIR_PERMISSIONS) \
		$(INSTALL_DIR)/include
	install -m $(SOURCE_PERMISSIONS) \
		$(HEADERS) $(INSTALL_DIR)/include

bin_install:		.BEGIN \
					.BEGIN_STATIC $(STATIC_LIBRARY) \
					.BEGIN_SHARED $(SHARED_LIBRARY) \
					.BEGIN_INSTALL .bin_install \
					.END
.bin_install:		$(STATIC_LIBRARY) \
					$(SHARED_LIBRARY)
	-install -d -m $(DIR_PERMISSIONS) \
		$(INSTALL_DIR)/lib
	install -c -m $(SOURCE_PERMISSIONS) $(STATIC_LIBRARY) \
		$(INSTALL_DIR)/lib/$(STATIC_LIBRARY)
    ifneq ($(OS),WIN)
	ranlib $(INSTALL_DIR)/lib/$(STATIC_LIBRARY)
    endif
	install -c -m $(SOURCE_PERMISSIONS) $(SHARED_LIBRARY) \
		$(INSTALL_DIR)/lib/$(SHARED_LIBRARY)-$(VERSION)
	-rm -f $(INSTALL_DIR)/lib/$(SHARED_LIBRARY)
	ln -s $(SHARED_LIBRARY)-$(VERSION) \
		$(INSTALL_DIR)/lib/$(SHARED_LIBRARY)
ifdef 64_BIT
	@echo
	@echo  "+++ 64-bit
	@$(MAKE) --no-print-directory 64=_64 .bin_install

64_bit_install:
	@$(MAKE) --no-print-directory 64=_64 bin_install
endif

docs_install:	.BEGIN .docs_install .END
.docs_install:	.docs
	-install -d -m $(DIR_PERMISSIONS) $(DOCS_DIR)
	-cp -r -p docs $(DOCS_DIR)

docs:			.BEGIN .docs .END
.docs:			.BEGIN_DOCS docs/html/PIRL++/index.html
docs/html/PIRL++/index.html:	$(HEADERS) \
								$(SOURCES) \
								Doxyfile \
								Header.doxygen \
								Overview.doxygen
	mkdir -p docs/html/PIRL++
	doxygen Doxyfile
	cp PIRL_icon.gif docs/html/PIRL++
Doxyfile:
	doxygen -g Doxyfile

dist:			DIST_DIR = PIRL++-$(VERSION)
dist:			.BEGIN .dist .END
.dist:
	rm -rf distribution $(DIST_DIR)
	mkdir distribution
	Release \
		-directory distribution \
		-no_make \
		PIRL++
	cd distribution/PIRL++ && $(MAKE) docs clean
	mv distribution/PIRL++ $(DIST_DIR)
	gtar \
		--create --gzip \
		--file=$(DIST_DIR).tar.gz \
		$(DIST_DIR)
	-rm -rf distribution $(DIST_DIR)

clean:			.BEGIN .clean .END
.clean:
	-rm -f \
		$(STATIC_LIBRARY) $(OBJECTS) \
		$(SHARED_LIBRARY) $(SHARED_LIBRARY_OBJECTS)
    ifdef 64_BIT
	@$(MAKE) --no-print-directory 64=_64 .clean
    else
	-rm -f *.core *.d
    endif

#	WARNING: dist_clean will remove the documentation.
dist_clean:		.BEGIN .dist_clean .END
.dist_clean:	.clean
	(cd tests; $(MAKE) clean)
	-rm -rf docs
distclean:		dist_clean

tests:			.BEGIN .tests .END
.tests:
	(cd tests; $(MAKE))

check:			.BEGIN .check .END
.check:
	(cd tests; $(MAKE) check)

.PHONY:			.BEGIN \
				all .all debug \
				.BEGIN_STATIC \
				static_library library static_lib lib .library \
				.BEGIN_SHARED \
				shared_library shared_lib .shared_library \
				.BEGIN_INSTALL \
				install .install \
				bin_install .bin_install \
				docs_install .docs_install \
				.BEGIN_DOCS \
				docs .docs \
				dist .dist \
				clean .clean dist_clean distclean .dist_clean \
				tests .tests check .check\
				.END 

.PRECIOUS:		$(STATIC_LIBRARY) \
				$(SHARED_LIBRARY)

#	Report targets:

.BEGIN:
	@echo
	@echo "*** $(TITLE) version $(VERSION)"
	@echo "    OS $(OS), platform $(PLATFORM)"
	@echo "    Makefile $(CVS_REVISION)"
	@date

.BEGIN_STATIC:
	@echo
	@echo "=== Static library"
	@echo

.BEGIN_SHARED:
	@echo
	@echo "=== Dynamic library"
	@echo

.BEGIN_DOCS:
	@echo
	@echo "=== Documentation"
	@echo

.BEGIN_INSTALL:
	@echo
	@echo "=== Installation"
	@echo

.END:
	@echo
	@echo "*** $(TITLE) Completed ***"
	@date
	@echo

#	Project Makefile configuration.
ifeq ($(OS),WIN)
#	Object file pattern rule.
%.obj:	%.cc $(SOURCES)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS)  /c $< 
else
include			Makefile.conf
endif
