# vbo_modifier.py
# File used to build complex VBO

# Import librairies
from tkinter import NO
import glm
import math

def flip_x(to_flip: tuple) -> tuple:
    """Flip a tuple into the X axe

    Args:
        to_flip (tuple): tuple to flip

    Returns:
        tuple: tuple flipped
    """
    x = to_flip[0]
    return (-x, to_flip[1])

def flip_y(to_flip: tuple) -> tuple:
    """Flip a tuple into the Y axe

    Args:
        to_flip (tuple): tuple to flip

    Returns:
        tuple: tuple flipped
    """
    y = to_flip[1]
    return (to_flip[0], -y)

class Vertice:
    """Class representing a vertice
    """

    def __init__(self, point: tuple, rescaling: tuple, texture_pos: tuple, texture_rect: tuple, unchanged: int) -> None:
        """Create a Vertice object
        """
        self.point = point
        self.rescaling = rescaling
        self.texture_pos = texture_pos
        self.texture_rect = texture_rect
        self.unchanged = unchanged

    def get_point(self) -> tuple:
        """Return the point of the vertice into the 3D space

        Returns:
            tuple: point of the vertice into the 3D space
        """
        return self.point
    
    def get_rescaling(self) -> tuple:
        """Return the factor of rescaling of the vertice

        Returns:
            tuple: factor of rescaling of the vertice
        """
        return self.rescaling
    
    def get_texture_pos(self) -> tuple:
        """Return the pos of the texture

        Returns:
            tuple: pos of the texture
        """
        return self.texture_pos
    
    def get_texture_rect(self) -> tuple:
        """Return the rect of the texture

        Returns:
            tuple: rect of the texture
        """
        return self.texture_rect
    
    def get_unchanged(self) -> int:
        """Return the index of the axes into the vertices who doesn't change in the face

        Returns:
            int: index of the axes into the vertices who doesn't change in the face
        """
        return self.unchanged
    
    def set_point(self, point: tuple) -> None:
        """Change the value of a point
        """
        self.point = point
    
    def to_string(self, in_3d: bool = True) -> str:
        """Return the vertices into a string

        Returns:
            str: vertices into a string
        """
        to_return = str(round(self.get_point()[0], 5)) + "f " + str(round(self.get_point()[1], 5)) + "f " + str(round(self.get_point()[2], 5)) + "f "
        if in_3d:
            to_return += str(round(self.get_texture_pos()[0], 5)) + "f " + str(round(self.get_texture_pos()[1], 5)) + "f "
            to_return += str(round(self.get_texture_rect()[0], 5)) + "f " + str(round(self.get_texture_rect()[1], 5)) + "f " + str(round(self.get_texture_rect()[2], 5)) + "f " + str(round(self.get_texture_rect()[3], 5)) + "f "
            to_return += str(round(self.get_rescaling()[0], 5)) + "f " + str(round(self.get_rescaling()[1], 5)) + "f " + str(round(self.get_rescaling()[2], 5)) + "f "
        return to_return[:-1]
        

def form_informations(points: list) -> None:
    """Print some rotation about a form

    Args:
        points (list): points to test
    """
    max_x = -100
    minus_x = 100
    max_y = -100
    minus_y = 100
    max_z = -100
    minus_z = 100
    for point in points:
        position = point.get_point()
        if position[0] < minus_x: minus_x = position[0]
        if position[0] > max_x: max_x = position[0]
        if position[1] < minus_y: minus_y = position[1]
        if position[1] > max_y: max_y = position[1]
        if position[2] < minus_z: minus_z = position[2]
        if position[2] > max_z: max_z = position[2]

    print("Informations : ")
    print("Minus/max x :", minus_x, max_x, "| minus/max y :", minus_y, max_y, "| minus/max z:", minus_z, max_z)

def scale(points: list, scaling: tuple) -> list:
    """Take a list of point and scale it
    """
    for point in points:
        difference_position = point.get_point()

        point.set_point((difference_position[0] * scaling[0], difference_position[1] * scaling[1], difference_position[2] * scaling[2]))

def translate(points: list, translation: tuple) -> list:
    """Take a list of point and translate it
    """
    for point in points:
        difference_position = point.get_point()

        point.set_point((difference_position[0] + translation[0], difference_position[1] + translation[1], difference_position[2] + translation[2]))

def get_data(vertices: list, indices: list) -> list:
    """Return a list of vertices ordered by indices

    Args:
        vertices (list): list of vertices
        indices (list): list of indices

    Returns:
        list: vertices ordered by indices
    """
    data = []
    for triangle in indices:
        for indice in triangle:
            data.append(vertices[indice])
    return data
                    
def cube(face_pos = [(2/3, 1/4), (1/3, 3/4), (0, 1/4), (1/3, 1/4), (1/3, 1/2), (1/3, 0)], face_size = [(1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4)], position: tuple = (0, 0, 0), scale: tuple = (1, 1, 1)) -> list:
    """Return the data for a cube

    Returns:
        list: data for a cube
    """
    vertices = [(-0.5, -0.5, 0.5), (0.5, -0.5, 0.5), (0.5, 0.5, 0.5), (-0.5, 0.5, 0.5),
                (-0.5, 0.5, -0.5), (-0.5, -0.5, -0.5), (0.5, -0.5, -0.5), (0.5, 0.5, -0.5)]
    for v in range(len(vertices)):
        vertices[v] = (vertices[v][0] * scale[0] + position[0], vertices[v][1] * scale[1] + position[1], vertices[v][2] * scale[2] + position[2])

    indices = [(0, 2, 3), (0, 1, 2), # Face forward Z (1)
                (1, 7, 2), (1, 6, 7), # Face forward X (4)
                (6, 5, 4), (4, 7, 6), # Face backward Z (3)
                (3, 4, 5), (3, 5, 0), # Face backward X (2)
                (3, 4, 7), (3, 7, 2), # Face forward Y (5)
                (0, 6, 1), (0, 5, 6)] # Face backward Y (6)
    
    vertices_rescaling = [(0.0, 1.0, -1.0), (0.0, 1.0, -1.0),
                          (-1.0, 1.0, 0.0), (-1.0, 1.0, 0.0),
                          (0.0, 1.0, -1.0), (0.0, 1.0, -1.0),
                          (-1.0, 1.0, 0.0), (-1.0, 1.0, 0.0),
                          (1.0, -1.0, 0.0), (1.0, -1.0, 0.0),
                          (1.0, -1.0, 0.0), (1.0, -1.0, 0.0)]
    vertices_unchanged = [(2, 2, 2), (2, 2, 2),
                          (0, 0, 0), (0, 0, 0),
                          (2, 2, 2), (2, 2, 2),
                          (0, 0, 0), (0, 0, 0),
                          (1, 1, 1), (1, 1, 1),
                          (1, 1, 1), (1, 1, 1)]
    
    final_vertices = []
    
    for i in range(len(indices)):
        for g in range(len(indices[i])):
            part_vertices = vertices[indices[i][g]] # Vertices pos

            initial_pos = face_pos[math.floor(i/2)] # Texture pos
            pos = (0, 0)
            n = 0
            if vertices[indices[i][g]][vertices_unchanged[i][g]] > position[vertices_unchanged[i][g]]:
                n = 1

            o = 1
            p = 0
            if i // 2 == 2:
                o = 0

            if i // 2 == 4:
                if i % 2 == 1:
                    p = 1
                else:
                    o = 0

            if i // 2 == 5:
                if i % 2 == 0:
                    p = 1
                else:
                    o = 0

            size = face_size[math.floor(i/2)]
            if i % 2 == n:
                if g % 3 == p: 
                    pos = initial_pos
                elif g % 3 == p + 1 or (p == 2 and g % 3 == 0): 
                    pos = (initial_pos[0] + size[0], initial_pos[1])
                else: 
                    pos = (initial_pos[0] + size[0], initial_pos[1] + size[1])
            else:
                if g % 3 == o: 
                    pos = (initial_pos[0] + size[0], initial_pos[1] + size[1])
                elif g % 3 == o + 1: 
                    pos = (initial_pos[0], initial_pos[1] + size[1])
                else: 
                    pos = initial_pos

            local_pos = (pos[0] - initial_pos[0], pos[1] - initial_pos[1])
            if i // 2 == 3:
                local_pos = (-(local_pos[0] - size[0]), -(local_pos[1] - size[1]))
            if i // 2 == 4:
                local_pos = (local_pos[0], -(local_pos[1] - size[1]))

            final_vertices.append(Vertice(part_vertices, vertices_rescaling[i], (initial_pos[0] + local_pos[0], initial_pos[1] + local_pos[1]), (initial_pos[0], initial_pos[1], size[0], size[1]), vertices_unchanged[i][g]))
    
    return final_vertices

def points_polygon(diagonal: float, edge: int = 4, position: tuple = (0, 0, 0)) -> list:
    """Return a list of the point into a polygon

    Returns:
        list: point into a polygon
    """
    vertices = [position]
    for i in range(edge):
        theta = (2 * math.pi * i) / edge + math.pi / 4.0
        x = math.cos(theta)
        y = math.sin(theta)
        vertices.append((x * diagonal + position[0], y * diagonal + position[1], position[2]))
    return vertices

def polygon(diagonal: float, face_pos = (0, 0), face_size = (1, 1), edge: int = 4, position: tuple = (0, 0, 0)) -> str:
    """Return the data for a polygon

    Returns:
        list: data for a polygon
    """
    to_return = ""
    vertices = points_polygon(diagonal, edge, position) # Get the points into the polygon
    for v in range(len(vertices)):
        vertices[v] = (vertices[v][0] / 2, vertices[v][1] / 2, vertices[v][2] / 2)

    indices = []
    for i in range(edge - 1):
        indices.append((0, i + 1, i + 2))
    indices.append((0, len(vertices) - 1, 1))

    vertices_texture = points_polygon(diagonal, edge)
    for i in range(len(vertices_texture)):
        pos = (vertices_texture[i][0], vertices_texture[i][1])
        vertices_texture[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)
        vertices_texture[i] = (vertices_texture[i][0] + face_pos[0], vertices_texture[i][1] + face_pos[1])

    final_vertices = []
    for i in range(len(indices)):
        for g in range(len(indices[i])):
            part_vertices = vertices[indices[i][g]] # Vertices pos

            initial_pos = vertices_texture[indices[i][g]]

            vertice_rescaling = (0, 1, -1)
            final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], face_size[0], face_size[1]), 2))

    for v in final_vertices:
        to_return += v.to_string() + " "

    return to_return[:-1]

def polygon_3d(diagonal: float, edge: int = 4, position: tuple = (0, 0, 0), scale: tuple = (1, 1, 1), texture_bottom_pos = (0, 0), texture_middle_pos = (0, 1000/2571), texture_middle_size = (1, 1570/2571), texture_top_bottom_size = (1, 500/2571), texture_top_pos = (0, 500/2571), top_texture_size = (0, 0)) -> str:
    """Return the data for a 3d polygon

    Returns:
        list: data for a 3d polygon
    """
    use_mid = True

    to_return = ""
    vertices = points_polygon(diagonal, edge, position = (0, 0, 1.0)) # Get vertices of the first face of the polygon
    for v in range(len(vertices)):
        vertices[v] = (vertices[v][0] / 2 + position[0], vertices[v][1] / 2 + position[1], vertices[v][2] / 2 + position[2])
    indices = []
    for i in range(edge - 1): # Get the indices of every points in the first face
        indices.append((0, i + 1, i + 2))
    indices.append((0, len(vertices) - 1, 1))

    vertices_2 = points_polygon(diagonal, edge, position = (0, 0, -1.0)) # Get vertices of the second face of the polygon
    for v in range(len(vertices_2)):
        vertices_2[v] = (vertices_2[v][0] / 2 + position[0], vertices_2[v][1] / 2 + position[1], vertices_2[v][2] / 2 + position[2])
    indices_2 = []
    for i in range(edge - 1): # Get the indices of every points in the second face
        indices_2.append((i + 2, i + 1, 0))
    indices_2.append((1, len(vertices_2) - 1, 0))

    if use_mid:
        indices_mid = []
        for v in range(len(vertices)): # Get the indices of every points in the mids face
            v_plus_1 = v + 1
            v_plus_offset = v + 1
            if v_plus_1 >= len(vertices): v_plus_1 = 1
            if v_plus_offset >= len(vertices_2): v_plus_offset = 1
            indices_mid.append((v, v_plus_1, v))
            indices_mid.append((v, v_plus_offset, v_plus_1))

    vertices_texture = points_polygon(diagonal, edge) # Get the textures points of the first face
    for i in range(len(vertices_texture)):
        pos = (vertices_texture[i][0], vertices_texture[i][1])
        vertices_texture[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)
        vertices_texture[i] = ((vertices_texture[i][0]) * texture_top_bottom_size[0] + texture_top_pos[0], (vertices_texture[i][1]) * texture_top_bottom_size[1] + texture_top_pos[1])

    indices_texture = []
    for i in range(edge - 1): # Get the indices of the points of the first face
        indices_texture.append((0, i + 1, i + 2))
    indices_texture.append((0, len(vertices_texture) - 1, 1))

    vertices_texture_2 = points_polygon(diagonal, edge) # Get the textures points of the second face
    for i in range(len(vertices_texture_2)):
        pos = (vertices_texture_2[i][0], vertices_texture_2[i][1])
        vertices_texture_2[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)
        vertices_texture_2[i] = ((vertices_texture_2[i][0]) * texture_top_bottom_size[0] + texture_bottom_pos[0], (vertices_texture_2[i][1]) * texture_top_bottom_size[1] + texture_bottom_pos[1])

    indices_texture_2 = []
    for i in range(edge - 1): # Get the indices of the textures points of the second face
        indices_texture_2.append((i + 2, i + 1, 0))
    indices_texture_2.append((1, len(vertices_texture_2) - 1, 0))

    if use_mid:
        indices_texture_mid = []
        for i in range(len(vertices) - 1): # Get the indices of the textures points of the mids face
            i_plus_1 = i + 2
            i_plus_offset = i + 2
            if i_plus_1 >= len(vertices): i_plus_1 = 1
            if i_plus_offset >= len(vertices) + len(vertices_2): i_plus_offset = 1
            indices_texture_mid.append((i + 1, i_plus_1, i + 1))
            indices_texture_mid.append((i + 1, i_plus_offset, i_plus_1))
        indices_texture_mid.append((len(vertices_2) - 1, 1, i + 1))
        indices_texture_mid.append((len(vertices_2) - 1, 1, i_plus_1))

    final_vertices = []
    for i in range(len(indices)):
        for g in range(len(indices[i])):
            part_vertices = vertices[indices[i][g]] # Vertices pos

            initial_pos = vertices_texture[indices[i][g]]

            vertice_rescaling = (0, 1, -1)
            final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], texture_top_bottom_size[0], texture_top_bottom_size[1]), 2))

    if use_mid:
        for i in range(len(indices_mid)):
            for g in range(len(indices_mid[i])):
                initial_pos_x = 0
                part_vertices = vertices[indices_mid[i][g // 2]] # Vertices pos
                if g % 2 == i % 2:
                    initial_pos_x = 1
                    part_vertices = vertices_2[indices_mid[i][g // 2 + 1]] # Vertices pos

                initial_pos = (initial_pos_x, texture_middle_pos[1] + i * (texture_middle_size[1] / len(indices_mid)))

                vertice_rescaling = (0, -1, 1)
                final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], texture_middle_size[0], texture_middle_size[1]), 2))
    
    for i in range(len(indices_2)):
        for g in range(len(indices_2[i])):
            part_vertices = vertices_2[indices_2[i][g]] # Vertices pos

            initial_pos = vertices_texture_2[indices_2[i][g]]

            vertice_rescaling = (0, 1, -1)
            final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], texture_top_bottom_size[0], texture_top_bottom_size[1]), 2))

    for v in final_vertices:
        to_return += v.to_string() + " "

    return to_return[:-1]

def pyramid(diagonal: float, apex_texture_pos = (0, 0), apex_size = (1, 1, 1), apex_texture_size = (1, 0.5), edge: int = 4, edge_texture_pos = (0, 0.5), edge_texture_size = (1, 0.5), position = (0, 0, 0), top_position = (0, 0, 0.5)) -> str:
    """Return the data for a 3d pyramid

    Returns:
        list: data for a 3d polygon
    """
    to_return = ""
    vertices = points_polygon(diagonal, edge, position = (0, 0, -1.0)) # Get vertices of the first face of the polygon
    for v in range(len(vertices)):
        vertices[v] = ((vertices[v][0] / 2) * apex_size[0] + position[0], (vertices[v][1] / 2) * apex_size[1] + position[1], (vertices[v][2] / 2) * apex_size[2] + position[2])

    indices = []
    for i in range(edge - 1): # Get the indices of every points in the first face
        indices.append((0, i + 1, i + 2))
    indices.append((0, len(vertices) - 1, 1))

    indices_mid = []
    for i in range(edge - 1): # Get the indices of every triangle of the pyramid
        indices_mid.append((-1, i + 1, i + 2))
    indices_mid.append((-1, len(vertices) - 1, 1))

    vertices_texture = points_polygon(diagonal, edge) # Get the textures points of the first face
    for i in range(len(vertices_texture)):
        pos = (vertices_texture[i][0], vertices_texture[i][1])
        vertices_texture[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)
        vertices_texture[i] = ((vertices_texture[i][0]) * apex_texture_size[0] + apex_texture_pos[0], (vertices_texture[i][1]) * apex_texture_size[1] + apex_texture_pos[1])

    indices_texture = []
    for i in range(edge - 1): # Get the indices of the points of the first face
        indices_texture.append((0, i + 1, i + 2))
    indices_texture.append((0, len(vertices_texture) - 1, 1))

    final_vertices = []
    for i in range(len(indices)):
        for g in range(len(indices[i])):
            part_vertices = vertices[indices[i][g]] # Vertices pos

            initial_pos = vertices_texture[indices[i][g]]

            vertice_rescaling = (0, 1, -1)
            final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], apex_texture_size[0], apex_texture_size[1]), 2))

    vertices.append((top_position[0] + position[0], top_position[1] + position[1], top_position[1] + position[2]))

    for i in range(len(indices_mid)):
        for g in range(len(indices_mid[i])):
            part_vertices = vertices[indices_mid[i][g]] # Vertices pos

            if part_vertices == vertices[-1]:
                initial_pos = (edge_texture_pos[0] + edge_texture_size[0] / 2, edge_texture_pos[1] + edge_texture_size[1])
            else:
                initial_pos = edge_texture_pos

            vertice_rescaling = (0, 1, -1)
            final_vertices.append(Vertice(part_vertices, vertice_rescaling, (initial_pos[0], initial_pos[1]), (initial_pos[0], initial_pos[1], edge_texture_size[0], edge_texture_size[1]), 2))

    for v in final_vertices:
        to_return += v.to_string() + " "

    return to_return[:-1]

class VBO_Constructor:
    """Class representating a easy VBO constructor
    """

    def __init__(self) -> None:
        """Create an easy VBO constructor
        """
        self.vertices = []

    def add_content(self, vertices: str) -> None:
        """Add content to the constructor

        Args:
            parts (list): form to add
        """
        self.vertices += vertices

    def get_vertices(self) -> str:
        """Return the vertices into the constructor

        Returns:
            list: content into the constructor
        """
        return self.vertices
    
    def informations(self) -> None:
        """Gives informations about the VBO
        """
        form_informations(self.get_vertices())
    
    def save(self, path: str) -> None:
        """Save the vbo into a file

        Args:
            path (str): file where to save the vbo
        """
        content = ""
        for i in self.get_vertices():
            content += i.to_string() + " "

        file = open(path, "w")
        file.write(content[:-1])
        file.close()

def construct_chair() -> None:
    """Construct a simple chair
    """
    constructor = VBO_Constructor()
    constructor.add_content(cube(face_pos = [(0, 1572/1622), (0, 1572/1622), (0, 1572/1622), (0, 1572/1622), (0, 500/1622), (0, 0)], face_size = [(10/11, 50/1622), (10/11, 50/1622), (10/11, 50/1622), (10/11, 50/1622), (10/11, 500/1622), (10/11, 500/1622)], position = (0, 0.05, 0), scale = (0.7, 0.1, 0.7)))
    constructor.add_content(cube(face_pos = [(10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 1000/1622), (10/11, 1000/1622)], face_size = [(1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 50/1622), (1/11, 50/1622)], position = (-0.25, -0.25, -0.25), scale = (0.1, 0.5, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 1000/1622), (10/11, 1000/1622)], face_size = [(1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 50/1622), (1/11, 50/1622)], position = (-0.25, -0.25, 0.25), scale = (0.1, 0.5, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 1000/1622), (10/11, 1000/1622)], face_size = [(1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 50/1622), (1/11, 50/1622)], position = (0.25, -0.25, -0.25), scale = (0.1, 0.5, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 500/1622), (10/11, 1000/1622), (10/11, 1000/1622)], face_size = [(1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 50/1622), (1/11, 50/1622)], position = (0.25, -0.25, 0.25), scale = (0.1, 0.5, 0.1)))
    constructor.add_content(cube(face_pos = [(0, 1286/1622), (10/11, 0), (0, 1000/1622), (10/11, 0), (10/11, 0), (10/11, 0)], face_size = [(10/11, 286/1622), (1/11, 500/1622), (10/11, 286/1622), (1/11, 500/1622), (1/11, 500/1622), (1/11, 500/1622)], position = (0.0, 0.3, -0.3), scale = (0.7, 0.4, 0.1)))
    constructor.save("vbos/chair.vbo")

def construct_cube() -> None:
    """Construct a simple cube
    """
    constructor = VBO_Constructor()
    constructor.add_content(cube())
    constructor.save("vbos/cube.vbo")

def construct_famas() -> None:
    """Construct a FaMAS assault rifle
    """
    texture_size = (542, 250)

    # 523 190
    width = 1 / 10
    constructor = VBO_Constructor()
    breech_height = 25
    breech_width = 430
    breech_x = (93 + 430 / 2)
    breech_y = 140 - breech_height / 2
    constructor.add_content(cube(face_pos = [(0, 142/texture_size[1]), (0, 0), (0, 142/texture_size[1]), (0, 0), (0, 80/texture_size[1]), (0, 80/texture_size[1])], face_size = [(38/texture_size[0], 50/texture_size[1]), (500/texture_size[0], 80/texture_size[1]), (38/texture_size[0], 50/texture_size[1]), (500/texture_size[0], 80/texture_size[1]), (500/texture_size[0], 62/texture_size[1]), (500/texture_size[0], 62/texture_size[1])], position = (0, breech_y / 190, breech_x / 523), scale = (width, breech_height / 190, breech_width / 523)))

    # Construct the top part
    top_height = 20
    top_wrist_height = 7
    top_wrist_length = 240
    top_y = breech_y / 190 + breech_height / 190 / 2 + top_height / 190 / 2
    constructor.add_content(cube(face_pos = [(415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1])], face_size = [(18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (25/texture_size[0], 25/texture_size[1]), (25/texture_size[0], 25/texture_size[1])], position = (0, top_y, (93 + 20 / 2) / 523), scale = (width, top_height /  190, 20 / 523)))
    constructor.add_content(cube(face_pos = [(415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (415/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1])], face_size = [(18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (18/texture_size[0], 50/texture_size[1]), (25/texture_size[0], 25/texture_size[1]), (25/texture_size[0], 25/texture_size[1])], position = (0, top_y, (315 + 20 / 2) / 523), scale = (width, top_height /  190, 20 / 523)))
    constructor.add_content(cube(face_pos = [(390/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1]), (390/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1]), (190/texture_size[0], 142/texture_size[1])], face_size = [(25/texture_size[0], 25/texture_size[1]), (200/texture_size[0], 16/texture_size[1]), (25/texture_size[0], 25/texture_size[1]), (200/texture_size[0], 16/texture_size[1]), (200/texture_size[0], 16/texture_size[1]), (200/texture_size[0], 16/texture_size[1])], position = (0, top_y + top_height / 190 / 2, (93 + top_wrist_length / 2) / 523), scale = (width, top_wrist_height / 190, top_wrist_length / 523)))

    # Construct the wrist
    wrist_height = 30
    wrist_length = 35
    wrist_width = width / 2
    wrist_x = 280 + wrist_length / 2
    constructor.add_content(cube(face_pos = [(433/texture_size[0], 142/texture_size[1]), (433/texture_size[0], 142/texture_size[1]), (433/texture_size[0], 142/texture_size[1]), (433/texture_size[0], 142/texture_size[1]), (454/texture_size[0], 142/texture_size[1]), (454/texture_size[0], 142/texture_size[1])], face_size = [(21/texture_size[0], 50/texture_size[1]), (21/texture_size[0], 50/texture_size[1]), (21/texture_size[0], 50/texture_size[1]), (21/texture_size[0], 50/texture_size[1]), (16/texture_size[0], 25/texture_size[1]), (16/texture_size[0], 25/texture_size[1])], position = (0, (breech_y - (breech_height / 2 + wrist_height / 2)) / 190, wrist_x / 523), scale = (wrist_width, wrist_height / 190, wrist_length / 523)))

    # Construct the loader
    loader_height = 35
    loader_length = 40
    loader_width = width / 2
    loader_x = 385 + loader_length / 2
    constructor.add_content(cube(face_pos = [(500/texture_size[0], 100/texture_size[1]), (500/texture_size[0], 0), (500/texture_size[0], 100/texture_size[1]), (500/texture_size[0], 0), (500/texture_size[0], 100/texture_size[1]), (500/texture_size[0], 100/texture_size[1])], face_size = [(8/texture_size[0], 8/texture_size[1]), (42/texture_size[0], 100/texture_size[1]), (8/texture_size[0], 8/texture_size[1]), (42/texture_size[0], 100/texture_size[1]), (8/texture_size[0], 8/texture_size[1]), (8/texture_size[0], 8/texture_size[1])], position = (0, (breech_y - (breech_height / 2 + loader_height / 2)) / 190, loader_x / 523), scale = (loader_width, loader_height / 190, loader_length / 523)))

    # Construct the gun
    gun_length = 93
    gun_width = width / 3
    constructor.add_content(cube(face_pos = [(140/texture_size[0], 142/texture_size[1]), (38/texture_size[0], 142/texture_size[1]), (140/texture_size[0], 142/texture_size[1]), (38/texture_size[0], 142/texture_size[1]), (38/texture_size[0], 142/texture_size[1]), (38/texture_size[0], 142/texture_size[1])], face_size = [(50/texture_size[0], 50/texture_size[1]), (102/texture_size[0], 20/texture_size[1]), (50/texture_size[0], 50/texture_size[1]), (102/texture_size[0], 20/texture_size[1]), (102/texture_size[0], 20/texture_size[1]), (102/texture_size[0], 20/texture_size[1])], position = (0, breech_y / 190, (breech_x - (breech_width / 2 + gun_length / 2)) / 523), scale = (gun_width, gun_width, gun_length / 523)))
    
    translate(constructor.get_vertices(), (0, -(0.4210526315789474 + (0.8605263157894736 - 0.4210526315789474) / 2), -0.5))
    scale(constructor.get_vertices(), (0.5, 0.5, 0.5))
    constructor.informations()
    constructor.save("vbos/famas.vbo")

def construct_polygon(diagonal: float, edge: int = 4) -> None:
    """Construct a simple polygon
    """
    constructor = VBO_Constructor()
    constructor.add_content(polygon(diagonal, edge = edge))
    constructor.save("vbos/polygon" + str(edge) + ".vbo")

def construct_polygon_3d(diagonal: float, edge: int = 4) -> None:
    """Construct a simple polygon
    """
    constructor = VBO_Constructor()
    constructor.add_content(polygon_3d(diagonal, edge))
    constructor.save("vbos/polygon_3d" + str(edge) + ".vbo")

def construct_shell() -> None:
    """Construct a simple polygon
    """
    texture_size = (500, 2571)

    constructor = VBO_Constructor()
    constructor.add_content(pyramid(1, edge = 10, apex_size = (1, 1, 1), edge_texture_pos = (0, 0), edge_texture_size = (500 / texture_size[0], 500 / texture_size[1]), apex_texture_pos = (0, 500 / texture_size[1]), apex_texture_size = (500 / texture_size[0], 500 / texture_size[1]), position = (0, 0, 0.5)))
    constructor.add_content(polygon_3d(1, edge = 10, position = (0, 0, -0.5), texture_bottom_pos = (0, 500 / texture_size[1]), texture_middle_pos = (0, 1000 / texture_size[1]), texture_middle_size = (500 / texture_size[0], 1571 / texture_size[1]), texture_top_pos = (0, 500 / texture_size[1])))
    constructor.save("vbos/shell.vbo")

def construct_table() -> None:
    """Construct a simple table
    """
    constructor = VBO_Constructor()
    constructor.add_content(cube(face_pos = [(0, 20/21), (0, 20/21), (0, 20/21), (0, 20/21), (0, 10/21), (0, 0)], face_size = [(10/11, 1/21), (10/11, 1/21), (10/11, 1/21), (10/11, 1/21), (10/11, 10/21), (10/11, 10/21)], position = (0, 0.45, 0), scale = (1, 0.1, 1)))
    constructor.add_content(cube(face_pos = [(10/11, 0), (10/11, 0), (10/11, 0), (10/11, 0), (10/11, 10/21), (10/11, 10/21)], face_size = [(1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 1/21), (1/11, 1/21)], position = (-0.45, -0.05, -0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 0), (10/11, 0), (10/11, 0), (10/11, 0), (10/11, 10/21), (10/11, 10/21)], face_size = [(1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 1/21), (1/11, 1/21)], position = (-0.45, -0.05, 0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 0), (10/11, 0), (10/11, 0), (10/11, 0), (10/11, 10/21), (10/11, 10/21)], face_size = [(1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 1/21), (1/11, 1/21)], position = (0.45, -0.05, -0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(10/11, 0), (10/11, 0), (10/11, 0), (10/11, 0), (10/11, 10/21), (10/11, 10/21)], face_size = [(1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 10/21), (1/11, 1/21), (1/11, 1/21)], position = (0.45, -0.05, 0.45), scale = (0.1, 0.9, 0.1)))
    constructor.save("vbos/table.vbo")

construct_cube()