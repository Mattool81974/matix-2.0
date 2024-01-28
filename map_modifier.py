# texture_modifier.py
# File used to build complex VBO

# Import librairies
import math
import os
import PIL.ImageDraw as id
import PIL.Image as im

big_delimitation = "<----------------------------------------------->"

def is_in(pos: tuple, rect: tuple) -> bool:
    """Verify if a pos is in a tuple or not
    """
    return pos[0] >= rect[0] and pos[1] >= rect[1] and pos[0] < rect[0] + rect[2] and pos[1] < rect[1] + rect[3]

def map_to_str(map: list, level: int, level_count: int) -> str:
    """Convert a map to str
    """
    content = "w " + str(level) + ";" + str(level_count) + "\n"

    for line in map:
        for car in line:
            content += str(car) + ";"
        content = content[:-1] + "\n"

    return content

def construct_warehouse(path: str):
    """Construct a simple warehouse map
    """
    # Define base variables for the map
    content = ""
    door_width = 2
    floor_exterior_part = 1
    floor_interior_part = 2
    middle_part = 0
    wall_part = 10
    width, height = 60, 60
    # Staff part
    staff_part_wall_rect = (0, (height / 4) * 3, height / 2, 1)
    staff_part_entrance_rect = (height / 12 + 3, (height / 4) * 3, door_width, 1) # X, Y, width, height
    toilet_part_wall_rect = (staff_part_entrance_rect[0] - 1, staff_part_entrance_rect[1], 1, height / 4) # X, Y, width, height
    toilet_part_wall_mw_rect = (1, (height / 8) * 7 - 1, height / 12 + 1, 1) # X, Y, width, height
    toilet_part_entrance_m_rect = (toilet_part_wall_rect[0], toilet_part_wall_rect[1] + toilet_part_wall_rect[3] / 2 - 3, 1, door_width) # X, Y, width, height
    toilet_part_entrance_w_rect = (toilet_part_wall_rect[0], toilet_part_wall_rect[1] + toilet_part_wall_rect[3] / 2, 1, door_width) # X, Y, width, height

    # Write the parts of the map
    content += "l\n0 0 0 0 0 0 0 " + str(width) + " 3 " + str(height) + "\n" + big_delimitation + "\n"

    # Write the floor of the first stage
    content += "w 0;0\n"
    for i in range(height):
        for j in range(width):
            if i > height / 2 and j > width / 2:
                content += str(floor_exterior_part) + ";"
            else:
                content += str(floor_interior_part) + ";"
        content = content[:-1] + "\n"
    content += big_delimitation + "\n"

    # Write the middle of the map
    map = []
    for i in range(height):
        line = []
        for j in range(width):
            if not(i > height / 2 and j > width / 2):
                if i == 0 or j == 0 or i == height - 1 or j == width - 1 or (i == height / 2 and j >= width / 2) or (j == width / 2 and i > height / 2):
                    line.append(wall_part)
                elif is_in((i, j), staff_part_wall_rect) and not is_in((i, j), staff_part_entrance_rect):
                    line.append(wall_part)
                elif is_in((i, j), toilet_part_wall_rect) and not is_in((i, j), toilet_part_entrance_m_rect) and not is_in((i, j), toilet_part_entrance_w_rect):
                    line.append(wall_part)
                elif is_in((i, j), toilet_part_wall_mw_rect):
                    line.append(wall_part)
                else: line.append(middle_part)
            else: line.append(middle_part)
        map.append(line)
    content += map_to_str(map, 0, 1)

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

construct_warehouse("maps/warehouse.wad")