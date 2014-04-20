#
# Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
#
# Myth Project's source is based on the Trinity Project source, you can find the
# link to that easily in Trinity Copyrights. Myth Project is a private community.
# To get access, you either have to donate or pass a developer test.
# You can't share Myth Project's sources! Only for personal use.
#
# Set build-directive (used in core to tell which buildtype we used)
add_definitions(-D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}")

if( WITH_WARNINGS )
  add_definitions(-w1)
  message(STATUS "ICC All warnings enabled")
else()
  add_definitions(-w)
  message(STATUS "ICC: All warnings disabled")
endif()

if( WITH_COREDEBUG )
  add_definitions(-g)
  message(STATUS "ICC: Debug-flag set (-g)")
endif()