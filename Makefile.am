SUBDIRS = deps test

lib_LTLIBRARIES = libwarped.la

# These are the headers that will be installed. Only list headers included
# (directly or indirectly) through warped.hpp. Keep all these source listings
# sorted.
WARPED_INSTALL_HPP_FILES = \
	 src/VTime.hpp \
    src/Configuration.hpp \
    src/FileStream.hpp \
    src/Event.hpp \
    src/LogicalProcess.hpp \
    src/LPState.hpp \
    src/Partitioner.hpp \
	 src/RandomNumberGenerator.hpp \
    src/serialization.hpp \
    src/Simulation.hpp \
    src/warped.hpp \
    src/utility/memory.hpp \
    src/utility/warnings.hpp

# All other headers are listed in this variable
WARPED_NOINST_HPP_FILES = \
   src/CommandLineConfiguration.hpp \
   src/EventDispatcher.hpp \
   src/LTSFQueue.hpp \
   src/ProfileGuidedPartitioner.hpp \
   src/RoundRobinPartitioner.hpp \
   src/SequentialEventDispatcher.hpp \
   src/STLLTSFQueue.hpp \
	src/RandomNumberGenerator.hpp \
   src/utility/strings.hpp \
	src/TicketLock.hpp

# All cpp files are listed in this variable
WARPED_CPP_FILES = \
	 src/VTime.cpp \
    src/CommandLineConfiguration.cpp \
    src/Configuration.cpp \
    src/FileStream.cpp \
    src/ProfileGuidedPartitioner.cpp \
    src/RoundRobinPartitioner.cpp \
    src/SequentialEventDispatcher.cpp \
    src/Simulation.cpp \
    src/STLLTSFQueue.cpp \
    src/LogicalProcess.cpp

libwarped_la_SOURCES = \
    $(WARPED_INSTALL_HPP_FILES) \
    $(WARPED_NOINST_HPP_FILES) \
    $(WARPED_CPP_FILES)

include_HEADERS = $(WARPED_INSTALL_HPP_FILES)

libwarped_la_LIBADD = \
    $(top_builddir)/deps/json/libjsoncpp.la \
    $(top_builddir)/deps/metis/libmetis/libmetis.la

libwarped_la_CXXFLAGS = -Wall -Wextra -pedantic -Werror

# Use -isystem here instead of -I to disable warnings for third party code
libwarped_la_CPPFLAGS=-isystem$(top_srcdir)/deps
