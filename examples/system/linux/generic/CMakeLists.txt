collector_list  (_list PROJECT_INC_DIRS)
include_directories (${_list} ${CMAKE_CURRENT_SOURCE_DIR})

collector_list  (_list PROJECT_LIB_DIRS)
link_directories (${_list})

collector_list (_deps PROJECT_LIB_DEPS)

set (_src_common ${CMAKE_CURRENT_SOURCE_DIR}/sys_init.c)
foreach (_app shm_demo)
  set (_src ${CMAKE_CURRENT_SOURCE_DIR}/${_app}.c)
  list(APPEND _src ${_src_common})
  if (WITH_SHARED_LIB)
    add_executable (${_app}-share ${_src})
    if (PROJECT_EC_FLAGS)
      string(REPLACE " " ";" _ec_flgs ${PROJECT_EC_FLAGS})
      target_compile_options (${_app}-share PUBLIC ${_ec_flgs})
    endif (PROJECT_EC_FLAGS)
    target_link_libraries (${_app}-share ${PROJECT_NAME}-shared ${_deps})
    install (TARGETS ${_app}-share RUNTIME DESTINATION bin)
    add_dependencies (${_app}-share ${PROJECT_NAME}-shared)
  endif (WITH_SHARED_LIB)

  if (WITH_STATIC_LIB)
    if (${PROJECT_SYSTEM} STREQUAL "linux")
        add_executable (${_app}-static ${_src})
        if (PROJECT_EC_FLAGS)
          string(REPLACE " " ";" _ec_flgs ${PROJECT_EC_FLAGS})
          target_compile_options (${_app}-static PUBLIC ${_ec_flgs})
        endif (PROJECT_EC_FLAGS)
        target_link_libraries (${_app}-static ${PROJECT_NAME}-static ${_deps})
        install (TARGETS ${_app}-static RUNTIME DESTINATION bin)
    endif (${PROJECT_SYSTEM} STREQUAL "linux")
  endif (WITH_STATIC_LIB)
endforeach (_app)

# vim: expandtab:ts=2:sw=2:smartindent

