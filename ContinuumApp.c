
#include "ContinuumApp.h"

#include "continuum.h"

#include <stdio.h>

G_DEFINE_TYPE(ContinuumApp, continuum_app, G_TYPE_OBJECT)

typedef struct _ContinuumAppPrivate		ContinuumAppPrivate;

//static guint signals[LAST_SIGNAL] = {0,};

static void continuum_app_finalize(GObject* gobject);

static void
continuum_app_class_init(ContinuumAppClass* klass)
{
	klass->parent_class.finalize = continuum_app_finalize;
}

static void
continuum_app_init(ContinuumApp* self)
{
	/* initialize fields generically here */
}

static void
continuum_app_finalize(GObject* gobject)
{
	//ContinuumApp* self = CONTINUUM_APP(gobject);

	/* Chain up to the parent class */
	G_OBJECT_CLASS(continuum_app_parent_class)->finalize(gobject);
}

static ContinuumApp* instance;

ContinuumApp*
continuum_app_get_instance()
{
	return instance;
}

void
continuum_app_render_ui(ContinuumApp* self)
{
	ContinuumAppClass* klass = CONTINUUM_APP_GET_CLASS(self);
	if (klass->render_ui)
		klass->render_ui(self);
}

int
continuum_app_run(ContinuumApp* self)
{
	if (instance)
	{
		printf("App already running\n");
		return 1;
	}

	instance = self;
	return continuum_main();
}

/*********************
 * Private Functions *
 *********************/

