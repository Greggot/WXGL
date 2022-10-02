cmake_minimum_required (VERSION 3.8)
project ("WXGL")
set(CMAKE_CXX_STANDARD 17)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

add_executable(WXGL WIN32)

# User input paths
set(WXPath ... CACHE PATH "Path to WX")
set(GLFWPath ... CACHE PATH "someval")
set(VCversion vc143 CACHE STRING "someval")

# WXGL compile definitions
add_compile_definitions(UNICODE)
add_compile_definitions(_UNICODE)
add_compile_definitions(WXUSINGDLL)
add_compile_definitions(wxMSVC_VERSION_AUTO)
target_compile_definitions(WXGL PRIVATE WXUSINGDLL)
target_compile_definitions(WXGL PRIVATE wxMSVC_VERSION_AUTO)

# WX and GLFW include directories
target_include_directories(WXGL PRIVATE ${CMAKE_SOURCE_DIR}/include)
include_directories(${WXPath}/include)
include_directories(${WXPath}/include/msvc)
include_directories(${GLFWPath}/include)

# Add all source files from project
SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR}/source)
FOREACH(subdir ${SUBDIRS})
    set(fullsub ${CMAKE_SOURCE_DIR}/source/${subdir})
    file(GLOB files "${fullsub}/*.cpp")
    target_sources(WXGL PRIVATE ${files})
ENDFOREACH()
target_sources(WXGL PRIVATE ${CMAKE_SOURCE_DIR}/source/MainFrame.cpp)

# Add external libraries
file(GLOB libraries "${WXPath}/lib/${VCversion}_x64_dll/*.lib")
link_directories(${WXPath}/lib/vc143_x64_dll)
target_link_libraries(WXGL ${libraries})
target_link_libraries(WXGL "${GLFWPath}/lib/glfw3.lib")
target_link_libraries(WXGL "opengl32.lib")
target_link_libraries(WXGL "user32.lib")
target_link_libraries(WXGL "gdi32.lib")
target_link_libraries(WXGL "shell32.lib")

file(GLOB dlls "${WXPath}/lib/${VCversion}_x64_dll/*.dll")
FOREACH(dll ${dlls})
    install(FILES ${dll} DESTINATION ${CMAKE_INSTAL_PREFIX}/bin)
ENDFOREACH()
install(TARGETS WXGL DESTINATION ${CMAKE_INSTAL_PREFIX})
    
