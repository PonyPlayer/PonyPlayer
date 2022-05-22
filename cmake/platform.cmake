function(windows_copy_dependency EXTRA_ARG)
    set(windeployqt "${Qt6_DIR}/../../../bin/windeployqt.exe")
    if(CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
        set(WINDEPLOYQT_ARG --debug)
    else()
        set(WINDEPLOYQT_ARG --release)
    endif()
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND
            "${windeployqt}" "$<TARGET_FILE:${PROJECT_NAME}>"
            ${WINDEPLOYQT_ARG} ${EXTRA_ARG}
            --verbose 0
            )
endfunction(windows_copy_dependency)

function(windows_deploy_qt EXTRA_ARGS)
    set(windeployqt "${Qt6_DIR}/../../../bin/windeployqt.exe")
    if(CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
        set(WINDEPLOYQT_ARGS --debug)
    else()
        set(WINDEPLOYQT_ARGS --release)
    endif()
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND
            "${windeployqt}" "$<TARGET_FILE:${PROJECT_NAME}>"
            ${WINDEPLOYQT_ARGS}
            ${EXTRA_ARGS}
            --qmldir "${CMAKE_SOURCE_DIR}/src/view"
            --verbose 0
            )
endfunction(windows_deploy_qt)



function(linux_setup_install_flag_before_add_executable)
    if (UNIX)
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    endif ()
endfunction()


function(linux_setup_deployment)
    if(UNIX)
        install(TARGETS ${PROJECT_NAME} RUNTIME
                LIBRARY DESTINATION usr/lib
                ARCHIVE DESTINATION usr/lib
                RUNTIME DESTINATION usr/bin
                PUBLIC_HEADER DESTINATION usr/include
                )
        install(FILES ${CMAKE_SOURCE_DIR}/assets/ponyplayer-icon.png DESTINATION usr/share/icons/hicolor/96x96/apps/)
    endif()
endfunction()

function(macos_setup_deployment)
    if (APPLE)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                MACOSX_BUNDLE_GUI_IDENTIFIER scut.ponyplayer
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                MACOSX_BUNDLE TRUE
                WIN32_EXECUTABLE TRUE
                )
    endif()
endfunction()

if (WIN32)
    # copy ffmpeg library
    foreach(lib_name IN LISTS FFMPEG_LIBS)
        file(GLOB lib_source "${FFMPEG_PREFIX_PATH}/bin/${lib_name}*.dll")
        if(NOT lib_source)
            message(FATAL_ERROR "Cannot find: ${lib_name}.")
        endif()
        get_filename_component(lib_filename "${lib_source}" NAME)
        add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${lib_source}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                )
        install(FILES "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${lib_filename}" DESTINATION bin)
    endforeach()

    # copy qt library
    windows_deploy_qt("")

    # copy assets
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets/filters"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/filters"
            )

    # set package properties
    set(CPACK_GENERATOR WIX ZIP)
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_VENDOR CS2019)
    set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME};PonyPlayer)
    set(CPACK_CREATE_DESKTOP_LINKS PonyPlayer)
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME}/${PROJECT_VERSION})
#    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE.txt)
#    set(CPACK_PACKAGE_ICO ${CMAKE_SOURCE_DIR}/assets/ponyplayer-icon.png)

    set(CPACK_ARCHIVE_THREADS 0)
    set(CPACK_WIX_UPGRADE_GUID "7A1B5C35-D464-5239-AF42-172AD856488F")


    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            )

    windows_deploy_qt("--dir \"${CMAKE_CURRENT_BINARY_DIR}/winqt_tmp/\" --release")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/winqt_tmp/" DESTINATION bin)
    include(CPack)

elseif(UNIX)
    if (APPLE)
        set_source_files_properties(${PONY_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
#        set_source_files_properties(${ASSETS_FILTER} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets/filters"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/filters"
                )
        set_target_properties(${PROJECT_NAME} PROPERTIES
                MACOSX_BUNDLE_GUI_IDENTIFIER scut.ponyplayer
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                MACOSX_BUNDLE TRUE
                MACOSX_BUNDLE_ICON_FILE ponyicon
                WIN32_EXECUTABLE TRUE
                )
    endif()
endif()
