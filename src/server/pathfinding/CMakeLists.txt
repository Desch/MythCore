#
# Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
#
# Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
#
# Myth Project's source is based on the Trinity Project source, you can find the
# link to that easily in Trinity Copyrights. Myth Project is a private community.
# To get access, you either have to donate or pass a developer test.
# You may not share Myth Project's sources! For personal use only.
#

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

file(GLOB_RECURSE sources_Detour detour/*.cpp detour/*.h)
file(GLOB_RECURSE sources_Recast recast/*.cpp recast/*.h)
file(GLOB sources_localdir *.cpp *.h)

if( USE_COREPCH AND MSVC )
  set(pathfinding_STAT_SRCS
    PrecompiledHeaders/pathfindingPCH.cpp
    PrecompiledHeaders/pathfindingPCH.h
  )
endif()

set(pathfinding_STAT_SRCS
  ${pathfinding_STAT_SRCS}
  ${sources_Detour}
  ${sources_Recast}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/detour
  ${CMAKE_CURRENT_SOURCE_DIR}/recast
  ${CMAKE_BINARY_DIR}
  ${ACE_INCLUDE_DIR}
)

add_library(pathfinding STATIC ${pathfinding_STAT_SRCS})

# Generate precompiled header
if( USE_COREPCH )
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_precompiled_header(pathfinding ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/pathfindingPCH.h)
  elseif(MSVC)
    add_native_precompiled_header(pathfinding ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/pathfindingPCH)
  endif()
endif()

