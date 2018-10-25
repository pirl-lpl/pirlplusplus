#	PIRL C++ utilities
#
#	gmake syntax
#
#	CVS ID: $Id: Makefile,v 1.20 2011/07/04 23:07:09 castalia Exp $

TITLE			:=	PIRL C++ Utilities
R := R
CVS_REVISION	:=	$Revision: 1.20 $

PROJECT_ROOT	=	../..


#	Host system architecture identification.
empty :=
space := $(empty) $(empty)
MACHINE			:=	$(subst $(space),_,$(shell uname -m))
PLATFORM		:=	$(subst $(space),_,$(shell uname -p))
OS				:=  $(subst $(space),_,$(shell uname -s))
ifeq ($(PLATFORM),unknown)
PLATFORM = $(MACHINE)
endif
ifneq (,$(findstring CYGWIN, $(OS)))
OS := WIN
endif
ifeq ($(OS),Darwin)
ifeq ($(MACHINE),i386)
#	Define the following symbol to build Apple Mac fat (universal) binaries.
#
#	On a MACHINE identified as i386 the specified architectures will be
#	included. N.B. Systems that support 64-bit x86_64 architectures report
#	themselves as i386.
#
#	If this symbol is not defined only the host system architecture will be
#		supported by the libraries that are built.
FAT_MAC				?=	-arch x86_64 \
						-arch i386 \
						-arch ppc
endif
endif


#	idaeim Utility/Checker capabilities.
IDAEIM_ROOT			?=	/opt/idaeim
ifeq ($(OS),WIN)
idaeim_INCLUDE		:=	
idaeim_LIBRARY		:=	
else
$(warning Including ideaim library)
idaeim_INCLUDE		:=	-I$(IDAEIM_ROOT)/include
idaeim_LIBRARY		:=	-L$(IDAEIM_ROOT)/lib \
							-lidaeim$(64)
endif

#	Programs:

PROGRAMS			=	endian_test \
						Binary_IO_test \
						Data_Block_test \
						Reference_Counted_Pointer_test \
						Files_test 
					
#	Dimensions test not built on Windows; needs idaeim library.
ifneq ($(strip $(idaeim_INCLUDE)),)
PROGRAMS			+=	Dimensions_test
endif

ifeq ($(OS),WIN)
PROGRAMS			:= 	$(PROGRAMS:%=%.exe)
endif

#	Include files:

INCLUDE_DIR			:=	..

INCLUDES			:=	-I$(INCLUDE_DIR) \
						$(idaeim_INCLUDE)


#	Libraries:
ifeq ($(OS),WIN)
MODULE_LIBRARY		=	../libPIRL++.lib
else
MODULE_LIBRARY		=	../libPIRL++$(64).a
endif

LIBRARIES			+=	$(MODULE_LIBRARY) \
						$(idaeim_LIBRARY)

ifeq ($(OS),WIN)
LIBRARIES			+=	LIBCMT.LIB \
						USER32.LIB \
						ADVAPI32.LIB \
						SHELL32.LIB \
						WSOCK32.LIB \
						USERENV.LIB
endif


#	Compiler flags:

ifeq ($(OS),WIN)
COMPILER				?=	cl.exe
else
COMPILER				?=	g++
endif
CC						:=	$(COMPILER)
CXX						:=	$(COMPILER)

#	Preprocessor flags:

ifeq ($(OS),WIN)
CPPFLAGS				+=	/nologo /W2 /D "_WIN32" /EHsc /MT
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

CXXFLAGS__64			:=	-mcpu=v9 -m64
CXXFLAGS				+=	$(OPTIMIZATION) $(CXXFLAGS_$(64)) $(FAT_MAC)


#	Linker flags:

### Built-in
LDFLAGS__64				:=	-mcpu=v9 -m64
LDFLAGS					+=	$(LDFLAGS_$(64)) $(FAT_MAC)

### Built-in
LDLIBS					+=	$(LIBRARIES)


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

ALL:			.BEGIN all .END
all:			programs

ifdef 64_BIT
64_bit:
	@$(MAKE) --no-print-directory 64=_64
endif

programs:		$(PROGRAMS)
debug:			programs

ifeq ($(OS), WIN)
$(PROGRAMS):	%.exe: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< $(LIBRARIES) /Fe$@
else
$(PROGRAMS):	%: %.o
endif

check:			programs
	$(foreach program, $(PROGRAMS), $(program); echo;)


$(MODULE_LIBRARY):
	(cd ..; $(MAKE) library)

clean:
	rm -f $(PROGRAMS) *.o *.test *.core *.d *.obj

#	Report targets:

.BEGIN:
	@echo
	@echo "*** $(TITLE)"
	@echo "    OS $(OS), platform $(PLATFORM)"
	@echo "    Makefile $(CVS_REVISION)"
	@date

.END:
	@echo
	@echo "*** $(TITLE) Completed ***"
	@date
	@echo

ifneq ($(OS), WIN)
#	Project Makefile configuration.
include		../Makefile.conf
endif
