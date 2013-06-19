## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-D_FILE_OFFSET_BITS=64 \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(CLOUD_WALLPAPER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = cloud_wallpaper

cloud_wallpaper_SOURCES = \
	global.h \
	global.c \
	main.c \
	cloud-wallpaper.h \
	cloud-wallpaper.c \
	cw-window.c \
	cw-window.h \
	cw-thumbnails-window.c \
	cw-thumbnails-window.h \
	cw-picture-window.c \
	cw-picture-window.h \
	cw-store.c \
	cw-store.h \
	cw-store-item.c \
	cw-store-item.h 

cloud_wallpaper_LDFLAGS = \
	-Wl,--export-dynamic

cloud_wallpaper_LDADD = $(CLOUD_WALLPAPER_LIBS)


