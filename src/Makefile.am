AM_CPPFLAGS        = -I$(top_builddir) -I$(top_srcdir)

noinst_HEADERS  = data.h display.h enhancement.h hrit.h nc_file.h model.h obs.h
bin_PROGRAMS    = twiin twiin_convert

#twiin_LDADD    = $(top_builddir)/twiin/libtwiin.la
twiin_SOURCES	= data.cc display.cc enhancement.cc hrit.cc main.cc \
		  model.cc obs.cc nc_file.cc
twiin_convert_SOURCES	= data.cc display.cc enhancement.cc hrit.cc \
			  model.cc obs.cc twiin_convert.cc nc_file.cc

if ENABLE_GTKMM
   noinst_HEADERS += console.h cross_section.h
   twiin_SOURCES += console.cc cross_section.cc
endif

