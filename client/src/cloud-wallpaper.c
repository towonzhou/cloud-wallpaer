/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cloud-wallpaper.c
 * Copyright (C) 2012 lvlisong <lvlisong@redflag-linux>
 *
 * cloud-wallpaper is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cloud-wallpaper is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "cloud-wallpaper.h"

#include <glib/gi18n.h>
#include <cw-window.h>
#include <gtk/gtk.h>
#include "cw-store.h"




G_DEFINE_TYPE (Cloudwallpaper, cloud_wallpaper, GTK_TYPE_APPLICATION);


/* Define the private structure in the .c file */
#define CLOUD_WALLPAPER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CLOUD_WALLPAPER_TYPE_APPLICATION, CloudwallpaperPrivate))

struct _CloudwallpaperPrivate
{
	GtkWidget* main_window;
};

/* Create a new window loading a file */
static void
cloud_wallpaper_new_window (GApplication *app,
                           GFile        *file)
{
	GtkWidget *window;

	window = cw_window_new ();

	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL)
	{
		/* TODO: Add code here to open the file in the new window */
	}

	CloudwallpaperPrivate* priv = CLOUD_WALLPAPER_GET_PRIVATE(app);
	priv->main_window = window;

}


/* GApplication implementation */
static void
cloud_wallpaper_activate (GApplication *application)
{
	cloud_wallpaper_new_window (application, NULL);

	CWStore* store = cw_store_new();
	cw_store_load_from_directory(store, ".");
	cloud_wallpaper_set_store(application, store);

}

static void
cloud_wallpaper_open (GApplication  *application,
                     GFile        **files,
                     gint           n_files,
                     const gchar   *hint)
{
	gint i;

	for (i = 0; i < n_files; i++)
		cloud_wallpaper_new_window (application, files[i]);
}

static void
cloud_wallpaper_init (Cloudwallpaper *object)
{
	CloudwallpaperPrivate* priv = CLOUD_WALLPAPER_GET_PRIVATE(object);
	priv->main_window = NULL;
}

static void
cloud_wallpaper_finalize (GObject *object)
{
	G_OBJECT_CLASS (cloud_wallpaper_parent_class)->finalize (object);
}

static void
cloud_wallpaper_class_init (CloudwallpaperClass *klass)
{
	G_APPLICATION_CLASS (klass)->activate = cloud_wallpaper_activate;
	G_APPLICATION_CLASS (klass)->open = cloud_wallpaper_open;

	g_type_class_add_private (klass, sizeof (CloudwallpaperPrivate));

	G_OBJECT_CLASS (klass)->finalize = cloud_wallpaper_finalize;
}

Cloudwallpaper *
cloud_wallpaper_new (void)
{
	g_type_init ();

	return g_object_new (cloud_wallpaper_get_type (),
	                     "application-id", "org.gnome.cloud_wallpaper",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}

CWStore* cloud_wallpaper_set_store(GApplication* self, CWStore* store)
{
	CloudwallpaperPrivate* priv = CLOUD_WALLPAPER_GET_PRIVATE(self);
	if (NULL == priv || NULL == priv->main_window)
		return NULL;
	return cw_window_set_store(priv->main_window, store);
}

CWStore* cloud_wallpaper_get_store(GApplication* self)
{
	CloudwallpaperPrivate* priv = CLOUD_WALLPAPER_GET_PRIVATE(self);
	if (NULL == priv || NULL == priv->main_window)
		return NULL;
	return cw_window_get_store(priv->main_window);
}

