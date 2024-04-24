SET(_PF86 "PROGRAMFILES(X86)")
set(SDL2IMG_SEARCH_PATHS
	${SDL2IMG_ROOT_DIR}
	./lib/SDL2_image
	lib/SDL2_image
	$ENV{PROGRAMFILES}/SDL2_image
	"$ENV{_PF86}/SDL2_image"
	~/Library/Frameworks/	# MAC
	/Library/Frameworks/	# Also MAC
	/usr/local/
	/usr
	/opt
	/sw
	/opt/local
	/opt/csw
)

find_path( SDL2IMG_INCLUDE_DIRS
	NAMES
	SDL_image.h SDL_image/SDL_image.h
	PATHS
	${SDL2IMG_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where SDL_image.h resides"
)

FIND_LIBRARY( SDL2IMG_LIBRARIES
	NAMES
		SDL2_image
	PATHS
		${SDL2IMG_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x${ARCH_NUMBER}
		lib/${ARCH_NUMBER}
	DOC
		"The SDL2_image library"
)

# Check if we found it and send a message telling so
IF ( SDL2IMG_INCLUDE_DIRS AND SDL2IMG_LIBRARIES )
	MESSAGE(STATUS "Looking for SDL2_image - found")
	SET( SDL2_IMG_FOUND TRUE )
ELSE ( SDL2IMG_INCLUDE_DIRS AND SDL2IMG_LIBRARIES )
	MESSAGE(STATUS "Looking for SDL2_image - not found")
	SET( SDL2_TTF_FOUND FALSE )
ENDIF ( SDL2IMG_INCLUDE_DIRS AND SDL2IMG_LIBRARIES )