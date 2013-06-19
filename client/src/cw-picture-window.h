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

#ifndef _CW_PICTURE_WINDOW_H_
#define _CW_PICTURE_WINDOW_H_

#include <glib-object.h>
#include <gtk/gtk.h>
#include "cw-store-item.h"

G_BEGIN_DECLS

#define CW_TYPE_PICTURE_WINDOW             (cw_picture_window_get_type ())
#define CW_PICTURE_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CW_TYPE_PICTURE_WINDOW, CWPictureWindow))
#define CW_PICTURE_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CW_TYPE_PICTURE_WINDOW, CWPictureWindowClass))
#define CW_IS_PICTURE_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CW_TYPE_PICTURE_WINDOW))
#define CW_IS_PICTURE_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CW_TYPE_PICTURE_WINDOW))
#define CW_PICTURE_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CW_TYPE_PICTURE_WINDOW, CWPictureWindowClass))

typedef struct _CWPictureWindowClass CWPictureWindowClass;
typedef struct _CWPictureWindow CWPictureWindow;
typedef struct _CWPictureWindowPrivate CWPictureWindowPrivate;



struct _CWPictureWindowClass
{
	GtkDrawingAreaClass parent_class;
	/* Signals */
	void(* picture_switch) (CWPictureWindow *self, glong way);
};

struct _CWPictureWindow
{
	GtkDrawingArea parent_instance;


};

GType cw_picture_window_get_type (void) G_GNUC_CONST;
GtkWidget* cw_picture_window_new();

gboolean cw_picture_window_draw(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data);

void cw_picture_window_set_store_item(
		GtkWidget* window,
		CWStoreItem* item);

gboolean cw_picture_window_state_event(
		GtkWidget *widget,
		GdkEventWindowState *event,
		gpointer  user_data);

gboolean cw_picture_window_button_press_event(
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer func_data);



G_END_DECLS

#endif /* _CW_PICTURE_WINDOW_H_ */

