#!/bin/bash

mkdir -p ~/.blender/2.61/scripts/addons

for file in io_export_ogreDotScene.py
do
	ln -s $(readlink -f $file) ~/.blender/2.61/scripts/addons/$file
done

