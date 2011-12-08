#!/bin/sh

rm -rf autom4te.cache

libtoolize --force --copy || exit 1
aclocal || exit 1
autoheader || exit 1
automake --add-missing --force-missing --copy --foreign || exit 1
autoconf || exit 1
