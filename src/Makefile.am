AM_CPPFLAGS        = -I$(top_builddir) -I$(top_srcdir)

noinst_HEADERS  = console.h cross_section.h data.h display.h enhancement.h \
		  hrit.h nc_file.h
bin_PROGRAMS    = twiin-cmd twiin-cc-cmd

if ENABLE_GTKMM
   bin_PROGRAMS += twiin
endif

#twiin_LDADD    	= $(top_builddir)/twiin/libtwiin.la
twiin_SOURCES		= console.cc cross_section.cc data.cc display.cc \
			  enhancement.cc hrit.cc nc_file.cc
#twiin_cmd_LDADD	= $(top_builddir)/twiin/libtwiin.la
twiin_cmd_SOURCES	= main.cc data.cc display.cc nc_file.cc
#twiin_cc_cmd_LDADD	= $(top_builddir)/twiin/libtwiin.la
twiin_cc_cmd_SOURCES	= cross_section_cmd.cc data.cc display.cc nc_file.cc

