cmake_minimum_required(VERSION 3.16)
project(qt_codex1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "D:/soft/Qt/5.15.2/mingw81_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.h
        src/MainWindow.cpp
        src/core/AppContext.h
        src/core/AppContext.cpp
        src/core/ControlRegistry.h
        src/core/ControlRegistry.cpp
        src/core/ExceptionHandler.h
        src/core/ExceptionHandler.cpp
        src/core/Logger.h
        src/core/Logger.cpp
        src/i18n/TranslationManager.h
        src/i18n/TranslationManager.cpp
        src/ui/ContentStack.h
        src/ui/ContentStack.cpp
        src/ui/ControlPageBase.h
        src/ui/NavigationPanel.h
        src/ui/NavigationPanel.cpp
        src/ui/PaginationWidget.h
        src/ui/PaginationWidget.cpp
        src/pages/ButtonPage.h
        src/pages/ButtonPage.cpp
        src/pages/InputPage.h
        src/pages/InputPage.cpp
        src/pages/ViewPage.h
        src/pages/ViewPage.cpp
        src/pages/TablePage.h
        src/pages/TablePage.cpp
        src/pages/PaginationPage.h
        src/pages/PaginationPage.cpp
        src/pages/DialogPage.h
        src/pages/DialogPage.cpp
        src/pages/FeedbackPage.h
        src/pages/FeedbackPage.cpp
        src/pages/LayoutPage.h
        src/pages/LayoutPage.cpp
        src/pages/MenuToolbarPage.h
        src/pages/MenuToolbarPage.cpp
        src/pages/GraphicsPage.h
        src/pages/GraphicsPage.cpp
        src/pages/ChartPage.h
        src/pages/ChartPage.cpp
        src/pages/StylePage.h
        src/pages/StylePage.cpp
        src/pages/ContainerPage.h
        src/pages/ContainerPage.cpp
        src/pages/AdvancedPage.h
        src/pages/AdvancedPage.cpp
)

add_executable(qt_codex1 ${PROJECT_SOURCES})
target_include_directories(qt_codex1 PRIVATE src)
target_link_libraries(qt_codex1
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

qt5_add_translation(QM_FILES resources/translations/zh_CN.ts)
add_custom_target(translations ALL DEPENDS ${QM_FILES})
add_dependencies(qt_codex1 translations)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/translations")
foreach (QM_FILE ${QM_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QM_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/translations/")
endforeach ()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
