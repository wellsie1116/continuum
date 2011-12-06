
#include "ContinuumApp.h"

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

ContinuumApp*
continuum_app_new()
{
	return (ContinuumApp*)g_object_new(CONTINUUM_TYPE_APP, NULL);
}

void
continuum_app_run(ContinuumApp* self)
{
	printf("In Main app function\n");
}

/*********************
 * Private Functions *
 *********************/

