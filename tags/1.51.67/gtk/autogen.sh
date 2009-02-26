#!/bin/sh

glib-gettextize -f -c
intltoolize --copy --force --automake
aclocal
autoconf
automake --add-missing
