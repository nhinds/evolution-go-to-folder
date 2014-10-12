AM_CPPFLAGS =                                                   \
	-I$(EVOLUTION_SOURCE)                           	\
	-I$(top_srcdir)                                 	\
	-DLOCALEDIR="\"$(LOCALEDIR)\""                  	\
	-DEVOLUTION_VERSION_STRING=\"$(EVOLUTION_EXEC_VERSION)\"\
	-DEVOLUTION_VERSION=$(evolution_version_int)            \
	-DEVOLUTION_GLADEDIR=\""$(gladedir)"\"                  \
	-DEVOLUTION_ICONDIR=\""$(ICON_DIR)"\"                   \
	-DLIBNOTIFY_VERSION=$(LIBNOTIFY_VERSION)		\
	$(MODULE_CFLAGS)


moduledir = $(MODULE_INSTALL_DIR)

module_LTLIBRARIES = module-hello-world.la

module_hello_world_la_SOURCES = hello-world.c
module_hello_world_la_CFLAGS=$(libappindicator_CFLAGS)
module_hello_world_la_LIBADD = $(LIBGCONFBRIDGE_LIBS) $(LIBNOTIFY_LIBS)	\
	$(libappindicator_LIBS)
module_hello_world_la_LDFLAGS = -module -avoid-version -rpath $(MODULE_INSTALL_DIR)

CLEANFILES = $(BUILT_SOURCES)
