#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="mkBiB"

(test -f $srcdir/configure.ac) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level gnome-dictionary directory"
    exit 1
}

(test -f $srcdir/build-aux/ylwrap) || {
     autoreconf --install 
}

GNOMEDOC=`which yelp-build`
 if test -z $GNOMEDOC; then
echo "*** The tools to build the documentation are not found,"
echo " please intall the yelp-tool package ***"
exit 1
fi

which gnome-autogen.sh || {
    echo "You need to install gnome-common package"
    exit 1
}

USE_GNOME2_MACROS=1 . gnome-autogen.sh
