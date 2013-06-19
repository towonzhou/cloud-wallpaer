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

#ifndef _CW_STORE_ITEM_H_
#define _CW_STORE_ITEM_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CW_TYPE_STORE_ITEM             (cw_store_item_get_type ())
#define CW_STORE_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CW_TYPE_STORE_ITEM, CWStoreItem))
#define CW_STORE_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CW_TYPE_STORE_ITEM, CWStoreItemClass))
#define CW_IS_STORE_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CW_TYPE_STORE_ITEM))
#define CW_IS_STORE_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CW_TYPE_STORE_ITEM))
#define CW_STORE_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CW_TYPE_STORE_ITEM, CWStoreItemClass))

typedef struct _CWStoreItemClass CWStoreItemClass;
typedef struct _CWStoreItem CWStoreItem;
typedef struct _CWStoreItemPrivate CWStoreItemPrivate;

typedef struct _GdkPixbuf GdkPixbuf;

struct _CWStoreItemClass
{
	GObjectClass parent_class;

	/* Signals */
	void(* load_complete) (CWStoreItem *self);
};

struct _CWStoreItem
{
	GObject parent_instance;

	CWStoreItemPrivate* priv;

};

GType cw_store_item_get_type (void) G_GNUC_CONST;
CWStoreItem* cw_store_item_new(void);

gboolean cw_store_item_load_from_file (
		CWStoreItem* self,
		const gchar* filename);

GdkPixbuf* cw_store_item_get_pixbuf(CWStoreItem* self);
void cw_store_item_clean_pixbuf(CWStoreItem* self);
GdkPixbuf* cw_store_item_get_thumbnail_pixbuf(CWStoreItem* self);
gboolean   cw_store_item_load_large_image(CWStoreItem* self);
gboolean   cw_store_item_get_image_size(CWStoreItem* self, gint* w, gint* h);

G_END_DECLS

#endif /* _CW_STORE_ITEM_H_ */

