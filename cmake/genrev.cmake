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

execute_process(
  COMMAND hg id -n
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE rev_id_str
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

execute_process(
  COMMAND hg id -i
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE rev_hash_str
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

if(EXISTS ${CMAKE_SOURCE_DIR}/.hg_archival.txt)
  file(READ
    ${CMAKE_SOURCE_DIR}/.hg_archival.txt rev_hash_str
    LIMIT 10
    OFFSET 7
    NEWLINE_CONSUME
  )
  string(STRIP ${rev_hash_str} rev_hash_str)
  set(rev_id_str "YOUR MYTHCORE IS OUT OF COMMUNITY SUPPORT, PLEASE ASK SUN FOR DETAILS")
  set(rev_id "0")
  set(rev_hash ${rev_hash_str})
endif()

# Last minute check - ensure that we have a proper revision
# If everything above fails (means the user has erased the mercurial revisional control directory, or runs archive and erased their .hg_archival.txt)
if(NOT rev_id_str)
  message("")
  message(STATUS "WARNING - No revision-information found - have you been tampering with the sources?")

  # Ok, since we have no valid ways of finding/setting the revision, let's force some defaults
  set(rev_hash_str "Archive")
  set(rev_hash "0")
  set(rev_id_str "0")
  set(rev_id "0")
endif()

# Strip off excess strings (shows when the source is actually modified)
if(NOT rev_id_str MATCHES "Archive")
  string(REPLACE "+" "" rev_id ${rev_id_str})
endif()
string(REPLACE "+" "" rev_hash ${rev_hash_str})

# Its not set during initial run
if(NOT BUILDDIR)
  set(BUILDDIR ${CMAKE_BINARY_DIR})
endif()

# Create the actual revision.h file from the above params
if(NOT "${rev_id_cached}" MATCHES "${rev_id_str}")
  configure_file(
    "${CMAKE_SOURCE_DIR}/revision.h.in.cmake"
    "${BUILDDIR}/revision.h"
    @ONLY
  )
  set(rev_id_cached "${rev_id_str}" CACHE INTERNAL "Cached revision ID")
endif()