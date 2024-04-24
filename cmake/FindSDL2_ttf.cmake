SET(_PF86 "PROGRAMFILES(X86)")
set(SDL2_TTF_SEARCH_PATHS
	${SDL2_TTF_ROOT_DIR}
	./lib/SDL2_ttf
	lib/SDL2_ttf
	$ENV{PROGRAMFILES}/SDL2_ttf
	"$ENV{_PF86}/SDL2_ttf"
	~/Library/Frameworks/	# MAC
	/Library/Frameworks/	# Also MAC
	/usr/local/
	/usr
	/opt
	/sw
	/opt/local
	/opt/csw
)

find_path( SDL2_TTF_INCLUDE_DIRS
	NAMES
	SDL_ttf.h SDL2_ttf/SDL_ttf.h
	PATHS
	${SDL2_TTF_SEARCH_PATHS}
	PATH_SUFFIXES
		include
	DOC
		"The directory where SDL_ttf.h resides"
)


FIND_LIBRARY( SDL2_TTF_LIBRARIES
	NAMES
		SDL2_ttf
	PATHS
		${SDL2_TTF_SEARCH_PATHS}
	PATH_SUFFIXES
		lib
		lib64
		lib/x${ARCH_NUMBER}
		lib/${ARCH_NUMBER}
	DOC
		"The SDL2 library"
)

# Check if we found it and send a message telling so
IF ( SDL2_TTF_INCLUDE_DIRS AND SDL2_TTF_LIBRARIES )
	SET( SDL2_TTF_FOUND TRUE )
	MESSAGE(STATUS "Looking for SDL2_ttf - found")
ELSE ( SDL2_TTF_INCLUDE_DIRS AND SDL2_TTF_LIBRARIES )
	SET( SDL2_TTF_FOUND FALSE )
	MESSAGE(STATUS "Looking for SDL2_ttf - not found")
ENDIF ( SDL2_TTF_INCLUDE_DIRS AND SDL2_TTF_LIBRARIES )