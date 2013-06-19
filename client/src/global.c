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
#include "global.h"
#include <gtk/gtk.h>

#define THUMBNAIL_WIDTH		128
#define THUMBNAIL_HEIGHT	128
#define THUMBNAIL_COL_SPACE 0
#define THUMBNAIL_ROW_SPACE 0
#define THUMBNAIL_BORDER_SIZE 3
#define THUMBNAIL_SHADOW_OFFSET_X 6
#define THUMBNAIL_SHADOW_OFFSET_Y 6
#define THUMBNAIL_SHADOW_RADIUS 13

#define IMAGE_SHADOW_RADIUS 13
#define IMAGE_SHADOW_OFFSET_X 6
#define IMAGE_SHADOW_OFFSET_Y 6
#define IMAGE_BORDER_SIZE 4

//#define SHADOW_SCALE 1.06
#define SHADOW_SCALE 4

static int get_shadow_scale()
{
	return SHADOW_SCALE;
}

void cw_window_screen_changed(GtkWidget *window,
		GdkScreen *previous_screen, gpointer user_data)
{
	GdkScreen *screen = gtk_widget_get_screen (GTK_WIDGET(window));
	GdkVisual *visual = gdk_screen_get_rgba_visual (screen);

	if (visual == NULL)
		visual = gdk_screen_get_system_visual (screen);

	gtk_widget_set_visual (GTK_WIDGET(window), visual);
}

int get_thumbnail_shadow_offset_x()
{
	return THUMBNAIL_SHADOW_OFFSET_X ;
}

int get_thumbnail_shadow_offset_y()
{
	return THUMBNAIL_SHADOW_OFFSET_Y ;
}

int get_thumbnail_border_size()
{
	return THUMBNAIL_BORDER_SIZE ;
}

int get_thumbnail_width()
{
	return THUMBNAIL_WIDTH;
}

int get_thumbnail_height()
{
	return THUMBNAIL_HEIGHT;
}

int get_thumbnail_shadow_radius()
{
	return THUMBNAIL_SHADOW_RADIUS ;
}

int get_thumbnail_frame_width()
{
	return  get_thumbnail_width()
		+ get_thumbnail_border_size() * 2
		+ get_shadow_scale() * 2
		+ get_thumbnail_shadow_offset_x() * 2
		+ THUMBNAIL_COL_SPACE;
}

int get_thumbnail_frame_height()
{
	return get_thumbnail_height()
		+ get_thumbnail_border_size() * 2
		+ get_shadow_scale() * 2
		+ get_thumbnail_shadow_offset_y() * 2
		+ THUMBNAIL_ROW_SPACE;
}

void offset_rect(GdkRectangle* rt, int x_offset, int y_offset)
{
	rt->x += x_offset;
	rt->y += y_offset;
}

void calc_shadow_rect(
		const GdkRectangle* org,
		GdkRectangle* shadow)
{
	shadow->width = org->width + get_shadow_scale() * 2;
	shadow->height = org->height +  get_shadow_scale() * 2;
	shadow->x = org->x - (shadow->width - org->width) / 2;
	shadow->y = org->y - (shadow->height - org->height) / 2;
}

void site_thumbnail_rect(
		int col,
		int row,
		GdkRectangle* rt)
{
	rt->x += col * get_thumbnail_frame_width()
		+ (get_thumbnail_frame_width() - rt->width) / 2;

	rt->y += row * get_thumbnail_frame_height()
		+ (get_thumbnail_frame_height() - rt->height) / 2;
}

void get_pix_rect(GdkPixbuf* pix, int border_width, GdkRectangle* rt)
{
	rt->x = 0;
	rt->y = 0;
	rt->width = gdk_pixbuf_get_width(pix) + border_width * 2;
	rt->height = gdk_pixbuf_get_height(pix) + border_width * 2;
}


double site_picture_rect(
		const GdkRectangle* window_rect,
		GdkRectangle* image_rect)
{
	// image * scale + border * 2 + shadow_scale * 2+ offset = window
	// image * scale = window - offset - shadow_scale * 2 - border * 2
	// scale = (window - offset - shadow_scale * 2 - border * 2) / image

	int pw = image_rect->width;
	int ph = image_rect->height;

	image_rect->width = window_rect->width
		- IMAGE_SHADOW_OFFSET_X
		- get_shadow_scale() * 2
		- get_image_border_size() * 2;

	image_rect->height = window_rect->height
		- IMAGE_SHADOW_OFFSET_Y
		- get_shadow_scale() * 2
		- get_image_border_size() * 2;

	double	scale_x = image_rect->width * 1.0 / pw;
	double  scale_y = image_rect->height * 1.0 / ph;

	double	scale = scale_x > scale_y ? scale_y : scale_x;

	pw *= scale;
	ph *= scale;
	pw += get_image_border_size() * 2;
	ph += get_image_border_size() * 2;

	double offset_x = (window_rect->width - IMAGE_SHADOW_OFFSET_X - pw) / 2;
	double offset_y = (window_rect->height -  IMAGE_SHADOW_OFFSET_Y - ph) / 2;

	image_rect->x      = offset_x;
	image_rect->y      = offset_y;
	image_rect->width  = pw;
	image_rect->height = ph;

	return scale;
}

int get_image_shadow_offset_x()
{
	return IMAGE_SHADOW_OFFSET_X ;
}

int get_image_shadow_offset_y()
{
	return IMAGE_SHADOW_OFFSET_Y ;
}

int get_image_shadow_radius()
{
	return IMAGE_SHADOW_RADIUS;
}

int get_image_border_size()
{
	return IMAGE_BORDER_SIZE;
}

void draw_shadow_rgba (cairo_t   *cr,
		GdkRectangle rect,
		double radius,
		double transparency)
{
	cairo_pattern_t *pattern;
	double x0, x1, x2, x3;
	double y0, y1, y2, y3;

	x0 = rect.x;
	x1 = rect.x + radius;
	x2 = rect.x + rect.width - radius;
	x3 = rect.x + rect.width;

	y0 = rect.y;
	y1 = rect.y + radius;
	y2 = rect.y + rect.height - radius;
	y3 = rect.y + rect.height;

	/* Fill non-border part */
	cairo_set_source_rgba (cr, 0, 0, 0, transparency);
	cairo_rectangle (cr,
			x1, y1, x2 - x1, y2 - y1);
	cairo_fill (cr);

	/* Upper border */

	pattern = cairo_pattern_create_linear (0, y0, 0, y1);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, 0.0);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, transparency);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x1, y0,
			x2 - x1, y1 - y0);
	cairo_fill (cr);

	/* Bottom border */

	pattern = cairo_pattern_create_linear (0, y2, 0, y3);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x1, y2,
			x2 - x1, y3 - y2);
	cairo_fill (cr);

	/* Left border */

	pattern = cairo_pattern_create_linear (x0, 0, x1, 0);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, 0.0);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, transparency);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x0, y1,
			x1 - x0, y2 - y1);
	cairo_fill (cr);

	/* Right border */

	pattern = cairo_pattern_create_linear (x2, 0, x3, 0);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x2, y1,
			x3 - x2, y2 - y1);
	cairo_fill (cr);

	/* NW corner */

	pattern = cairo_pattern_create_radial (x1, y1, 0,
			x1, y1, radius);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x0, y0,
			x1 - x0, y1 - y0);
	cairo_fill (cr);

	/* NE corner */

	pattern = cairo_pattern_create_radial (x2, y1, 0,
			x2, y1, radius);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x2, y0,
			x3 - x2, y1 - y0);
	cairo_fill (cr);

	/* SW corner */

	pattern = cairo_pattern_create_radial (x1, y2, 0,
			x1, y2, radius);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x0, y2,
			x1 - x0, y3 - y2);
	cairo_fill (cr);

	/* SE corner */

	pattern = cairo_pattern_create_radial (x2, y2, 0,
			x2, y2, radius);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0, 0, transparency);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0, 0, 0.0);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_rectangle (cr,
			x2, y2,
			x3 - x2, y3 - y2);
	cairo_fill (cr);
}


