SET(_PF86 "PROGRAMFILES(X86)")
set(SDL2_SEARCH_PATHS
	${SDL2_ROOT_DIR}
	./lib/SDL2
	lib/SDL2
	$ENV{PROGRAMFILES}/SDL2
	"$ENV{_PF86}/SDL2"
	~/Library/Frameworks/	# MAC
	/Library/Frameworks/	# Also MAC
	/usr/local/
	/usr
	/opt
	/sw
	/opt/local
	/opt/csw
)

find_path( SDL2_INCLUDE_DIRS
	NAMES
	SDL.h SDL2/SDL.h
	PATHS
	${SDL2_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where SDL.h resides"
)


FIND_LIBRARY( SDL2_LIBRARIES
	NAMES
		SDL2 SDL2main
	PATHS
		${SDL2_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x${ARCH_NUMBER}
		lib/${ARCH_NUMBER}
	DOC
		"The SDL2 library"
)

# Check if we found it and send a message telling so
IF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
	SET( SDL2_FOUND TRUE )
	MESSAGE(STATUS "Looking for SDL2 - found")
ELSE ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
	SET( SDL2_FOUND FALSE )
	MESSAGE(STATUS "Looking for SDL2 - not found")
ENDIF ( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )