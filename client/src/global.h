/*
 *
 *  Portions Copyright (C) 2012 wind (lvlisong@redflag-linux.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _GLOBAL_CONFIG_
#define _GLOBAL_CONFIG_
#include <glib-object.h>
#include <gtk/gtk.h>

#define SHADOW_TRANSPARENT 0.8
#define WINDOW_TRANSPARENT 0.3

#define THUMBNAIL_MINIMIZE_WINDOW_MODE_HEIGHT (get_thumbnail_frame_height())

#define THUMBNAIL_WINDOW_SIDE_SIZE 20
#define THUMBNAIL_WINDOW_WIDTH_SCALE 0.8
#define THUMBNAIL_WINDOW_HEIGHT_SCALE 0.75

#define PICTURE_SWITCH_NEXT 0
#define PICTURE_SWITCH_PREV 1
#define PICTURE_SWITCH_UP 2
#define PICTURE_SWITCH_DOWN 3
#define PICTURE_SWITCH_LEFT 4
#define PICTURE_SWITCH_RIGHT 5

G_BEGIN_DECLS

void cw_window_screen_changed(GtkWidget *window,
		GdkScreen *previous_screen, gpointer user_data);

void draw_shadow_rgba (cairo_t   *cr,
		GdkRectangle rect,
		double radius,
		double transparency);

int get_thumbnail_shadow_offset_y();
int get_thumbnail_shadow_offset_x();

int get_thumbnail_frame_width();
int get_thumbnail_frame_height();

int get_thumbnail_width();
int get_thumbnail_height();

int get_thumbnail_border_size();
int get_thumbnail_shadow_radius();

void offset_rect(GdkRectangle* rt, int x_offset, int y_offset);
void get_pix_rect(GdkPixbuf* pix, int border_width, GdkRectangle* rt);
void site_thumbnail_rect( int col, int row, GdkRectangle* rt);
void calc_shadow_rect( const GdkRectangle* org, GdkRectangle* shadow);

double site_picture_rect(
		const GdkRectangle* window_rect,
		GdkRectangle* image_rect);
int get_image_shadow_offset_x();
int get_image_shadow_offset_y();
int get_image_shadow_radius();
int get_image_border_size();

G_END_DECLS

#endif // _GLOBAL_CONFIG_

