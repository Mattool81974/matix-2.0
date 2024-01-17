import math
import os
import PIL.ImageDraw as id
import PIL.Image as im

def create_cube(path: str, final_path: str):
    """Create a cube texture

    Args:
        path (str): path of a directory with the textures
        final_path (str): path where to register the cube
    """

    if not os.path.exists(path):
        print("Error : the path \"" + path + "\" does not exist.")
        return
    
    file1 = im.open(path + "/1.png").resize((500, 500))
    file2 = im.open(path + "/2.png").resize((500, 500))
    file3 = im.open(path + "/3.png").resize((500, 500))
    file4 = im.open(path + "/4.png").resize((500, 500))
    file5 = im.open(path + "/5.png").resize((500, 500))
    file6 = im.open(path + "/6.png").resize((500, 500))

    w, h = 1500, 2000
    cube = im.new("RGBA", (w, h), (255, 255, 255, 0))
    cube.paste(file1, (0, 1000))
    cube.paste(file2, (500, 1000))
    cube.paste(file3, (1000, 1000))
    cube.paste(file4, (500, 0))
    cube.paste(file5, (500, 500))
    cube.paste(file6, (500, 1500))

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

create_cube("textures/locker_dir", "textures/locker.png")
create_cube("textures/computer_dir", "textures/computer.png")