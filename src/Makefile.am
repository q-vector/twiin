INCLUDES        = -I$(top_builddir) -I$(top_srcdir)

noinst_HEADERS  = console.h data.h display.h nc_file.h
bin_PROGRAMS    = twiin twiin-cmd

#twiin_LDADD    	= $(top_builddir)/twiin/libtwiin.la
twiin_SOURCES		= console.cc data.cc display.cc snd_axf.cc
#tinfox_wmo_LDADD	= $(top_builddir)/tinfox/libtinfox.la
twiin_cmd_SOURCES	= main.cc data.cc display.cc snd_axf.cc

