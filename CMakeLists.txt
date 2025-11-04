cmake_minimum_required(VERSION 3.16)

project(ContactBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/AddContactDialog.cpp
    model/ContactModel.cpp
)

set(HEADERS
    src/mainwindow.h
    src/AddContactDialog.cpp
    model/ContactModel.h
)
set(FORMS
    src/mainwindow.ui
    src/AddContactDialog.ui
    )
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_SOURCE_DIR}/model)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets)
