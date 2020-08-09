#!/bin/sh
# $Id: autogen.sh 931 2007-08-30 06:25:19Z mirrorbox $

# autogen.sh
#
# optional $1 = optional directory containing build tree or svn working copy

AUTOCONF=${AUTOCONF:-autoconf}
AUTOMAKE=${AUTOMAKE:-automake}
ACLOCAL=${ACLOCAL:-aclocal}
AUTOHEADER=${AUTOHEADER:-autoheader}
LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}

# identify svn revision, if an svn working copy
if test "$1" != "" && test -d "$1/.svn"; then
    revision=`LC_ALL=C svn info $1 | awk '/^Revision: / {printf "%05d\n", $2}'`;
elif test -d ".svn"; then
    revision=`LC_ALL=C svn info | awk '/^Revision: / {printf "%05d\n", $2}'`; 
else
    revision="NONE"; fi

# generate configure.ac with substituted svn revision
sed -e "s/@REVISION@/${revision}/g" < "configure.ac.in" > "configure.ac"

echo Running $ACLOCAL -I m4 ... && $ACLOCAL -I m4
echo Running $LIBTOOLIZE --force --copy ... && $LIBTOOLIZE --force --copy
echo Running $AUTOHEADER ... && $AUTOHEADER
echo Running $AUTOMAKE --add-missing --copy --gnu ... && $AUTOMAKE --add-missing --copy --gnu
echo Running $AUTOCONF ... && $AUTOCONF
