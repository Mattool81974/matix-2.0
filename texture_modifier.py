import math
import PIL.ImageDraw as id
import PIL.Image as im

def perfect_texture(path):
    """Draw a perfect texture

    Path (str): path where to store the texture
    """
    w, h = 1000, 2000
    
    # creating new Image object 
    img = im.new("RGBA", (w, h), (255, 255, 255, 255)) 
    
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

perfect_texture("textures/perfect.png")