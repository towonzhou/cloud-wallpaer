/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cloud-wallpaper
 * Copyright (C) 2013 lvlisong <lvlisong@redflag-linux>
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

#include "cw-store-item.h"
#include "global.h"
#include <gtk/gtk.h>

enum
{
	LOAD_COMPLETE,

	LAST_SIGNAL
};


static guint store_item_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (CWStoreItem, cw_store_item, G_TYPE_OBJECT);


/* Define the private structure in the .c file */
#define CW_STORE_ITE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CW_TYPE_STORE_ITEM, CWStoreItemPrivate))

struct _CWStoreItemPrivate
{
	GdkPixbuf* pixbuf;
	GdkPixbuf* thumbnail_pixbuf;
	gint 	   width;
	gint 	   height;
	gint	   level;
	gchar*	   filename;
};


static void
cw_store_item_init (CWStoreItem *cw_store_item)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(cw_store_item);
	priv->pixbuf             = NULL;
	priv->thumbnail_pixbuf   = NULL;
	priv->width              = 400;
	priv->height             = 400;
	priv->filename           = NULL;
	priv->level              = 0;

	/* TODO: Add initialization code here */
}

static void
cw_store_item_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (cw_store_item_parent_class)->finalize (object);
}

static void
cw_store_item_load_complete (CWStoreItem *self)
{
	/* TODO: Add default signal handler implementation here */
}

static void
cw_store_item_class_init (CWStoreItemClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GObjectClass* parent_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (klass, sizeof (CWStoreItemPrivate));

	object_class->finalize = cw_store_item_finalize;

	klass->load_complete = cw_store_item_load_complete;

	store_item_signals[LOAD_COMPLETE] =
		g_signal_new ("load-complete",
		              G_OBJECT_CLASS_TYPE (klass),
		              G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (CWStoreItemClass, load_complete),
		              NULL, NULL,
					  g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0
		              );
}


CWStoreItem* cw_store_item_new(void)
{
	g_type_init();
	return g_object_new(cw_store_item_get_type(), NULL);
}


gboolean cw_store_item_get_image_size(CWStoreItem* self, gint* w, gint* h)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (NULL == priv)
		return FALSE;

	*w = priv->width;
	*h = priv->height;
	return TRUE;
}

gboolean cw_store_item_load_from_file (CWStoreItem* self, const gchar* filename)
{
	g_assert(filename);
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);

	if (NULL == gdk_pixbuf_get_file_info(filename, &priv->width, &priv->height))
		return FALSE;

	GError* err = NULL;
	priv->thumbnail_pixbuf = gdk_pixbuf_new_from_file_at_scale(
			filename,
			get_thumbnail_width(),
			get_thumbnail_height(),
			TRUE,
			&err);
	if (NULL == priv->thumbnail_pixbuf) {
		g_warning("%s\n", err->message);
		g_error_free(err);
		return FALSE;
	}
	priv->filename = g_strdup(filename);

	return TRUE;
}

GdkPixbuf* cw_store_item_get_thumbnail_pixbuf(CWStoreItem* self)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (priv)
		return priv->thumbnail_pixbuf;
	return NULL;
}

GdkPixbuf* cw_store_item_get_pixbuf(CWStoreItem* self)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (priv)
		return priv->pixbuf;
	return NULL;
}

void cw_store_item_clean_pixbuf(CWStoreItem* self)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (priv && priv->pixbuf) {
		g_object_unref(priv->pixbuf);
		priv->pixbuf = NULL;
	}
}

gboolean cw_store_item_load_large_image(CWStoreItem* self)
{
	CWStoreItemPrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (NULL == priv || NULL == priv->filename)
		return FALSE;

	GError* err = NULL;
	if (NULL == priv->pixbuf) {
		priv->pixbuf = gdk_pixbuf_new_from_file(priv->filename, &err);
		if (NULL == priv->pixbuf) {
			g_warning("%s\n", err->message);
			g_error_free(err);
			return FALSE;
		}
	}

	g_signal_emit(self, store_item_signals[LOAD_COMPLETE], 0, NULL);
	return TRUE;
}

