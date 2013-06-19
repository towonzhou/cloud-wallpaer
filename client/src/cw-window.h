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

#ifndef _CW_WINDOW_H_
#define _CW_WINDOW_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CW_TYPE_WINDOW             (cw_window_get_type ())
#define CW_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CW_TYPE_WINDOW, CWWindow))
#define CW_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CW_TYPE_WINDOW, CWWindowClass))
#define CW_IS_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CW_TYPE_WINDOW))
#define CW_IS_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CW_TYPE_WINDOW))
#define CW_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CW_TYPE_WINDOW, CWWindowClass))

typedef struct _CWWindowClass CWWindowClass;
typedef struct _CWWindow CWWindow;
typedef struct _CWWindowPrivate CWWindowPrivate;

typedef struct _CWStore CWStore;


struct _CWWindowClass
{
	GtkWindowClass parent_class;
};

struct _CWWindow
{
	GtkWindow parent_instance;


};

GType cw_window_get_type (void) G_GNUC_CONST;

GtkWidget* cw_window_new();
gboolean cw_window_draw(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data);

CWStore* cw_window_set_store(GtkWidget* window, CWStore* store);
CWStore* cw_window_get_store(GtkWidget* window);

void cw_window_active_large_image(
		GtkWidget* widget, gpointer item, gpointer user_data);

gboolean cw_window_key_release_event(
		GtkWidget *widget,
		GdkEventKey   *event,
		gpointer   user_data);

G_END_DECLS

#endif /* _CW_WINDOW_H_ */

