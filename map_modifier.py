# texture_modifier.py
# File used to build complex VBO

# Import librairies
import math
import os
import PIL.ImageDraw as id
import PIL.Image as im

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

construct_shooting_range("maps/shooting_range.wad")