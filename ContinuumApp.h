#include <glib-object.h>

G_BEGIN_DECLS

#define CONTINUUM_APP_ERROR continuum_app_error_quark()

#define CONTINUUM_TYPE_APP				(continuum_app_get_type())
#define CONTINUUM_APP(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), CONTINUUM_TYPE_APP, ContinuumApp))
#define CONTINUUM_APP_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), CONTINUUM_TYPE_APP, ContinuumAppClass))
#define CONTINUUM_IS_APP(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), CONTINUUM_TYPE_APP))
#define CONTINUUM_IS_APP_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), CONTINUUM_TYPE_APP))
#define CONTINUUM_APP_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), CONTINUUM_TYPE_APP, ContinuumAppClass))

typedef struct _ContinuumApp			ContinuumApp;
typedef struct _ContinuumAppClass		ContinuumAppClass;

struct _ContinuumApp
{
	GObject parent;
};

struct _ContinuumAppClass
{
	GObjectClass parent_class;
	
	/* signals */

	/* virtual funcs */
	void (*render_ui)(ContinuumApp* self);
};

GType			continuum_app_get_type		() G_GNUC_CONST;

/**
 * continuum_app_get_instance
 *
 * Retrieves the single instance of the running app
 *
 * Returns: (transfer none): The instance of App
 */
ContinuumApp*	continuum_app_get_instance	();

void			continuum_app_render_ui		(ContinuumApp* self);

/**
 * continuum_app_run:
 * @self: The this ptr
 *
 * Runs the application
 *
 * Returns: The exit code for the application
 */
int				continuum_app_run			(ContinuumApp* self);

G_END_DECLS
