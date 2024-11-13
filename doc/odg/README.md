# Description

This directory contains the illustrations of the documentation in .odg format
and .pdf format for use in the LaTex file.

The .odg format can be edited in [LibreOffice](https://www.libreoffice.org/) 
Draw.

The [medium-map.odg](./medium-map.odg) and [map-hard.odg](./map-hard.odg) 
files are the .odg versions of the  maps for the *medium* and *hard* levels. 
This was helpful to visually identify the data to be entered into the 
[shapes.h](../../Ardymo/shapes.h) file.

The *hard* level has been created as an SVG file with [Boxy SVG](boxy-svg.com).
The program [svg-parser.py](../../tools/svg-parser.py) is used to convert 
the svg file into `obstacle_t` values which can be directly used in 
[shapes.h](../../Ardymo/shapes.h).
