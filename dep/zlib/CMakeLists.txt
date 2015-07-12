#
# Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
#
# Myth Project's source is based on the Trinity Project source, you can find the
# link to that easily in Trinity Copyrights. Myth Project is a private community.
# To get access, you either have to donate or pass a developer test.
# You may not share Myth Project's sources! For personal use only.
#

SET(zlib_STAT_SRCS
  adler32.c
  compress.c
  crc32.c
  deflate.c
  infback.c
  inffast.c
  inflate.c
  inftrees.c
  trees.c
  uncompr.c
  zutil.c
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(zlib STATIC ${zlib_STAT_SRCS})