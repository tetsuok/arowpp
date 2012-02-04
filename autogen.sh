#!/bin/sh

set -ex
rm -rf autom4te.cache
aclocal --force -I m4
autoconf -f -W all,no-obsolete
autoheader -f -W all
automake -a -c -f -W all

rm -rf autom4te.cache
exit 0
