function(windows_deploy_qt)
    set(windeployqt "${Qt6_DIR}/../../../bin/windeployqt.exe")
    if (CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
        list(APPEND ARGN --debug)
    else ()
        list(APPEND ARGN --release)
    endif ()
    list(APPEND ARGN --verbose 0)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND "${windeployqt}" "$<TARGET_FILE:${PROJECT_NAME}>" ${ARGN}
            )
endfunction(windows_deploy_qt)


if (WIN32)
    # set package properties
    set(CPACK_GENERATOR WIX ZIP)
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_VENDOR CS2019)
    set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME};PonyPlayer)
    set(CPACK_CREATE_DESKTOP_LINKS PonyPlayer)
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    set(CPACK_ARCHIVE_THREADS 0)
    set(CPACK_WIX_UPGRADE_GUID "7A1B5C35-D464-5239-AF42-172AD856488F") # uuid("PonyPlayer")
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            )
    set(CPACK_RESOURCE_FILE_LICENSE "${PONY_LICENSE}")
    set(CPACK_PACKAGE_ICO "${PONY_ICON}")
    include(CPACK)

    # copy FFmpeg dll
    foreach (lib_name IN LISTS FFMPEG_LIBS)
        file(GLOB lib_source "${FFMPEG_PREFIX_PATH}/bin/${lib_name}*.dll")
        if (NOT lib_source)
            message(FATAL_ERROR "Cannot find: ${lib_name}.")
        endif ()
        get_filename_component(lib_filename "${lib_source}" NAME)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${lib_source}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                )
        install(FILES "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${lib_filename}" DESTINATION bin)
    endforeach ()

    # copy Qt runtime
    windows_deploy_qt(--qmldir "${CMAKE_SOURCE_DIR}/src/view")
    windows_deploy_qt(
            --qmldir "${CMAKE_SOURCE_DIR}/src/view"
            --dir "${CMAKE_CURRENT_BINARY_DIR}/winqt_tmp/"
    )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/winqt_tmp/" DESTINATION bin)

    # copy Assets
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets/filters"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/filters"
            )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets/" DESTINATION bin/assets)
elseif (APPLE)
    set_source_files_properties(${PONY_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
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
elseif (UNIX)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets/filters"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/filters")
    install(TARGETS ${PROJECT_NAME} RUNTIME
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            PUBLIC_HEADER DESTINATION include
            )
    install(FILES ${CMAKE_SOURCE_DIR}/assets/ponyicon.png DESTINATION share/icons/hicolor/96x96/apps/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/ponyicon.png DESTINATION share/icons/)
    install(FILES ${CMAKE_SOURCE_DIR}/assets/ponyicon.ico DESTINATION share/icons/)
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION share/ponyplayer/)
endif ()