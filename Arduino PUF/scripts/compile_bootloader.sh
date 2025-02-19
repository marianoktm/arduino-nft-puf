#! /bin/bash

# Define the folder path you want to delete
folder_to_delete="../build"

# Check if the folder exists
if [ -d "$folder_to_delete" ]; then
    # Use rm with the -r (recursive) and -f (force) options to delete the folder
    rm -rf "$folder_to_delete"
    echo "Folder '$folder_to_delete' deleted."
else
    echo "Folder '$folder_to_delete' does not exist."
fi

source ../buildenv/bin/activate;
cd ..;
scons;
ls -lh build/bootloader;
# enrollment/build/enroller