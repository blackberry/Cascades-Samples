#!/bin/bash

function clean_up {

    # Perform program exit housekeeping
    cd "$ORIGDIR"
    rm -fr $TMPDIR 2>&1
}

function error_exit {

    # Display error message and exit
    echo -e "${PROGNAME}: ${1:-"Unknown error"}.  Exiting..." 1>&2
    clean_up
    exit 1
}

trap "clean_up; exit 1" SIGHUP SIGINT SIGTERM

# directories
PROGDIR=$(cd $(dirname $0); pwd)
PROGNAME=$(basename $0)
ORIGDIR=$(pwd)
TMPDIR=$PROGDIR/temp.$PROGNAME.$$.$RANDOM
DOCDIRS="docs"

# validate
if [ "x$1" = "x" ]; then
    error_exit "Usage: ${PROGNAME} branch\nUpdates qdoc in gh-pages branch from specified branch."
fi
if [ "* gh-pages" != "$(git branch | grep -e '* gh-pages' --line-regexp)" ]; then
    error_exit "This command works on the gh-pages branch only.  Do \`git checkout gh-pages' before running this command"
fi

# generate docs
cd $PROGDIR || error_exit "Unable to change directory to $PROGDIR"
git checkout $1 || error_exit "Unable to do \`git checkout' to branch $1"
for a in $DOCDIRS; do
    cd $PROGDIR/$a || error_exit "Unable to change directory to $a"
    qmake || error_exit "qmake command failed in directory $(pwd)\nYou may need to do something like \`export QTDIR=/usr/share/qt4' and \`export QMAKESPEC=$QNX_TARGET/usr/share/qt4/mkspecs/common/qnx'"
    make docs || error_exit "make command failed in directory $(pwd)"
    rm -f Makefile || error_exit "Unable to remove Makefile after making docs"
    # Remove large file that seems to be unnecessary and changes on every build.
    rm -f $PROGDIR/$a/html/*.pageindex || error_exit "Unable to remove *.pageindex after making docs"
    mkdir -p $TMPDIR/$a || error_exit "Unable to create directory"
    mv -f $PROGDIR/$a/html $TMPDIR/$a || error_exit "Unable to move generated docs files"
done

# copy docs to gh-pages branch
cd $PROGDIR || error_exit "Unable to change directory to $PROGDIR"
git checkout gh-pages || error_exit "Unable to do \`git checkout' to branch gh-pages"
for a in $DOCDIRS; do
    rm -fr $a/html/* || error_exit "Unable to remove all generated doc files from $a"
done
cp -fr $TMPDIR/* $PROGDIR || error_exit "Unable to copy generated docs files"

clean_up

echo "Done.  Review any changes to the working tree using \`git status'"

exit
