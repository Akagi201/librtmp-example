cmake_minimum_required(VERSION 2.8.4)

project(librtmp_example)

set(SOURCE_FILES1 rtmp_receive.c)
#set(SOURCE_FILES2 send_flv_over_rtmp.c)
#set(SOURCE_FILES3 send_h264_over_rtmp.c)

include_directories(${PROJECT_SOURCE_DIR}/deps "/usr/local/include")

link_directories("/usr/local/lib")

add_executable(rtmp_receive ${SOURCE_FILES1})
#add_executable(send_flv_over_rtmp ${SOURCE_FILES2})
#add_executable(send_h264_over_rtmp ${SOURCE_FILES3})

target_link_libraries(rtmp_receive "librtmp.a")
#target_link_libraries(send_flv_over_rtmp "librtmp.a")
#target_link_libraries(send_h264_over_rtmp "librtmp.a")
