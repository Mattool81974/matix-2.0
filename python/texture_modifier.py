# texture_modifier.py
# File used to build complex VBO

# Import librairies
import math
import os
import pygame as pg
import PIL.ImageDraw as id
import PIL.Image as im

font_container = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-*/,;:!?./§\\éèàçù^¨#{]()}|<*%¨> =\'\"_@"
font_size = 200
font_square_size = (105, 200)

def create_chair(path: str, final_path: str) -> None:
    """Create a chair texture

    Args:
        path (str): path through the table texture dir
        final_path (str): path through the final texture
    """
    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/front_back.png").resize((500, 286))
    file2 = im.open(path + "/behind_back.png").resize((500, 286))
    file3 = im.open(path + "/border_back.png").resize((50, 500))
    file4 = im.open(path + "/top_seating.png").resize((500, 500))
    file5 = im.open(path + "/bottom_seating.png").resize((500, 500))
    file6 = im.open(path + "/border_seating.png").resize((500, 50))
    file7 = im.open(path + "/leg.png").resize((50, 500))
    file8 = im.open(path + "/floor.png").resize((50, 50))

    w, h = 550, 1622
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 50))
    cube.paste(file2, (0, 336))
    cube.paste(file3, (500, 1122))
    cube.paste(file4, (0, 622))
    cube.paste(file5, (0, 1122))
    cube.paste(file6, (0, 0))
    cube.paste(file7, (500, 622))
    cube.paste(file8, (500, 572))

    cube.save(final_path)

def create_cylinder(path: str, final_path: str) -> None:
    """Create a cylinder texture

    Args:
        path (str): path of a directory with the textures
        final_path (str): path where to register the cylinder
    """
    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/top.png").resize((500, 500))
    file2 = im.open(path + "/bottom.png").resize((500, 500))
    file3 = im.open(path + "/middle.png").resize((500, 1570))

    w, h = 500, 2570
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 1570))
    cube.paste(file2, (0, 2070))
    cube.paste(file3, (0, 0))

    cube.save(final_path)

def create_cube(path: str, final_path: str) -> None:
    """Create a cube texture

    Args:
        path (str): path of a directory with the textures
        final_path (str): path where to register the cube
    """

    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/0.png").resize((500, 500))
    file2 = im.open(path + "/0.png").resize((500, 500))
    file3 = im.open(path + "/0.png").resize((500, 500))
    file4 = im.open(path + "/2.png").resize((500, 500))
    file5 = im.open(path + "/0.png").resize((500, 500))
    file6 = im.open(path + "/0.png").resize((500, 500))

    w, h = 1500, 2000
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 1000))
    cube.paste(file2, (500, 1000))
    cube.paste(file3, (1000, 1000))
    cube.paste(file4, (500, 0))
    cube.paste(file5, (500, 500))
    cube.paste(file6, (500, 1500))

    cube.save(final_path)

def create_font(path: str, font_name: str, final_size = (1000, 1000)) -> None:
    """Create a font texture
    """
    print(len(font_container))
    # Create main texture
    final_surface = pg.Surface(final_size, pg.SRCALPHA).convert_alpha()

    if font_name == "default":
        font = pg.font.Font(pg.font.get_default_font(), font_size) # Create font
    else:
        font = pg.font.SysFont(font_name, font_size) # Create font

    for i in range(len(font_container)):
        char = pg.Surface(font_square_size, pg.SRCALPHA)
        drawn_character = font.render(font_container[i], True, (0, 0, 0, 255))
        print(font_container[i], drawn_character.get_width())
        char.blit(drawn_character, (0, 0, char.get_width(), char.get_height())) # Draw the text

        x = (i % (final_size[0] / font_square_size[0])) * font_square_size[0]
        y = (i // (final_size[1] / font_square_size[1])) * font_square_size[1]
        final_surface.blit(char, (x, y, char.get_width(), char.get_height()))
    
    pg.image.save(final_surface, path)

def create_famas(path: str, final_path: str) -> None:
    """Create a famas texture

    Args:
        path (str): path through the famas texture dir
        final_path (str): path through the final texture
    """
    file1 = im.open(path + "/breech_side.png").resize((500, 80))
    file2 = im.open(path + "/breech_top_bottom.png").resize((500, 62))
    file3 = im.open(path + "/breech_front_back.png").resize((38, 50))
    file4 = im.open(path + "/gun_side.png").resize((102, 20))
    file5 = im.open(path + "/gun_front_back.png").resize((50, 50))
    file6 = im.open(path + "/top_wrist_side_1.png").resize((200, 16))
    file7 = im.open(path + "/top_wrist_side_2.png").resize((25, 25))
    file8 = im.open(path + "/top_wrist_side_3.png").resize((18, 50))
    file9 = im.open(path + "/wrist_side.png").resize((21, 50))
    file10 = im.open(path + "/wrist_top_bottom.png").resize((16, 25))
    file11 = im.open(path + "/loader_side.png").resize((42, 100))
    file12 = im.open(path + "/loader_front_back.png").resize((8, 8))

    w, h = 542, 250
    cube = im.new("RGBA", (w, h), (0, 0, 0, 255))
    y = h - 80
    cube.paste(file1, (0, y))
    cube.paste(file2, (0, y - 62))
    cube.paste(file3, (0, y - (62 + 50)))
    cube.paste(file4, (38, y - (62 + 20)))
    cube.paste(file5, (140, y - (62 + 50)))
    cube.paste(file6, (190, y - (62 + 16)))
    cube.paste(file7, (390, y - (62 + 25)))
    cube.paste(file8, (415, y - (62 + 50)))
    cube.paste(file9, (433, y - (62 + 50)))
    cube.paste(file10, (454, y - (62 + 25)))
    cube.paste(file11, (500, y - 20))
    cube.paste(file12, (500, y - 28))

    cube.save(final_path)

def create_shell(path: str, final_path: str) -> None:
    """Create a shell texture

    Args:
        path (str): path through the shell texture dir
        final_path (str): path through the final texture
    """
    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/bottom.png").resize((500, 500))
    file2 = im.open(path + "/tip.png").resize((500, 500))
    file3 = im.open(path + "/side.png").resize((500, 1571))

    w, h = 500, 2571
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 2571 - 1000))
    cube.paste(file2, (0, 2571 - 500))
    cube.paste(file3, (0, 0))

    cube.save(final_path)

def create_table(path: str, final_path: str) -> None:
    """Create a table texture

    Args:
        path (str): path through the table texture dir
        final_path (str): path through the final texture
    """
    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/top.png").resize((500, 500))
    file2 = im.open(path + "/bottom.png").resize((500, 500))
    file3 = im.open(path + "/border.png").resize((500, 50))
    file4 = im.open(path + "/leg.png").resize((50, 500))
    file5 = im.open(path + "/floor.png").resize((50, 50))

    w, h = 550, 1050
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 50))
    cube.paste(file2, (0, 550))
    cube.paste(file3, (0, 0))
    cube.paste(file4, (500, 550))
    cube.paste(file5, (500, 500))

    cube.save(final_path)

def perfect_texture(path):
    """Draw a perfect texture

    Path (str): path where to store the texture
    """
    w, h = 1500, 2000
    
    # creating new Image object 
    img = im.new("RGBA", (w, h), (255, 255, 255, 0)) 
    
    # create rectangle image 
    rectangle_width = 501
    offset = 0
    img1 = id.Draw(img)   
    img1.rectangle(((0 - offset, 0 - offset), (rectangle_width, rectangle_width)), fill ="red")
    img1.rectangle(((999, 0), (1000, 2000)), fill ="green")
    img1.rectangle(((0, 1999), (1000, 2000)), fill ="blue")
    """img1.rectangle(((0 - offset, 500 - offset), (rectangle_width, 500 + rectangle_width)), fill ="blue") 
    img1.rectangle(((0 - offset, 1000 - offset), (rectangle_width, 1000 + rectangle_width)), fill ="green")
    img1.rectangle(((0 - offset, 1500 - offset), (rectangle_width, 1500 + rectangle_width)), fill ="purple")
    img1.rectangle(((rectangle_width, 0 - offset), (500 + rectangle_width, rectangle_width)), fill ="pink") 
    img1.rectangle(((rectangle_width, 500 - offset), (500 + rectangle_width, 500 + rectangle_width)), fill ="grey")"""
    img.save(path)

pg.init()
pg.display.set_mode((1, 1))

path = "../assets/textures/lunar_rover/"
create_cube(path + "rover/head_dir", path + "rover/head.png")