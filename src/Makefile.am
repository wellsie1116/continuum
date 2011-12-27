noinst_HEADERS= DotSceneLoader.h ContinuumSceneLoader.h ContinuumApp.h continuum.h Box.h PhysicsWorld.h TickTimer.h
 
bin_PROGRAMS=continuum
continuum_CPPFLAGS= -I$(top_srcdir) -I/usr/local/include/tinyxml
continuum_SOURCES= DotSceneLoader.cpp ContinuumSceneLoader.cpp TickTimer.cpp Box.cpp PhysicsWorld.cpp ContinuumApp.cpp continuum.cpp
continuum_CXXFLAGS= $(GLIB_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) $(ODE_CFLAGS)
continuum_LDADD= $(GLIB_LIBS) $(OGRE_LIBS) $(OIS_LIBS) $(ODE_LIBS) -ltinyxml

EXTRA_DIST = autogen.sh
AUTOMAKE_OPTIONS = foreign
