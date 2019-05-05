ExternalProject_Add(catch2
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/catch2
  #GIT_REPOSITORY "https://github.com/catchorg/Catch2"
  #GIT_TAG "979bbf03bb00bc55ca09783791b5091a2247df68"
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug
             -DCATCH_BUILD_TESTING=OFF
             -DCATCH_INSTALL_DOCS=OFF
             -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}
             -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)