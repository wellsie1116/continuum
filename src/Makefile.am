noinst_HEADERS= \
				vect.h \
				PlayerController.h \
				DotSceneLoader.h \
				ContinuumSceneLoader.h \
				ContinuumApp.h \
				continuum.h \
				PhysicsObject.h \
				Box.h \
				Player.h \
				PhysicsWorld.h \
				PhysicsState.h \
				TickTimer.h \
				World.cpp


SOURCES= \
		 vect.cpp \
		 PlayerController.cpp \
		 DotSceneLoader.cpp \
		 ContinuumSceneLoader.cpp \
		 TickTimer.cpp \
		 PhysicsObject.cpp \
		 Box.cpp \
		 Player.cpp \
		 World.cpp \
		 PhysicsWorld.cpp \
		 PhysicsState.cpp \
		 ContinuumApp.cpp \
		 continuum.cpp

 
bin_PROGRAMS=continuum
continuum_CPPFLAGS= -I$(top_srcdir) -I/usr/local/include/tinyxml
continuum_SOURCES= $(SOURCES)
continuum_CXXFLAGS= $(GLIB_CFLAGS) $(OGRE_CFLAGS) $(OIS_CFLAGS) $(ODE_CFLAGS)
continuum_LDADD= $(GLIB_LIBS) $(OGRE_LIBS) $(OIS_LIBS) $(ODE_LIBS) -ltinyxml

EXTRA_DIST = autogen.sh
AUTOMAKE_OPTIONS = foreign
