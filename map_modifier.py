# texture_modifier.py
# File used to build complex VBO

# Import librairies
import math
import os
import PIL.ImageDraw as id
import PIL.Image as im

big_delimitation = "<----------------------------------------------->"

def construct_factory(path: str):
    """Construct a simple factory map
    """
    # Define base variables for the map
    content = ""
    floor_part = 1
    middle_part = 0
    wall_part = 2
    width, height = 50, 50

    # Write the parts of the map
    content += "l\n0 0 0 0 0 0 0 " + str(width) + " 3 " + str(height) + "\n" + big_delimitation + "\n"

    # Write the floor of the map
    content += "f 0;0\n"
    for i in range(height):
        for j in range(width):
            content += str(floor_part) + ";"
        content = content[:-1] + "\n"
    content += big_delimitation + "\n"

    # Write the middle of the map
    content += "w 0;0\n"
    for i in range(height):
        for j in range(width):
            if i != -1: content += str(wall_part) + ";"
            else: content += str(middle_part) + ";"
        content = content[:-1] + "\n"

    file = open(path, "w")
    file.write(content)
    file.close()

def construct_shooting_range(path: str):
    """Construct a simple shooting range map
    """

    # Define base variables for the map
    content = ""
    middle_part = 0
    side_part = 1
    width, height = 12, 52

    # Write size of the map
    content += str(width) + ";" + str(height) + "\n"

    # Write first part of the map
    for i in range(width): content += str(side_part) + ";"
    content = content[:-1] + "\n"

    # Write the middle of the map
    for i in range(height - 2):
        content += str(side_part) + ";"
        for i in range(width - 2): content += str(middle_part) + ";"
        content += str(side_part) + "\n"

    # Write last part of the map
    for i in range(width): content += str(side_part) + ";"
    content = content[:-1] + "\n"

    file = open(path, "w")
    file.write(content)
    file.close()

construct_factory("maps/factory.wad")