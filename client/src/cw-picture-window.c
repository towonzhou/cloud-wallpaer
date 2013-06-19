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

#include "cw-picture-window.h"
#include "cw-store-item.h"
#include "global.h"
#include <gtk/gtk.h>

enum
{
	PICTURE_SWITCH,

	LAST_SIGNAL
};


static guint picture_window_signals[LAST_SIGNAL] = { 0 };



G_DEFINE_TYPE (CWPictureWindow, cw_picture_window, GTK_TYPE_DRAWING_AREA);

/* Define the private structure in the .c file */
#define CW_PICTURE_WINDOW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CW_TYPE_PICTURE_WINDOW, CWPictureWindowPrivate))

struct _CWPictureWindowPrivate
{
	CWStoreItem*   store_item;
	GtkWidget*     spinner;
	gulong         connect_id;
};



static void
cw_picture_window_init (CWPictureWindow *cw_picture_window)
{

	CWPictureWindowPrivate *priv =
		CW_PICTURE_WINDOW_GET_PRIVATE(cw_picture_window);
	priv->store_item = NULL;
	priv->spinner    = NULL;
	priv->connect_id = 0;

	priv->spinner = gtk_spinner_new();
	GtkWidget* alignment = gtk_alignment_new(0.5, 0.5, 1, 1);
	//gtk_alignment_set_padding(
			//GTK_ALIGNMENT(alignment),
			//IMAGE_SHADOW_RADIUS,
			//IMAGE_SHADOW_RADIUS + IMAGE_SHADOW_OFFSET_X * 2,
			//IMAGE_SHADOW_RADIUS,
			//IMAGE_SHADOW_RADIUS + IMAGE_SHADOW_OFFSET_Y * 2);

	gtk_container_add (GTK_CONTAINER (alignment), priv->spinner);
	//gtk_container_add (GTK_CONTAINER (cw_picture_window), alignment);

	gtk_widget_add_events(GTK_WIDGET(cw_picture_window),
			GDK_POINTER_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK);
			//| GDK_BUTTON_RELEASE_MASK);

	GdkRGBA rgba = { 1, 1, 1, 0.0};
	gtk_widget_override_background_color(GTK_WIDGET(cw_picture_window),
			GTK_STATE_FLAG_NORMAL, &rgba);
	/* TODO: Add initialization code here */
}

static void
cw_picture_window_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (cw_picture_window_parent_class)->finalize (object);
}

static void
cw_picture_window_picture_switch (CWPictureWindow *self, glong way)
{
	/* TODO: Add default signal handler implementation here */
}

static void
cw_picture_window_class_init (CWPictureWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CWPictureWindowPrivate));

	klass->picture_switch = cw_picture_window_picture_switch;
	picture_window_signals[PICTURE_SWITCH] =
		g_signal_new("picture-switch",
				G_OBJECT_CLASS_TYPE(klass),
		        G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		        G_STRUCT_OFFSET (CWPictureWindowClass, picture_switch),
				NULL, NULL,
				g_cclosure_marshal_VOID__LONG,
				G_TYPE_NONE, 1,
				G_TYPE_LONG
				);

	g_signal_override_class_handler("screen-changed",
			cw_picture_window_get_type(),
			G_CALLBACK(cw_window_screen_changed));

	g_signal_override_class_handler("draw",
			cw_picture_window_get_type(),
			G_CALLBACK(cw_picture_window_draw));

	g_signal_override_class_handler("button-press-event",
			cw_picture_window_get_type(),
			G_CALLBACK(cw_picture_window_button_press_event));

	g_signal_override_class_handler("window-state-event",
			cw_picture_window_get_type(),
			G_CALLBACK(cw_picture_window_state_event));

	object_class->finalize = cw_picture_window_finalize;
}

GtkWidget* cw_picture_window_new()
{
	g_type_init();
	return g_object_new(cw_picture_window_get_type(), NULL);
}

static void cw_picture_window_show_image(
		GtkWidget* widget,
		CWPictureWindowPrivate* priv)
{
	int pw, ph;
	cw_store_item_get_image_size(priv->store_item, &pw, &ph);

	//gtk_window_resize(GTK_WINDOW(widget), w, h);
	GdkWindow* gw = gtk_widget_get_window(widget);
	if (gw)
		gdk_window_invalidate_rect(gw, NULL, FALSE);
	//}
}

static void cw_picture_window_on_image_load_complete(
		CWStoreItem* item,
		gpointer user_data)
{
	CWPictureWindowPrivate *priv = CW_PICTURE_WINDOW_GET_PRIVATE(user_data);
	if (NULL == priv)
		return ;

	g_signal_handler_disconnect(item, priv->connect_id);
	priv->connect_id = 0;
	cw_picture_window_show_image(GTK_WIDGET(user_data), priv);
}

void cw_picture_window_set_store_item(GtkWidget* window,
		CWStoreItem* store_item)
{
	CWPictureWindowPrivate *priv = CW_PICTURE_WINDOW_GET_PRIVATE(window);
	if (NULL == priv)
		return ;

	CWStoreItem* old = priv->store_item;
	priv->store_item = store_item;
	if (old == store_item)
		return;

	if (NULL == store_item) {
		gtk_widget_hide(window);
		return;
	}

	if ((NULL != old) && (priv->connect_id > 0)) {
		g_signal_handler_disconnect(old, priv->connect_id);
		priv->connect_id = 0;
	}

	GdkPixbuf* pix = cw_store_item_get_pixbuf(store_item);
	if (NULL == pix) {
		gtk_spinner_start (GTK_SPINNER (priv->spinner));
		gtk_widget_show(priv->spinner);
		//gtk_widget_show_all (GTK_WIDGET (window));
		priv->connect_id = g_signal_connect(store_item,
				"load_complete",
				G_CALLBACK(cw_picture_window_on_image_load_complete),
				window);
		cw_store_item_load_large_image(store_item);
	} else {
		cw_picture_window_show_image(window, priv);
	}
	if (old)
		cw_store_item_clean_pixbuf(old);
	//return old;
}

gboolean cw_picture_window_draw(GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data)
{
	CWPictureWindowPrivate* priv = CW_PICTURE_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv || NULL == priv->store_item)
		return TRUE;

	GdkRectangle window_rect = {
		0,
		0,
		gtk_widget_get_allocated_width(widget),
		gtk_widget_get_allocated_height(widget)
	};

	int pw, ph;
	cw_store_item_get_image_size(priv->store_item, &pw, &ph);
	GdkRectangle image_rect = { 0, 0, pw, ph};

	double scale = site_picture_rect(&window_rect, &image_rect);

	GdkRectangle shadow_rect;
	calc_shadow_rect(&image_rect, &shadow_rect);
	offset_rect(&shadow_rect,
			get_image_shadow_offset_x(),
			get_image_shadow_offset_y());

	draw_shadow_rgba(cr, shadow_rect, get_image_shadow_radius(), SHADOW_TRANSPARENT);

	cairo_rectangle(
			cr, image_rect.x, image_rect.y,
			image_rect.width,
			image_rect.height);

	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_fill(cr);

	offset_rect(&image_rect, get_image_border_size(), get_image_border_size());
	cairo_translate(cr, image_rect.x, image_rect.y);
	cairo_scale(cr, scale, scale);
	gdk_cairo_set_source_pixbuf(cr,
			cw_store_item_get_pixbuf(priv->store_item), 0, 0);
	cairo_paint(cr);

	return TRUE;
}


gboolean cw_picture_window_button_in_edge(GtkWidget* widget, int x, int y)
{
	int width = gtk_widget_get_allocated_width(widget);
	double edge = x * 1.0 / width;
#define EDGE_LEFT 0.2
#define EDGE_RIGHT 0.8
	if ( edge < EDGE_LEFT) {
		g_signal_emit(widget,
				picture_window_signals[PICTURE_SWITCH],
				0,
				PICTURE_SWITCH_PREV);
		return TRUE;
	} else if (edge > EDGE_RIGHT) {
		g_signal_emit(widget,
				picture_window_signals[PICTURE_SWITCH],
				0,
				PICTURE_SWITCH_NEXT);
		return TRUE;
	}
	return FALSE;
}

//gboolean cw_picture_window_button_release_event(
		//GtkWidget *widget,
		//GdkEventButton *event,
		//gpointer func_data)
//{
	//return TRUE;
//}

gboolean cw_picture_window_button_press_event(
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer func_data)
{
	if (cw_picture_window_button_in_edge(widget, event->x, event->y))
		return TRUE;

	//if ( GDK_2BUTTON_PRESS == event->type ||
			//GDK_3BUTTON_PRESS == event->type) {
		//if (GDK_2BUTTON_PRESS == event->type) {
		//}
	//}
	return TRUE;
}


gboolean cw_picture_window_state_event(
		GtkWidget *widget,
		GdkEventWindowState *event,
		gpointer  user_data)
{
	if (GDK_WINDOW_STATE_MAXIMIZED & event->changed_mask
			|| GDK_WINDOW_STATE_FULLSCREEN & event->changed_mask) {

		if (!(GDK_WINDOW_STATE_MAXIMIZED & event->new_window_state
					|| GDK_WINDOW_STATE_FULLSCREEN
					& event->new_window_state)) {

			CWPictureWindowPrivate *priv =
				CW_PICTURE_WINDOW_GET_PRIVATE(widget);
			if (NULL == priv)
				return FALSE;

			cw_picture_window_show_image(widget, priv);
		}
	}
	return FALSE;
}

