ExternalProject_Add(cereal
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/cereal
  GIT_REPOSITORY "https://github.com/USCiLab/cereal"
  GIT_TAG "51cbda5f30e56c801c07fe3d3aba5d7fb9e6cca4"
  #DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug
             -DJUST_INSTALL_CEREAL=ON
             -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}
             -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
)
