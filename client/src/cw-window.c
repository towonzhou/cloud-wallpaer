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

#include "cw-window.h"
#include <gtk/gtk.h>
#include "cw-store.h"
#include "cw-store-item.h"
#include "global.h"
#include "cw-picture-window.h"
#include "cw-thumbnails-window.h"
//#include <librsvg/rsvg-cairo.h>

enum {STATUS_NORMAL, STATUS_HOVER, STATUS_SELECTED};

G_DEFINE_TYPE (CWWindow, cw_window, GTK_TYPE_WINDOW);

/* Define the private structure in the .c file */
#define CW_WINDOW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CW_TYPE_WINDOW, CWWindowPrivate))

struct _CWWindowPrivate
{
	GtkWidget*   thumbnails_window;
	GtkWidget*   picture_window;
	GtkWidget*	 thumbnails_alignment;
	GtkWidget* 	 main_box;
	//GtkWidget*	 picture_alignment;
};

static void cw_window_set_pos_size(GtkWidget* widget, CWWindowPrivate* priv)
{
	GdkScreen *screen = gtk_widget_get_screen (GTK_WIDGET(widget));

	gint w = gdk_screen_get_width(screen)
		* THUMBNAIL_WINDOW_WIDTH_SCALE
		- THUMBNAIL_WINDOW_SIDE_SIZE * 2;
	gint h = gdk_screen_get_height(screen)
		* THUMBNAIL_WINDOW_HEIGHT_SCALE
		- THUMBNAIL_WINDOW_SIDE_SIZE * 2;

	w = w / get_thumbnail_frame_width()
		* get_thumbnail_frame_width();

	h = h / get_thumbnail_frame_height()
		* get_thumbnail_frame_height();

	w += THUMBNAIL_WINDOW_SIDE_SIZE * 2;
	h += THUMBNAIL_WINDOW_SIDE_SIZE * 2;

	gtk_widget_set_size_request(GTK_WIDGET(widget), w, h);
	int x = (gdk_screen_get_width(screen) - w) / 2;
	int y = (gdk_screen_get_height(screen) - h) / 2;
	gtk_window_move(GTK_WINDOW(widget), x, y);
}

void cw_window_set_child_window_size(
		GtkWidget* window,
		CWWindowPrivate* priv,
		gint window_mode,
		gboolean isfullscreen)
{
	int w, h;

	//if ( isfullscreen)
		//gtk_window_get_size(GTK_WINDOW(window), &w, &h);
	//else
		gtk_widget_get_size_request(GTK_WIDGET(window), &w, &h);

	cw_thumbnails_window_set_mode(priv->thumbnails_window,
			window_mode);

	if (MINIMIZE_WINDOW_MODE == window_mode) {
		if ( isfullscreen) {
			gtk_widget_hide(priv->thumbnails_alignment);
			gtk_widget_set_size_request(
					GTK_WIDGET(priv->picture_window), -1, -1);
		} else {
			int ph, th;
			ph = h - THUMBNAIL_MINIMIZE_WINDOW_MODE_HEIGHT;
			th = THUMBNAIL_MINIMIZE_WINDOW_MODE_HEIGHT;

			gtk_widget_set_size_request(
					GTK_WIDGET(priv->picture_window), -1, ph);

			gtk_alignment_set_padding(
					GTK_ALIGNMENT(priv->thumbnails_alignment),
					0, 0, 0, 0);

			gtk_box_set_child_packing(
					GTK_BOX(priv->main_box),
					priv->thumbnails_alignment,
					FALSE,
					TRUE,
					0,
					GTK_PACK_END);
			//gtk_widget_set_vexpand_set( priv->thumbnails_alignment, FALSE);

			gtk_widget_set_size_request(
					GTK_WIDGET(priv->thumbnails_alignment), -1, th);

			gtk_widget_show_all(priv->thumbnails_alignment);
		}

		gtk_widget_show_all(priv->picture_window);
	} else {
		gtk_widget_hide(priv->picture_window);

		//gtk_widget_set_vexpand_set( priv->thumbnails_alignment, TRUE);
			gtk_box_set_child_packing(
					GTK_BOX(priv->main_box),
					priv->thumbnails_alignment,
					TRUE,
					TRUE,
					0,
					GTK_PACK_END);

		gtk_alignment_set_padding(
				GTK_ALIGNMENT(priv->thumbnails_alignment),
				THUMBNAIL_WINDOW_SIDE_SIZE,
				THUMBNAIL_WINDOW_SIDE_SIZE,
				THUMBNAIL_WINDOW_SIDE_SIZE,
				THUMBNAIL_WINDOW_SIDE_SIZE);

		gtk_widget_set_size_request(
				GTK_WIDGET(priv->thumbnails_alignment),
				-1, -1);
		gtk_widget_show_all(priv->thumbnails_alignment);
	}
}

void cw_window_set_child_window(GtkWidget* widget, CWWindowPrivate* priv)
{
	GdkWindowState state = gdk_window_get_state(
			gtk_widget_get_window(GTK_WIDGET(widget)));
	gint window_mode = cw_thumbnails_window_get_mode(priv->thumbnails_window);
	gboolean isfullscreen = state & GDK_WINDOW_STATE_FULLSCREEN;

	cw_window_set_child_window_size(widget, priv, window_mode, isfullscreen);
}


static void
cw_window_init (CWWindow *cw_window)
{

	CWWindowPrivate *priv   = CW_WINDOW_GET_PRIVATE(cw_window);
	cw_window_screen_changed(GTK_WIDGET(cw_window), NULL, NULL);

	//gtk_window_set_type_hint(GTK_WINDOW(cw_window),
	//GDK_WINDOW_TYPE_HINT_DOCK);

	gtk_window_set_decorated(GTK_WINDOW(cw_window), FALSE);
	//gtk_window_set_resizable(GTK_WINDOW(cw_window), FALSE);
	//gtk_window_set_position(GTK_WINDOW(cw_window), GTK_WIN_POS_CENTER_ALWAYS);
	//gtk_window_set_skip_taskbar_hint(GTK_WINDOW(cw_window), FALSE);
	//gtk_window_set_skip_pager_hint(GTK_WINDOW(cw_window), FALSE);
	//gtk_window_set_default_size(GTK_WINDOW(cw_window), 800, 480);
	cw_window_set_pos_size(GTK_WIDGET(cw_window), priv);
	//gtk_window_maximize(GTK_WINDOW(cw_window));

	gtk_widget_add_events(GTK_WIDGET(cw_window),
			GDK_POINTER_MOTION_MASK
			| GDK_KEY_RELEASE_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK);


	priv->picture_window = cw_picture_window_new();
	//priv->picture_alignment = gtk_alignment_new(0.5, 0.5, 1, 1);
	//gtk_alignment_set_padding(
			//GTK_ALIGNMENT(priv->picture_alignment),
			//0, 0, 0, 0);

	//gtk_container_add(
			//GTK_CONTAINER(priv->picture_alignment),
			//priv->picture_window);

	priv->thumbnails_window = cw_thumbnails_window_new();
	priv->thumbnails_alignment = gtk_alignment_new(0.5, 0.5, 1, 1);
	gtk_alignment_set_padding(
			GTK_ALIGNMENT(priv->thumbnails_alignment),
			THUMBNAIL_WINDOW_SIDE_SIZE,
			THUMBNAIL_WINDOW_SIDE_SIZE,
			THUMBNAIL_WINDOW_SIDE_SIZE,
			THUMBNAIL_WINDOW_SIDE_SIZE);

	gtk_container_add(
			GTK_CONTAINER(priv->thumbnails_alignment),
			priv->thumbnails_window);

	priv->main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_end(
			GTK_BOX(priv->main_box),
			priv->thumbnails_alignment,
			TRUE,
			TRUE,
			0);

	gtk_box_pack_end(
			GTK_BOX(priv->main_box),
			priv->picture_window,
			TRUE,
			TRUE,
			0);

	gtk_container_add(
		GTK_CONTAINER(cw_window),
		priv->main_box);

	GdkRGBA rgba = { 1, 1, 1, 0.0};
	gtk_widget_override_background_color(GTK_WIDGET(cw_window),
			GTK_STATE_FLAG_NORMAL, &rgba);

	gtk_widget_override_background_color(GTK_WIDGET(priv->main_box),
			GTK_STATE_FLAG_NORMAL, &rgba);

	gtk_widget_show_all(GTK_WIDGET (priv->thumbnails_alignment));
	gtk_widget_show(GTK_WIDGET (priv->main_box));
	gtk_widget_show(GTK_WIDGET (cw_window));

	cw_window_set_child_window(GTK_WIDGET(cw_window), priv);

	gtk_widget_shape_combine_region(GTK_WIDGET(cw_window), NULL);

	g_signal_connect(
			priv->picture_window,
			"picture-switch",
			G_CALLBACK(cw_thumbnails_window_on_picture_switch),
			priv->thumbnails_window);

	g_signal_connect(
			priv->thumbnails_window,
			"active_large_image",
			G_CALLBACK(cw_window_active_large_image),
			cw_window);

	/* TODO: Add initialization code here */
}

static void
cw_window_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (cw_window_parent_class)->finalize (object);
}

static void
cw_window_class_init (CWWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CWWindowPrivate));

	g_signal_override_class_handler("key-release-event",
			cw_window_get_type(),
			G_CALLBACK(cw_window_key_release_event));

	object_class->finalize = cw_window_finalize;
}

CWStore* cw_window_set_store(GtkWidget* window, CWStore* store)
{
	CWWindowPrivate *priv = CW_WINDOW_GET_PRIVATE(window);
	if (priv && priv->thumbnails_window)
		return cw_thumbnails_window_set_store(priv->thumbnails_window, store);
	return NULL;
}

CWStore* cw_window_get_store(GtkWidget* window)
{
	CWWindowPrivate *priv = CW_WINDOW_GET_PRIVATE(window);
	if (priv && priv->thumbnails_window)
		return cw_thumbnails_window_get_store(priv->thumbnails_window);
	return NULL;
}

GtkWidget* cw_window_new()
{
	g_type_init();
	return g_object_new(cw_window_get_type(), NULL);
}

void cw_window_active_thumbnails_mode(GtkWidget* widget)
{
	CWWindowPrivate* priv = CW_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return;

	cw_thumbnails_window_set_mode(priv->thumbnails_window, FULL_WINDOW_MODE);
	cw_window_set_child_window(GTK_WIDGET(widget), priv);
}

void cw_window_active_large_image(
		GtkWidget* widget, gpointer item, gpointer user_data)
{
	CWWindowPrivate* priv = CW_WINDOW_GET_PRIVATE(user_data);
	if (NULL == priv )
		return;

	cw_thumbnails_window_set_mode(priv->thumbnails_window,
			MINIMIZE_WINDOW_MODE);
	cw_window_set_child_window(GTK_WIDGET(user_data), priv);
	cw_picture_window_set_store_item(priv->picture_window, item);
}

void cw_window_set_fullscreen(GtkWidget* widget)
{
	CWWindowPrivate* priv = CW_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return;

	GdkWindowState state = gdk_window_get_state(
			gtk_widget_get_window(widget));

	gboolean isfullscreen = state & GDK_WINDOW_STATE_FULLSCREEN;
	if ( isfullscreen)
		gtk_window_unfullscreen(GTK_WINDOW(widget));
	else
		gtk_window_fullscreen(GTK_WINDOW(widget));

	gint window_mode = cw_thumbnails_window_get_mode(priv->thumbnails_window);
	cw_window_set_child_window_size(
			GTK_WIDGET(widget),
			priv,
			window_mode,
			!isfullscreen);
}

gboolean cw_window_key_release_event(
		GtkWidget *widget,
		GdkEventKey   *event,
		gpointer   user_data)
{
	CWWindowPrivate* priv = CW_WINDOW_GET_PRIVATE(widget);
	if (NULL == priv )
		return FALSE;

	if (cw_thumbnails_window_get_mode(priv->thumbnails_window)
			== FULL_WINDOW_MODE) {
		switch (event->keyval) {
			case GDK_KEY_Return:
			case GDK_KEY_KP_Enter:
				cw_thumbnails_window_show_image(priv->thumbnails_window);
				break;
			case GDK_KEY_Escape:
			case GDK_KEY_Q:
			case GDK_KEY_q:
				gtk_widget_destroy(widget);
				break;
			case GDK_KEY_Tab:
			case GDK_KEY_Left:
			case GDK_KEY_KP_Left:
				cw_thumbnails_window_select_image(priv->thumbnails_window,
						PICTURE_SWITCH_LEFT);
				break;
			case GDK_KEY_Right:
			case GDK_KEY_KP_Right:
				cw_thumbnails_window_select_image(priv->thumbnails_window,
						PICTURE_SWITCH_RIGHT);
				break;
			case GDK_KEY_Up:
			case GDK_KEY_KP_Up:
				cw_thumbnails_window_select_image(priv->thumbnails_window,
						PICTURE_SWITCH_UP);
				break;
			case GDK_KEY_Down:
			case GDK_KEY_KP_Down:
				cw_thumbnails_window_select_image(priv->thumbnails_window,
						PICTURE_SWITCH_DOWN);
			case GDK_KEY_F:
			case GDK_KEY_f:
				cw_window_set_fullscreen(widget);
				break;
		}
	} else {
		switch (event->keyval) {
			case GDK_KEY_Return:
			case GDK_KEY_KP_Enter:
			case GDK_KEY_F:
			case GDK_KEY_f:
				cw_window_set_fullscreen(widget);
				break;
			case GDK_KEY_Escape:
			case GDK_KEY_Q:
			case GDK_KEY_q:
				cw_window_active_thumbnails_mode(widget);
				break;
			case GDK_KEY_BackSpace:
			case GDK_KEY_Left:
			case GDK_KEY_KP_Left:
				g_signal_emit_by_name(priv->picture_window,
						"picture-switch",
						PICTURE_SWITCH_PREV);
				break;
			case GDK_KEY_space:
			case GDK_KEY_Tab:
			case GDK_KEY_Right:
			case GDK_KEY_KP_Right:
				g_signal_emit_by_name(priv->picture_window,
						"picture-switch",
						PICTURE_SWITCH_NEXT);
				break;
		}

	}
	return TRUE;
}

