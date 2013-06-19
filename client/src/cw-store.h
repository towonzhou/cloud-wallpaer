/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cloud-wallpaper.bak
 * Copyright (C) 2013 lvlisong <lvlisong@redflag-linux>
 *
 * cloud-wallpaper.bak is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cloud-wallpaper.bak is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CW_STORE_H_
#define _CW_STORE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CW_TYPE_STORE             (cw_store_get_type ())
#define CW_STORE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CW_TYPE_STORE, CWStore))
#define CW_STORE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CW_TYPE_STORE, CWStoreClass))
#define CW_IS_STORE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CW_TYPE_STORE))
#define CW_IS_STORE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CW_TYPE_STORE))
#define CW_STORE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CW_TYPE_STORE, CWStoreClass))

typedef struct _CWStoreClass CWStoreClass;
typedef struct _CWStore CWStore;
typedef struct _CWStorePrivate CWStorePrivate;
typedef struct _CWStoreItem CWStoreItem;


struct _CWStoreClass
{
	GObjectClass parent_class;
};

struct _CWStore
{
	GObject parent_instance;

 	CWStorePrivate* priv;
};

GType cw_store_get_type (void) G_GNUC_CONST;
CWStore* cw_store_new();
gboolean cw_store_load_from_directory(CWStore* slef, const gchar* dirpath);

CWStoreItem* cw_store_get_item(CWStore* self, gint number);

G_END_DECLS

#endif /* _CW_STORE_H_ */

