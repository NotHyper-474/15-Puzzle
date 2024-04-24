set(directory_list "")

function(add_subdirectories dir result)
    file(GLOB subdirectories RELATIVE ${dir} ${dir}/*)
    
    foreach(subdir ${subdirectories})
        if(IS_DIRECTORY ${dir}/${subdir})
            list(APPEND directory_list ${dir}/${subdir})
            add_subdirectories(${dir}/${subdir} ${result})
        endif()
    endforeach()
    set(${result} ${directory_list} PARENT_SCOPE)
endfunction()