set(TARGET_NAME core)

add_library(${TARGET_NAME}
  byte_buffer2.cpp
  BootRecord.cpp
  FAT.cpp
  FileSystem.cpp
  INode.cpp
  LFNNode.cpp
)

target_include_directories (${TARGET_NAME}
  PUBLIC ../include
)

install (TARGETS ${TARGET_NAME}
  ARCHIVE DESTINATION lib/
)
