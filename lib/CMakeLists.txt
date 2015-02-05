
add_library(libGPCGUIRenderer INTERFACE)

target_include_directories(libGPCGUIRenderer INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
