cmake_minimum_required(VERSION 3.19)
project(SecureDB LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES de_DE en_US)

# Version 10.4.1 vom 09.10.2026
if(ANDROID)
    include(C:/Users/Reinhard/AppData/Local/Android/Sdk/android_openssl/android_openssl.cmake)
else()
    find_package(OpenSSL REQUIRED)
endif()
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(C:/Users/Reinhard/Documents/GitNeu/HelperClasses_D)

qt_add_executable(SecureDB
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h

    Itemtable.cpp Itemtable.h
    item.cpp item.h
    pwdaten1.cpp pwdaten1.h pwdaten1.ui pwdaten2.cpp pwdaten2.h pwdaten2.ui
    ItemName.cpp ItemName.h ItemName.ui
    ItemSelect.cpp ItemSelect.h ItemSelection.ui
    languagedialog.cpp languagedialog.h languagedialog.ui
)
qt_add_resources(SecureDB "images"
                 PREFIX "/d"
                 FILES images/C.png images/Exit.png images/Minus.png images/Noben.png images/Nunten.png images/Open.png images/RedWhite.png images/Save.png images/SecureDB.png images/Test.png)
qt_add_translations(TARGETS SecureDB
                    INCLUDE_DIRECTORIES C:/Users/Reinhard/Documents/GitNeu/SecureDB_D
                    TS_FILES C:/Users/Reinhard/Documents/GitNeu/SecureDB_D/SecureDB_de_DE.ts C:/Users/Reinhard/Documents/GitNeu/SecureDB_D/SecureDB_en_US.ts)

target_link_libraries(SecureDB
    PRIVATE
        Qt::Core
        Qt::Widgets
)
if(ANDROID)
    add_android_openssl_libraries(SecureAndroid)
else()
    target_link_libraries(SecureDB PRIVATE ${OPENSSL_LIBRARIES})
endif()

target_link_libraries(SecureDB PRIVATE C:/Users/Reinhard/Documents/GitNeu/HelperClasses_D/build/Desktop_Qt_6_11_1_MSVC2022_64bit-Release/HelperClasses.lib)

include(GNUInstallDirs)

install(TARGETS SecureDB
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SecureDB
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
