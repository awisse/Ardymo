# Description

The *svg-parser.py* programs transforms an SVG file produced by 
[Boxy SVG](https://boxy-svg.com/) into `obstacle_t` values which can be 
directly inserted into *shapes.h*.

* The only handled elements are

    * `circle` (always an `ellipsis` Boxy SVG),

    * `polyline` (split into `line_t` segments),

    * `line`,

    * `rect`

* Consequently, no bezier curves, arcs, fonts, etc will be transformed.

* The user must manually place the target as the first element of the level
  array.

