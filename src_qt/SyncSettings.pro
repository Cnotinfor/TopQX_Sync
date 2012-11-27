# ###################################################################
#  Project common settings
# ###################################################################
TARGET = SyncManager
TEMPLATE = lib

QT += network xml

DEFINES += SYNC_MANAGER_LIB

# DEBUG
CONFIG( debug, debug|release ) {
	TARGET = $${TARGET}_d
	BUILD_NAME = debug
}
#RELEASE
CONFIG( release, debug|release ) {
	BUILD_NAME = release
}
