#!/bin/bash

if [ $# -ne 1 ]; then
    echo $0: usage: sh dev-deploy.sh /path/to/work
    exit 1
fi

echo ""
echo "\t\t--- Now deploying eVias EOS Smart Contracts ---"
echo ""

path=$1

mkdir -p $1/
cp -R ./* $1/

exit 0
