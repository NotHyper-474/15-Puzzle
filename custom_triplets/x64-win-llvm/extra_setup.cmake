if(DEFINED CURRENT_PORT_DIR AND 
   DEFINED CURRENT_PACKAGES_DIR AND 
   DEFINED CURRENT_BUILDTREES_DIR AND
   DEFINED TARGET_TRIPLET AND
   DEFINED TARGET_TRIPLET_FILE AND
   DEFINED VCPKG_BASE_VERSION AND
   DEFINED VCPKG_MANIFEST_INSTALL AND
   DEFINED CMD)
    # These means we are within vcpkg and not somewhere else.
    # set(ENV{PATH} "${CMAKE_CURRENT_LIST_DIR}/wrappers;$ENV{PATH}")

    if (DEFINED ENV{ProgramW6432})
        file(TO_CMAKE_PATH "$ENV{ProgramW6432}" PROG_ROOT)
    else()
        file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" PROG_ROOT)
    endif()
    if (DEFINED ENV{LLVMInstallDir})
        file(TO_CMAKE_PATH "$ENV{LLVMInstallDir}/bin" POSSIBLE_LLVM_BIN_DIR)
    else()
        file(TO_CMAKE_PATH "${PROG_ROOT}/LLVM/bin" POSSIBLE_LLVM_BIN_DIR)
    endif()

    find_program(CLANG-CL_EXECUTBALE NAMES "clang-cl" "clang-cl.exe" PATHS "${POSSIBLE_LLVM_BIN_DIR}"
                                                                           ENV LLVMInstallDir
                                                                     PATH_SUFFIXES "bin"
                                                                     NO_DEFAULT_PATH)
    if(NOT DEFINED ENV{LLVMInstallDir} AND NOT DEFINED ENV{LLVMToolsVersion})
        # Search for clang-cl using cmake default search paths. This should find VS installed clang-cl if it is installed 
        find_program(CLANG-CL_EXECUTBALE NAMES "clang-cl" "clang-cl.exe")
    endif()
    if(NOT CLANG-CL_EXECUTBALE)
        message(FATAL_ERROR "Unable to find LLVM installation. Please define environment variable 'LLVMInstallDir' and 'LLVMToolsVersion'")
    endif()
    get_filename_component(LLVM_BIN_DIR "${CLANG-CL_EXECUTBALE}" DIRECTORY)
    set(LLVM_PATH_BACKUP "$ENV{PATH}")
    set(ENV{PATH} "${LLVM_BIN_DIR};$ENV{PATH}")
    if(CMAKE_PARENT_LIST_FILE MATCHES "-san(\\\.|-)" AND NOT PORT MATCHES "(icu)") # icu is building and executable calling the linker with hardcoded linker flags -> so no sanitizer
        list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS 
                    "-DVCPKG_USE_SANITIZERS:BOOL=TRUE"
            )
        file(READ "${CURRENT_PORT_DIR}/portfile.cmake" port_contents)
        if(NOT PORT MATCHES "(openssl|boost|libpq)" AND NOT port_contents MATCHES "(vcpkg_configure_meson|_msbuild|_nmake)")
            list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS 
                        "-DVCPKG_USE_COMPILER_FOR_LINKAGE:BOOL=TRUE"
                )
        else()
            message(STATUS "Found unsupported portfile. Deactivating linkage via compiler")
        endif()
        if(NOT DEFINED ENV{LLVMToolsVersion})
            file(GLOB clang_ver_path LIST_DIRECTORIES true "${LLVM_BIN_DIR}/../lib/clang/*") # This should only contain a single folder
        else()
            set(clang_ver_path "${LLVM_BIN_DIR}/../lib/clang/$ENV{LLVMToolsVersion}")
        endif()
        cmake_path(NORMAL_PATH clang_ver_path)
        set(ENV{PATH} "${clang_ver_path}/lib/windows;$ENV{PATH}")
        set(ENV{LINK} "/LIBPATH:\"${clang_ver_path}/lib/windows\"")
        #set(ENV{PATH} "$ENV{VCToolsInstallDir}/bin/Hostx64/${VCPKG_TARGET_ARCHITECTURE};$ENV{PATH}") # Probably need to remove the VS sanitizer libs so they won't get picked up by accident!
    endif()
else()
    # This is just the detect compiler run!
    if(CMAKE_PARENT_LIST_FILE MATCHES "-san(\\\.|-)")
        list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS 
                    "-DVCPKG_USE_SANITIZERS:BOOL=TRUE"
                    "-DVCPKG_USE_COMPILER_FOR_LINKAGE:BOOL=TRUE"
            )
    endif()
endif()

list(APPEND VCPKG_CMAKE_CONFIGURE_OPTIONS 
            "-DCMAKE_POLICY_DEFAULT_CMP0012=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0056=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0091=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0092=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0117=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0126=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0128=NEW"
            "-DCMAKE_POLICY_DEFAULT_CMP0137=NEW"
    )
