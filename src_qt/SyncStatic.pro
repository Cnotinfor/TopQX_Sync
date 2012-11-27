# ###################################################################
#  Project common settings
# ###################################################################
include( "AuthenticationSettings.pro" )

# ###################################################################
#  Project custom settings
# ###################################################################

CONFIG += staticlib create_prl
DESTDIR = ../lib/static

# ####################################################################
#  Source Files
# ####################################################################

include( "src.pri" )
