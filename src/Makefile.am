noinst_HEADERS= ContinuumApp.h continuum.h
 
bin_PROGRAMS=continuum
continuum_CPPFLAGS= -I$(top_srcdir)
continuum_SOURCES= ContinuumApp.cpp continuum.cpp
continuum_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS)
continuum_LDADD= $(OGRE_LIBS) $(OIS_LIBS)

EXTRA_DIST = autogen.sh
AUTOMAKE_OPTIONS = foreign
