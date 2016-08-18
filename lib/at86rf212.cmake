# Libmpu9250 cmake include file

# Add library includes
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Add project sources
set(LIBMPU9250_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/at86rf212.c
)

# Create library
add_library(at86rf212 ${LIBMPU9250_SOURCES})

set(OPTIONAL_LIBS at86rf212 ${OPTIONAL_LIBS})
