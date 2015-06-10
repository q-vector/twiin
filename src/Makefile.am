AM_CPPFLAGS        = -I$(top_builddir) -I$(top_srcdir)

noinst_HEADERS  = data.h display.h enhancement.h hrit.h nc_file.h
bin_PROGRAMS    = twiin

#twiin_LDADD    = $(top_builddir)/twiin/libtwiin.la
twiin_SOURCES	= data.cc display.cc enhancement.cc hrit.cc main.cc nc_file.cc

if ENABLE_GTKMM
   noinst_HEADERS += console.h cross_section.h
   twiin_SOURCES += console.cc cross_section.cc
endif

