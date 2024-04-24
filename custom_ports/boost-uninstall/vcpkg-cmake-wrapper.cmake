set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
unset(Boost_USE_STATIC_RUNTIME)
set(Boost_NO_BOOST_CMAKE ON)
unset(Boost_USE_STATIC_RUNTIME CACHE)
set(Boost_COMPILER "@BOOST_COMPILER@")

if(DEFINED CMAKE_MODULE_PATH)
    set(Z_VCPKG_BACKUP_BOOST_CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")
endif()
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_LIST_DIR}")
_find_package(${ARGS})
if(DEFINED Z_VCPKG_BACKUP_BOOST_CMAKE_MODULE_PATH)
    set(CMAKE_MODULE_PATH "${Z_VCPKG_BACKUP_BOOST_CMAKE_MODULE_PATH}")
else()
    unset(CMAKE_MODULE_PATH)
endif()