#  CMakeLists.txt file for the mdC2H6 - example program for the
#  Molecular Dynamics Toolkit (MDTK).
#
#  Copyright (C) 2009, 2013, 2015 Oleksandr Yermolenko
#  <oleksandr.yermolenko@gmail.com>
#
#  This file is part of MDTK, the Molecular Dynamics Toolkit.
#
#  MDTK is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  MDTK is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with MDTK.  If not, see <http://www.gnu.org/licenses/>.
#

SET(MDTK_CurrentTarget "mdC2H6${MDTK_BINARY_SUFFIX}")

include_directories (${MDTK_SOURCE_DIR} ${GSL_INCLUDE_DIRS})

link_directories (${MDTK_BINARY_DIR})

add_executable (${MDTK_CurrentTarget} main.cpp)

target_link_libraries (${MDTK_CurrentTarget} mdtk yaatk ${YAATK_COMPRESSION_LIBRARIES} ${GSL_LIBRARIES})

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    SET_TARGET_PROPERTIES(${MDTK_CurrentTarget} PROPERTIES LINK_FLAGS "-s -static")
  ELSE(WIN32)
    SET_TARGET_PROPERTIES(${MDTK_CurrentTarget} PROPERTIES LINK_FLAGS "-s")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

install(TARGETS ${MDTK_CurrentTarget}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
