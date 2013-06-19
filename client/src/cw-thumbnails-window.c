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

#include "cw-thumbnails-window.h"
#include <gtk/gtk.h>
#include "cw-store.h"
#include "cw-store-item.h"
#include "global.h"
#include "cw-picture-window.h"
//#include <librsvg/rsvg-cairo.h>

enum
{
	ACTIVE_LARGE_IMAGE,

	LAST_SIGNAL
};

static guint thumbnails_window_signals[LAST_SIGNAL] = { 0 };
enum {STATUS_NORMAL, STATUS_HOVER, STATUS_SELECTED};

G_DEFINE_TYPE (CWThumbnailsWindow, cw_thumbnails_window, GTK_TYPE_DRAWING_AREA);

/* Define the private structure in the .c file */
#define CW_THUMBNAILS_WINDOW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CW_TYPE_THUMBNAILS_WINDOW, CWThumbnailsWindowPrivate))

struct _CWThumbnailsWindowPrivate
{
	CWStore*     store;
	gint         current_item;
	gint         minimize_mode_first_item;
	gint         max_row;
	gint		 window_mode;
	gboolean     button_press;
};


static void
cw_thumbnails_window_init (CWThumbnailsWindow *cw_thumbnails_window)
{

	CWThumbnailsWindowPrivate *priv =
		CW_THUMBNAILS_WINDOW_GET_PRIVATE(cw_thumbnails_window);
	priv->store        = NULL;
	priv->current_item = -1;
	priv->minimize_mode_first_item = 0;
	priv->max_row      = -1;
	priv->window_mode  = FULL_WINDOW_MODE;
	priv->button_press = FALSE;

	cw_window_screen_changed(GTK_WIDGET(cw_thumbnails_window), NULL, NULL);

	gtk_widget_add_events(GTK_WIDGET(cw_thumbnails_window),
			GDK_POINTER_MOTION_MASK
			| GDK_KEY_RELEASE_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_SCROLL_MASK
			| GDK_BUTTON_RELEASE_MASK);

	GdkRGBA rgba = { 1, 1, 1, 0.0};
	gtk_widget_override_background_color(GTK_WIDGET(cw_thumbnails_window),
			GTK_STATE_FLAG_NORMAL, &rgba);

	gtk_widget_show_all (GTK_WIDGET (cw_thumbnails_window));
	gtk_widget_shape_combine_region(GTK_WIDGET(cw_thumbnails_window), NULL);

	g_signal_connect(
			cw_thumbnails_window,
			"size-allocate",
			G_CALLBACK(cw_thumbnails_window_size_allocate), NULL);

	/* TODO: Add initialization code here */
}

static void
cw_thumbnails_window_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (cw_thumbnails_window_parent_class)->finalize (object);
}

static void
cw_thumbnails_window_active_large_image (
		CWThumbnailsWindow *self, gpointer item)
{
	/* TODO: Add default signal handler implementation here */
}


static void
cw_thumbnails_window_class_init (CWThumbnailsWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CWThumbnailsWindowPrivate));

	klass->active_large_image = cw_thumbnails_window_active_large_image;
	thumbnails_window_signals[ACTIVE_LARGE_IMAGE] =
		g_signal_new("active-large-image",
				G_OBJECT_CLASS_TYPE(klass),
		        G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		        G_STRUCT_OFFSET (CWThumbnailsWindowClass, active_large_image),
				NULL, NULL,
				g_cclosure_marshal_VOID__POINTER,
				G_TYPE_NONE, 1,
				G_TYPE_POINTER
				);

	g_signal_override_class_handler("screen-changed",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_window_screen_changed));

	g_signal_override_class_handler("draw",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_thumbnails_window_draw));

	g_signal_override_class_handler("motion-notify-event",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_thumbnails_window_motion_notify_event));

	g_signal_override_class_handler("button-press-event",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_thumbnails_window_button_press_event));

	g_signal_override_class_handler("button-release-event",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_thumbnails_window_button_release_event));

	g_signal_override_class_handler("scroll-event",
			cw_thumbnails_window_get_type(),
			G_CALLBACK(cw_thumbnails_window_scroll_event));

	//g_signal_override_class_handler("size-allocate",
			//cw_thumbnails_window_get_type(),
			//G_CALLBACK(cw_thumbnails_window_size_allocate));

	object_class->finalize = cw_thumbnails_window_finalize;
}


CWStore* cw_thumbnails_window_set_store(GtkWidget* window, CWStore* store)
{
	CWThumbnailsWindowPrivate *priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(window);
	if (priv) {
		CWStore* old = priv->store;
		priv->store = store;
		return old;
	}
	return NULL;
}

CWStore* cw_thumbnails_window_get_store(GtkWidget* window)
{
	CWThumbnailsWindowPrivate *priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(window);
	if (priv)
		return priv->store;
	return NULL;
}


static void cw_thumbnails_window_draw_pictures(cairo_t* cr,
		GdkPixbuf* pix,
		GdkRectangle rt,
		gint status)
{
	cairo_save(cr);

	switch (status) {
		case STATUS_SELECTED:
			cairo_translate(cr,
					rt.x += get_thumbnail_shadow_offset_x(),
					rt.y += get_thumbnail_shadow_offset_y());
			break;
		case STATUS_HOVER:
			cairo_translate(cr,
					rt.x -= get_thumbnail_shadow_offset_x(),
					rt.y -= get_thumbnail_shadow_offset_y());
			break;
		default:
			cairo_translate(cr, rt.x, rt.y);
			break;
	}

	cairo_rectangle(cr, 0, 0, rt.width, rt.height);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_fill(cr);


	cairo_translate(cr, get_thumbnail_border_size(), get_thumbnail_border_size());
	gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
	cairo_paint(cr);

	cairo_restore(cr);
}

gboolean cw_thumbnails_window_grid_from_coordinate(
		const CWThumbnailsWindowPrivate* priv,
		GdkRectangle coor,
		gint *x1,
		gint *y1,
		gint *x2,
		gint *y2)
{

	int frame_width = get_thumbnail_frame_width();
	int frame_height = get_thumbnail_frame_height();
	*x1 = coor.x / frame_width;
	*y1 = coor.y / frame_height;

	*x2 = (coor.x + coor.width) / frame_width
		+ ((coor.x + coor.width) % frame_width ? 1 : 0);
	*y2 = (coor.y + coor.height) / frame_height
		+ ((coor.y + coor.height) % frame_height ? 1 : 0);

	if (*y2 > priv->max_row)
		*y2 = priv->max_row;

	return TRUE;
}

gboolean cw_thumbnails_window_draw(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv || NULL == priv->store)
		return FALSE;

	CWStore* store = priv->store;
	CWStoreItem* item;
	GdkPixbuf* pix;
	GdkRectangle rt;
	GdkRectangle shadow_rt;
	GdkRectangle clip;


	int row_top, col_left, row_bottom, col_right;
	gdk_cairo_get_clip_rectangle(cr, &clip);

	g_message("draw, %d, %d", clip.x, clip.y);

	if (!cw_thumbnails_window_grid_from_coordinate(
				priv, clip, &col_left, &row_top, &col_right, &row_bottom))
		return FALSE;

	int row, col;
	for (col = col_left; col < col_right; col++) {
		for (row = row_top; row < row_bottom ; row++) {
			item = cw_store_get_item(store, col * priv->max_row + row);
			if (NULL == item)
				break;

			pix = cw_store_item_get_thumbnail_pixbuf(item);
			get_pix_rect(pix, get_thumbnail_border_size(), &rt);
			site_thumbnail_rect(col, row, &rt);

			gint status = STATUS_NORMAL;
			if (col * priv->max_row + row == priv->current_item) {
				if (priv->button_press)
					status = STATUS_SELECTED;
				else
					status = STATUS_HOVER;
			}

			// draw shadow
			calc_shadow_rect(&rt, &shadow_rt);
			offset_rect(&shadow_rt,
					get_thumbnail_shadow_offset_x(),
					get_thumbnail_shadow_offset_y());
			draw_shadow_rgba(cr,
					shadow_rt,
					get_thumbnail_shadow_radius(),
					SHADOW_TRANSPARENT);

			// draw image
			cw_thumbnails_window_draw_pictures(cr, pix, rt, status);
		}
	}
	return TRUE;
}

GtkWidget* cw_thumbnails_window_new()
{
	g_type_init();
	return g_object_new(cw_thumbnails_window_get_type(), NULL);
}

static void cw_thumbnails_window_get_item_rect(
		CWThumbnailsWindowPrivate* priv,
		gint number,
		GdkRectangle* rt)
{
	int col    = number / priv->max_row;
	int row    = number % priv->max_row;
	rt->x      = col * get_thumbnail_frame_width();
	rt->y      = row * get_thumbnail_frame_height();
	rt->width  = get_thumbnail_frame_width();
	rt->height = get_thumbnail_frame_height();
}

static void cw_thumbnails_window_set_seletct(GtkWidget* widget,
		CWThumbnailsWindowPrivate* priv,
		gint number,
		gboolean force)
{
	GdkRectangle rt;
	if (number < 0)
		number = -1;

	if (priv->current_item == number) {
		if (force) {
			cw_thumbnails_window_get_item_rect(priv, number, &rt);
			gdk_window_invalidate_rect(gtk_widget_get_window(widget), &rt, FALSE);
		}
		return;
	}

	cw_thumbnails_window_get_item_rect(priv, priv->current_item, &rt);
	gdk_window_invalidate_rect(gtk_widget_get_window(widget), &rt, FALSE);
	cw_thumbnails_window_get_item_rect(priv, number, &rt);
	gdk_window_invalidate_rect(gtk_widget_get_window(widget), &rt, FALSE);

	priv->current_item = number;
}

static gboolean cw_thumbnails_window_select_from_coordinate(
		GtkWidget* widget,
		CWThumbnailsWindowPrivate* priv,
		gint x,
		gint y,
		gboolean force)
{
	gint col, row, number;
	col = x / get_thumbnail_frame_width();
	row = y / get_thumbnail_frame_height();

	number = col * priv->max_row + row;

	cw_thumbnails_window_set_seletct(widget, priv, number, force);
	return TRUE;
}

gboolean cw_thumbnails_window_motion_notify_event(GtkWidget *widget,
		GdkEventMotion  *event,
		gpointer   user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return TRUE;

	cw_thumbnails_window_select_from_coordinate(widget,
			priv, event->x, event->y, FALSE);
	return TRUE;
}

gboolean cw_thumbnails_window_button_press_event(GtkWidget *widget,
		GdkEventButton  *event,
		gpointer   user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return TRUE;

	priv->button_press = TRUE;

	cw_thumbnails_window_select_from_coordinate(widget,
			priv, event->x, event->y, TRUE);
	return TRUE;
}

void cw_thumbnails_window_show_image(GtkWidget* widget)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return;

	CWStoreItem* item = cw_store_get_item(priv->store, priv->current_item);
	g_signal_emit(widget,
			thumbnails_window_signals[ACTIVE_LARGE_IMAGE], 0, item);
}

void cw_thumbnails_window_on_picture_switch(
		GtkWidget* widget,
		glong way,
		gpointer user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(user_data);
	if (NULL == priv || NULL == priv->store)
		return;


	int number = priv->current_item;
	PICTURE_SWITCH_NEXT == way ? ++number : --number;
	CWStoreItem* item = cw_store_get_item(priv->store, number);
	if (item) {
		priv->current_item = number;
		cw_thumbnails_window_show_image(user_data);
	}
}

void cw_thumbnails_window_select_image(GtkWidget* widget, int way)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return;

	int number = priv->current_item;
	switch (way) {
		case PICTURE_SWITCH_UP:
			number--;
			break;
		case PICTURE_SWITCH_DOWN:
			number++;
			break;
		case PICTURE_SWITCH_LEFT:
			number -= priv->max_row;
			break;
		case PICTURE_SWITCH_RIGHT:
			number += priv->max_row;
			break;
	}

	if (number < 0)
		number = 0;

	CWStoreItem* item = cw_store_get_item(priv->store, number);
	if (item)
		cw_thumbnails_window_set_seletct(widget, priv, number, TRUE);
}

gboolean cw_thumbnails_window_button_release_event(GtkWidget *widget,
		GdkEventButton  *event,
		gpointer   user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return TRUE;

	priv->button_press = FALSE;

	cw_thumbnails_window_select_from_coordinate(widget,
			priv, event->x, event->y, TRUE);
	cw_thumbnails_window_show_image(widget);
	return TRUE;
}

void cw_thumbnails_window_scroll_window(
		GtkWidget* widget,
		double x,
		double y)
{
	GdkWindow* gw = gtk_widget_get_window(widget);
	//gdk_window_freeze_updates(gw);
	gdk_window_scroll(gw, x, y);
	//g_message("%f, %f", x, y);
}

gboolean cw_thumbnails_window_scroll_event(
		GtkWidget *widget,
		GdkEventScroll  *event,
		gpointer   user_data)
{
	switch (event->direction) {
		case GDK_SCROLL_DOWN:
			cw_thumbnails_window_scroll_window(widget, -10, 0);
			break;
		default:
			break;
	}
	return TRUE;
}

void cw_thumbnails_window_size_allocate (
		GtkWidget *widget,
		GdkRectangle *allocation,
		gpointer user_data)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return ;

	priv->max_row = allocation->height / get_thumbnail_frame_height() ;
	if (0 == priv->max_row)
		priv->max_row = -1;
}

void cw_thumbnails_window_set_mode(GtkWidget* window, gint mode)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(window);
	if (NULL == priv )
		return ;
	priv->window_mode = mode;
}

gint cw_thumbnails_window_get_mode(GtkWidget* window)
{
	CWThumbnailsWindowPrivate* priv = CW_THUMBNAILS_WINDOW_GET_PRIVATE(window);
	if (NULL == priv )
		return -1;
	return priv->window_mode;
}
