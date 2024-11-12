#!/usr/bin/python3
"""
Parse an svg file.
Convert geometric objects "ellipse", "rect", line and "polyline" to Ardymo
obstacle_t objects. Send the resulting array to stdout.
"""
import binascii
import math
import re
import struct
import sys
import xml.etree.ElementTree as elTree

SVG_NS = {'svg' : 'http://www.w3.org/2000/svg'}
SHAPES = ['polyline', 'ellipse', 'rect', 'line']
EPSILON = 1E-6

xmlns = re.compile(r'\{.*\}')

BORDER_FMT = '{{BORDER, {}, {}, {}, {}, 0x01}},'
CIRCLE_FMT = '{{CIRCLE, {}, {}, {}}},'
LINE_FMT = '{{LINE, {:.1f}, {:.1f}, {:.2f}, {:.0f}, 0x01}},'
RECTANGLE_FMT = '{{RECTANGLE, {:.1f}, {:.1f}, {:.1f}, {:.0f}, 0x{:s}}},'

# Auxiliary functions
def get_style_dict(style_str):
    """
    Extract the elements of the "style" attribute as a dictionary
    """
    style_elements = [s.strip() for s in style_str.split(';')]
    d = dict(tuple([s.strip() for s in e.split(':')])
             for e in style_elements if e)
    return d

def get_rect_origin(x: float, y: float, width: float, height: float,
                    transform_origin: str, matrix: list):
    """
    The rectangle origin is the top right corner for Ardymo.
    The rotation is clockwise with respect to the origin.
    This function computes the coordinates of the rectangle origin
    from the parameters of the svg object.
    """
    def to_pct(pct_str):
        return float(pct_str[:-1]) / 100.0

    # Parse transform-origin
    # Two elements:
    origin_str = [s.strip() for s in transform_origin.split()]
    if origin_str[0].endswith('%'):
        transform_origin = (to_pct(origin_str[0]) * width,
                            to_pct(origin_str[1]) * height)
    else: # px extension
        transform_origin = (float(origin_str[0][:-2]),
                            float(origin_str[1][:-2]))

    # Top right (+width) corner transformed
    x0, y0 = x + width - transform_origin[0], y - transform_origin[1]
    origin_x = matrix[0] * x0 + matrix[2] * y0 \
        + transform_origin[0] + matrix[4]
    origin_y = matrix[1] * x0 + matrix[3] * y0 \
        + transform_origin[1] + matrix[5]

    return origin_x, origin_y

def get_angle(matrix: list):
    """
    `matrix` is a two dimensional unitary rotation matrix
     of the format (cos(angle), sin(angle), -sin(angle), cos(angle))
     We transform this matrix back into an angle between -180 and 180
     degrees.
    """
    if matrix[1] >= 0:
        angle = math.acos(matrix[0])
    else:
        angle = -math.acos(matrix[0])
    angle = math.degrees(angle)
    return angle

def to_ieee754(x: float):
    """
    Transform a float to a IEEE-754 32-bit hexadecimal representation.
    """
    b = struct.pack('>f', x)
    bytes_str = binascii.hexlify(b).decode()
    return bytes_str

def circle(cx: float, cy: float, rx: float, ry: float):
    """
    The circle_t obstacle from svg ellipsis parameters
    """
    return CIRCLE_FMT.format(cx, cy, rx)

def border(viewBox: str):
    """
    Prepare "BORDER_FMT" obstacles, which are `line_t`
    from viewBox coordinates.
    """
    coords = viewBox.split()
    border_lines = []
    border_lines.append(BORDER_FMT.format(coords[0], coords[1],
                                          coords[2], -90))
    border_lines.append(BORDER_FMT.format(coords[0], coords[1],
                                          coords[3], 0))
    border_lines.append(BORDER_FMT.format(coords[0], coords[3],
                                          coords[2], -90))
    border_lines.append(BORDER_FMT.format(coords[2], coords[1],
                                          coords[3], 0))
    return border_lines

def line(x0: float, y0: float, x1: float, y1: float):
    """
    Transform a line segment from its two endpoints to a `line_t` struct
    with an origin, a length and an angle.
    """
    dx = x1 - x0
    dy = y1 - y0
    if abs(dx) < EPSILON:
        if dy >= 0:
            angle = 0
        else:
            angle = 180
    elif dx < 0:
        angle = math.degrees(math.atan2(-dx, dy))
    else:
        angle = math.degrees(math.atan2(-dx, dy))
    return LINE_FMT.format(x0, y0, math.hypot(dx, dy), round(angle,0))

def polyline(points_str: str):
    # Transform polyline endpoints to a list of line_t segments
    points = [float(x) for x in points_str.split()]
    n = len(points) // 2 - 1
    for i in range(n):
        line_t = line(points[2 * i], points[2 * i + 1],
                      points[2 * (i + 1)], points[2 * i + 3])
        yield line_t

def rect(x: float, y: float, width: float, height: float,
         tr_origin: str, transform: str):
    """
    Extract rectangle information from the svg rect parameters. There
    are two cases:
        1. A vanilla rectangle, parallel to the axes, without rotation.
        2. A rotated rectangle. We will have to compute its origin for
           the Ardymo `rectangle_t` format.
        The angle comes from the transform matrix
    """

    # We will need the hexadecimal format of `width` for the last
    # parameter
    hex_width = to_ieee754(width)
    if transform: # Case 2: There is a rotation
        # Parse the transformation matrix
        matrix = [float(i)
                  for i in transform.strip('matrix()').split(',')]
        x, y = get_rect_origin(x, y, width, height, tr_origin, matrix)
        angle = get_angle(matrix)
        return RECTANGLE_FMT.format(x, y, height, angle, hex_width)

    x += width # Origin of our rectangle is top right corner

    return RECTANGLE_FMT.format(x, y, height, 0, hex_width)

def main():
    """
    Get the filename and parse the .svg file.
    Print the obstacles line by line
    """
    if len(sys.argv) != 2:
        print("Usage: svg-parser.py <filename>")
        sys.exit(0)

    path = sys.argv[1]
    tree = elTree.parse(path)
    root = tree.getroot()
    viewBox = root.get('viewBox')

    obstacles = border(viewBox)

    for element in root:
        # Remove the namespace from the tag
        tag = xmlns.sub('', element.tag)
        if tag not in SHAPES:
            continue
        styles = get_style_dict(element.get('style'))

        if tag == 'ellipse':
            obstacles.append(circle(element.get('cx'),
                                    element.get('cy'),
                                    element.get('rx'),
                                    element.get('ry')))

        elif tag == 'line':
            obstacles.append(line(float(element.get('x1')),
                                  float(element.get('y1')),
                                  float(element.get('x2')),
                                  float(element.get('y2'))))

        elif tag == 'polyline':
            # A red line is the path. Not an obstacle
            if styles['stroke'] == 'rgb(255, 0, 0)':
                continue
            for pline in polyline(element.get('points')):
                obstacles.append(pline)

        elif tag == 'rect':
            transform = element.get('transform', '')
            tr_origin = styles.get('transform-origin', '')
            obstacles.append(rect(float(element.get('x')),
                                  float(element.get('y')),
                                  float(element.get('width')),
                                  float(element.get('height')),
                                  tr_origin, transform))

    obst_count = len(obstacles)
    print(f'// {obst_count} obstacles from "{path}"')
    print(f'// Representing {obst_count * 18} bytes')
    print('/* *** Don\'t forget to put the target first manually *** */')
    for obst in obstacles:
        print(obst)

if __name__ == '__main__':
    main()

# vim: ft=python:ts=4:sts=4:expandtab:textwidth=79:sw=4
