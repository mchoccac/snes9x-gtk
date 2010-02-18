#!/bin/sh

glib-gettextize -f -c
intltoolize --copy --force --automake
aclocal
automake --add-missing
autoconf
