#
# Conky, a system monitor, based on torsmo
#
# Please see COPYING for details
#
# Copyright (c) 2005-2021 Brenden Matthews, et. al. (see AUTHORS) All rights
# reserved.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details. You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

set(CPACK_GENERATOR "TGZ;STGZ;DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>=2.10-1), libgcc1 (>= 1:4.4.0)")
set(CPACK_PACKAGE_CONTACT "brenden@diddyinc.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Brenden Matthews")
set(CPACK_PACKAGE_FILE_NAME
    "${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}-${CPU_ARCH}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})

set(CPACK_STRIP_FILES true)

# Version numbering, copyright notice, etc. are taken from the variables defined
# in ../CMakeLists.txt.
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")

if(CPACK_GENERATOR MATCHES "NSIS")
  set(CPACK_NSIS_COMPRESSOR "/FINAL /SOLID lzma")
  set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} ${PROJECT_NAME})
endif(CPACK_GENERATOR MATCHES "NSIS")

# Source package setup. Compile with "make package_source".
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION}-src")
set(CPACK_SOURCE_GENERATOR "TBZ2")
execute_process(COMMAND ${APP_UNAME}
                COMMAND ${APP_AWK} "{print $1}"
                RESULT_VARIABLE RETVAL
                OUTPUT_VARIABLE CPU_ARCH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CPACK_SOURCE_IGNORE_FILES
    # Exclude backup files generated by text editors.
    "~$"
    # Git dir/files.
    "\\\\.git.*$"
    # vim swap files
    "\\\\..*\\\\.swp$"
    # Build dir.
    "build.*$")

# Enable CPack
include(CPack)
