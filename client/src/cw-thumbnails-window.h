/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gtk-foobar
 * Copyright (C) 2013 lvlisong <lvlisong@redflag-linux.com>
 *
 * gtk-foobar is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gtk-foobar is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CW_THUMBNAILS_WINDOW_H_
#define _CW_THUMBNAILS_WINDOW_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CW_TYPE_THUMBNAILS_WINDOW             (cw_thumbnails_window_get_type ())
#define CW_THUMBNAILS_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CW_TYPE_THUMBNAILS_WINDOW, CWThumbnailsWindow))
#define CW_THUMBNAILS_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CW_TYPE_THUMBNAILS_WINDOW, CWThumbnailsWindowClass))
#define CW_IS_THUMBNAILS_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CW_TYPE_THUMBNAILS_WINDOW))
#define CW_IS_THUMBNAILS_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CW_TYPE_THUMBNAILS_WINDOW))
#define CW_THUMBNAILS_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CW_TYPE_THUMBNAILS_WINDOW, CWThumbnailsWindowClass))

typedef struct _CWThumbnailsWindowClass CWThumbnailsWindowClass;
typedef struct _CWThumbnailsWindow CWThumbnailsWindow;
typedef struct _CWThumbnailsWindowPrivate CWThumbnailsWindowPrivate;

typedef struct _CWStore CWStore;

enum {FULL_WINDOW_MODE, MINIMIZE_WINDOW_MODE};

struct _CWThumbnailsWindowClass
{
	GtkDrawingAreaClass parent_class;
	/* Signals */
	void(* active_large_image) (CWThumbnailsWindow *self, gpointer item);
};

struct _CWThumbnailsWindow
{
	GtkDrawingArea parent_instance;


};

GType cw_thumbnails_window_get_type (void) G_GNUC_CONST;

GtkWidget* cw_thumbnails_window_new();
gboolean cw_thumbnails_window_draw(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data);

gboolean cw_thumbnails_window_motion_notify_event(
		GtkWidget *widget,
		GdkEventMotion  *event,
		gpointer   user_data);

gboolean cw_thumbnails_window_button_press_event(
		GtkWidget *widget,
		GdkEventButton  *event,
		gpointer   user_data);

gboolean cw_thumbnails_window_button_release_event(
		GtkWidget *widget,
		GdkEventButton  *event,
		gpointer   user_data);

gboolean cw_thumbnails_window_scroll_event(
		GtkWidget *widget,
		GdkEventScroll  *event,
		gpointer   user_data);

void cw_thumbnails_window_size_allocate (GtkWidget
		*widget,
		GdkRectangle *allocation,
		gpointer user_data);

CWStore* cw_thumbnails_window_set_store(GtkWidget* window, CWStore* store);
CWStore* cw_thumbnails_window_get_store(GtkWidget* window);
void	 cw_thumbnails_window_set_mode(GtkWidget* window, gint mode);
gint cw_thumbnails_window_get_mode(GtkWidget* window);

void cw_thumbnails_window_on_picture_switch(
		GtkWidget* widget,
		glong way,
		gpointer user_data);

void cw_thumbnails_window_show_image(GtkWidget* widget);
void cw_thumbnails_window_select_image(GtkWidget* widget, int way);

G_END_DECLS

#endif /* _CW_THUMBNAILS_WINDOW_H_ */

