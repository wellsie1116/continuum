noinst_HEADERS= ContinuumApp.h continuum.h Box.h PhysicsWorld.h TickTimer.h
 
bin_PROGRAMS=continuum
continuum_CPPFLAGS= -I$(top_srcdir)
continuum_SOURCES= TickTimer.cpp Box.cpp PhysicsWorld.cpp ContinuumApp.cpp continuum.cpp
continuum_CXXFLAGS= $(GLIB_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS)
continuum_LDADD= $(GLIB_LIBS) $(OGRE_LIBS) $(OIS_LIBS) -lNewton

EXTRA_DIST = autogen.sh
AUTOMAKE_OPTIONS = foreign
