cmake_minimum_required(VERSION 3.5)


project(asciiplayer,
        VERSION 0.0.1
        DESCRIPTION "CLI player that render videos in ASCII"
        LANGUAGES CXX)

add_executable(main main.cpp)

find_package(PkgConfig REQUIRED)

install(TARGETS main RUNTIME DESTINATION bin)

pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)


target_link_libraries(main PkgConfig::LIBAV)

