/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cloud-wallpaper.h
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

#ifndef _CLOUD_WALLPAPER_
#define _CLOUD_WALLPAPER_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CLOUD_WALLPAPER_TYPE_APPLICATION             (cloud_wallpaper_get_type ())
#define CLOUD_WALLPAPER_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLOUD_WALLPAPER_TYPE_APPLICATION, Cloudwallpaper))
#define CLOUD_WALLPAPER_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CLOUD_WALLPAPER_TYPE_APPLICATION, CloudwallpaperClass))
#define CLOUD_WALLPAPER_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLOUD_WALLPAPER_TYPE_APPLICATION))
#define CLOUD_WALLPAPER_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CLOUD_WALLPAPER_TYPE_APPLICATION))
#define CLOUD_WALLPAPER_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CLOUD_WALLPAPER_TYPE_APPLICATION, CloudwallpaperClass))

typedef struct _CloudwallpaperClass CloudwallpaperClass;
typedef struct _Cloudwallpaper Cloudwallpaper;
typedef struct _CloudwallpaperPrivate CloudwallpaperPrivate;

typedef struct _CWStore CWStore;

struct _CloudwallpaperClass
{
	GtkApplicationClass parent_class;
};

struct _Cloudwallpaper
{
	GtkApplication parent_instance;

	CloudwallpaperPrivate *priv;
};

GType cloud_wallpaper_get_type (void) G_GNUC_CONST;
Cloudwallpaper *cloud_wallpaper_new (void);
CWStore* cloud_wallpaper_set_store(GApplication* self, CWStore* store);
CWStore* cloud_wallpaper_get_store(GApplication* self);


/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */

