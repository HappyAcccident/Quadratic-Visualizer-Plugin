set(CPM_DOWNLOAD_VERSION 0.42.0)
set(CPM_HASH_SUM "CPM_HASH_SUM_PLACEHOLDER")

set(CPM_DOWNLOAD_LOCATION "${LIB_DIR}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

file(DOWNLOAD
     https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
     ${CPM_DOWNLOAD_LOCATION}
)

include(${CPM_DOWNLOAD_LOCATION})