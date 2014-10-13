AM_CPPFLAGS =                                                   \
	-I$(EVOLUTION_SOURCE)                           	\
	-DEVOLUTION_VERSION_STRING=\"$(EVOLUTION_EXEC_VERSION)\"\
	-DEVOLUTION_VERSION=$(evolution_version_int)            \
	-DEVOLUTION_GLADEDIR=\""$(gladedir)"\"                  \
	-DEVOLUTION_ICONDIR=\""$(ICON_DIR)"\"                   \
	-Werror                                                 \
	$(MODULE_CFLAGS)


moduledir = $(MODULE_INSTALL_DIR)

module_LTLIBRARIES = module-go-to-folder.la

module_go_to_folder_la_SOURCES = go-to-folder.c
module_go_to_folder_la_LIBADD = $(LIBGCONFBRIDGE_LIBS)
module_go_to_folder_la_LDFLAGS = -module -avoid-version -rpath $(MODULE_INSTALL_DIR)

CLEANFILES = $(BUILT_SOURCES)
