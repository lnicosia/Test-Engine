################################################################################
# File: UseCcache.cmake
# Created Date: 15 Dec 2021
# Author: Nicolas IRAGNE (nicoragne@hotmail.fr)
# -----
# Sets the RULE_LAUNCH_COMPILE CMake property to use ccache.
# -----
################################################################################

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message("Found ccache: ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else()
    message(WARNING "Could NOT find ccache!")
endif()