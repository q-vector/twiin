AM_CPPFLAGS        = -I$(top_builddir) -I$(top_srcdir)

noinst_HEADERS  = data.h enhancement.h hrit.h nc_file.h model.h obs.h twiin.h
#bin_PROGRAMS    = twiin twiin_convert
bin_PROGRAMS    = twiin

#twiin_LDADD    = $(top_builddir)/twiin/libtwiin.la
twiin_SOURCES	= data.cc enhancement.cc hrit.cc main.cc \
		  model.cc obs.cc twiin.cc nc_file.cc
twiin_convert_SOURCES	= cross_section.cc data.cc enhancement.cc hrit.cc \
			  model.cc obs.cc twiin.cc twiin_convert.cc nc_file.cc

if ENABLE_GTKMM
   noinst_HEADERS += cross_section.h
   twiin_SOURCES += cross_section.cc
endif

