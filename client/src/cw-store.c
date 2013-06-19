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

#include "cw-store.h"
#include "cw-store-item.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

G_DEFINE_TYPE (CWStore, cw_store, G_TYPE_OBJECT);

/* Define the private structure in the .c file */
#define CW_STORE_ITE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CW_TYPE_STORE, CWStorePrivate))

struct _CWStorePrivate
{
	GArray* store;
};


static void
cw_store_init (CWStore *cw_store)
{

	CWStorePrivate* priv = CW_STORE_ITE_GET_PRIVATE(cw_store);
	priv->store          = g_array_new(TRUE, TRUE, sizeof(CWStoreItem*));

	/* TODO: Add initialization code here */
}

static void
cw_store_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (cw_store_parent_class)->finalize (object);
}

static void
cw_store_class_init (CWStoreClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (CWStorePrivate));

	object_class->finalize = cw_store_finalize;
}


CWStore* cw_store_new()
{
	g_type_init();
	return g_object_new(cw_store_get_type(), NULL);
}

gboolean cw_store_load_from_directory(CWStore* self, const gchar* dirpath)
{
	g_assert(dirpath);
	DIR* dir = opendir(dirpath);
	if (NULL == dir) {
		perror("opendir");
		return FALSE;
	}

	CWStorePrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	int fdir = open(".", O_RDONLY);
	chdir(dirpath);

	struct dirent* pd;
	struct stat buffer;

	char cwd[2048];
	getcwd(cwd, 2048);
	while ( (pd = readdir(dir))) {
		if ( 0 != stat(pd->d_name, &buffer)) {
			printf("%s\n", pd->d_name);
			perror("stat");
			continue;
		}
		if (S_ISREG(buffer.st_mode)) {
			CWStoreItem* item = cw_store_item_new();
			gchar* path = g_strdup_printf("%s/%s", cwd, pd->d_name);
			if (cw_store_item_load_from_file(item, path))
				g_array_append_val(priv->store, item);
			else
				g_object_unref(item);
			g_free(path);
		} else if (S_ISDIR(buffer.st_mode)) {
			gchar* path = g_strdup_printf("%s/%s", cwd, pd->d_name);
			if (0 == strcmp(".", pd->d_name) || 0 == strcmp("..", pd->d_name))
					continue;
			cw_store_load_from_directory(self, path);
			g_free(path);
		}
	}
	fchdir(fdir);
	close(fdir);
	closedir(dir);

	return TRUE;
}

CWStoreItem* cw_store_get_item(CWStore* self, gint number)
{
	CWStorePrivate* priv = CW_STORE_ITE_GET_PRIVATE(self);
	if (priv && priv->store) {
		if (priv->store->len <= number)
			return NULL;
		return g_array_index(priv->store, CWStoreItem*, number);
	}
	return NULL;
}
