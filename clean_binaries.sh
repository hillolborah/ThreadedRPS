#!/bin/bash

echo "Compiled Binaries : "

find . -maxdepth 1 -type f -executable ! -name "*.sh" -print

read -p "Delete Binaries? (y/n): " choice
if [[ "$choice" == [Yy] ]]; then
    find . -maxdepth 1 -type f -executable ! -name "*.sh" -delete
    echo "Binaries Deleted."
else
    echo "Deletions cancelled."
fi
