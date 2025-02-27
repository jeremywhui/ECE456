#!/usr/bin/bash
#
# go to a level above your project directory.
# usage: maketar.sh <dir-name>
#

# Check arguments

if [ $# -ne 1 ]
then 
    echo "This script needs your working directory name as an argument"
    exit
fi

# output file name is <dir-name>.tar.gz. 
tar czf $1.tar.gz $1
