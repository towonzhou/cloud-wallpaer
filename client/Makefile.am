## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

cloud_wallpaperdocdir = ${prefix}/doc/cloud_wallpaper
cloud_wallpaperdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(cloud_wallpaperdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(cloud_wallpaperdocdir)
