#!/bin/sh
#
# build a minix 3.x binary package
#
# written by Marco van den Heuvel <blackystardust68@yahoo.com>
#
# this script needs to be run from the top of the vice tree

echo "Generating Minix-3.x binary archive"

# see if we are in the top of the tree
if [ ! -f configure.in ]; then
  cd ../..
  if [ ! -f configure.in ]; then
    echo "please run this script from the base of the VICE directory"
    echo "or from the appropriate build directory"
    exit 1
  fi
fi

curdir=`pwd`
curdirbase=`basename $curdir`
rm -f SDL.build

cd ..

binpackage $curdirbase .

echo Minix-3.x package generated as ../$curdirbase.tar.bz2
