# vbo_contructor.py
# File used to build complex VBO

# Import librairies
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
    
    def to_string(self) -> str:
        """Return the vertices into a string

        Returns:
            str: vertices into a string
        """
        to_return = str(round(self.get_point()[0], 5)) + "f " + str(round(self.get_point()[1], 5)) + "f " + str(round(self.get_point()[2], 5)) + "f "
        to_return += str(round(self.get_texture_pos()[0], 5)) + "f " + str(round(self.get_texture_pos()[1], 5)) + "f "
        to_return += str(round(self.get_texture_rect()[0], 5)) + "f " + str(round(self.get_texture_rect()[1], 5)) + "f " + str(round(self.get_texture_rect()[2], 5)) + "f " + str(round(self.get_texture_rect()[3], 5)) + "f "
        to_return += str(round(self.get_rescaling()[0], 5)) + "f " + str(round(self.get_rescaling()[1], 5)) + "f " + str(round(self.get_rescaling()[2], 5)) + "f"
        return to_return

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
                    
def cube(face_pos = [(2/3, 1/4), (1/3, 3/4), (0, 1/4), (1/3, 1/4), (1/3, 1/2), (1/3, 0)], face_size = [(1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4), (1/3, 1/4)], position: tuple = (0, 0, 0), scale: tuple = (1, 1, 1)) -> str:
    """Return the data for a cube

    Returns:
        list: data for a cube
    """
    to_return = ""
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
                          (0.0, -1.0, 1.0), (0.0, -1.0, 1.0),
                          (0.0, -1.0, 1.0), (0.0, -1.0, 1.0)]
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
    
    for v in final_vertices:
        to_return += v.to_string() + " "

    return to_return[:-1]

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

def polygon(diagonal: float, edge: int = 4, position: tuple = (0, 0, 0)) -> str:
    """Return the data for a polygon

    Returns:
        list: data for a polygon
    """
    to_return = ""
    vertices = points_polygon(diagonal, edge, position)

    indices = []
    for i in range(edge - 1):
        indices.append((0, i + 1, i + 2))
    indices.append((0, len(vertices) - 1, 1))

    vertices_texture = points_polygon(diagonal, edge)
    for i in range(len(vertices_texture)):
        pos = (vertices_texture[i][0], vertices_texture[i][1])
        vertices_texture[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)

    indices_texture = []
    for i in range(edge - 1):
        indices_texture.append((0, i + 1, i + 2))
    indices_texture.append((0, len(vertices_texture) - 1, 1))

    for v in vertices:
        for g in v:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"
    for i in indices:
        for g in i:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"
    for v in vertices_texture:
        for g in v:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"
    for i in indices_texture:
        for g in i:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"

    return to_return[:-1]

def polygon_3d(diagonal: float, edge: int = 4, inversed: bool = False, scale: tuple = (1, 1, 1)) -> str:
    """Return the data for a 3d polygon

    Returns:
        list: data for a 3d polygon
    """
    use_mid = True

    to_return = ""
    vertices = points_polygon(diagonal, edge, position = (0, 0, 1.0)) # Get vertices of the first face of the polygon
    indices = []
    for i in range(edge - 1): # Get the indices of every points in the first face
        indices.append((0, i + 1, i + 2))
    indices.append((0, len(vertices) - 1, 1))

    vertices_2 = points_polygon(diagonal, edge, position = (0, 0, -1.0)) # Get vertices of the second face of the polygon

    indices_2 = []
    offset = len(vertices)
    for i in range(edge - 1): # Get the indices of every points in the second face
        indices_2.append((i + 2 + offset, i + 1 + offset, offset))
    indices_2.append((offset + 1, offset + len(vertices_2) - 1, offset))

    if use_mid:
        indices_mid = []
        for v in range(len(vertices)): # Get the indices of every points in the mids face
            v_plus_1 = v + 1
            v_plus_offset = v + offset + 1
            if v_plus_1 >= len(vertices): v_plus_1 = 1
            if v_plus_offset >= len(vertices) + len(vertices_2): v_plus_offset = offset + 1
            indices_mid.append((v + offset, v_plus_1, v))
            indices_mid.append((v + offset, v_plus_offset, v_plus_1))

    vertices_texture = points_polygon(diagonal, edge) # Get the textures points of the first face
    for i in range(len(vertices_texture)):
        pos = (vertices_texture[i][0], vertices_texture[i][1])
        vertices_texture[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)

    indices_texture = []
    for i in range(edge - 1): # Get the indices of the points of the first face
        indices_texture.append((0, i + 1, i + 2))
    indices_texture.append((0, len(vertices_texture) - 1, 1))

    vertices_texture_2 = points_polygon(diagonal, edge) # Get the textures points of the second face
    for i in range(len(vertices_texture_2)):
        pos = (vertices_texture_2[i][0], vertices_texture_2[i][1])
        vertices_texture_2[i] = ((1 + pos[0]) / 2.0, (1 + pos[1]) / 2.0)

    indices_texture_2 = []
    offset = len(vertices_texture_2)
    for i in range(edge - 1): # Get the indices of the textures points of the second face
        indices_texture_2.append((i + 2 + offset, i + 1 + offset, offset))
    indices_texture_2.append((offset + 1, offset + len(vertices_texture_2) - 1, offset))

    if use_mid:
        indices_texture_mid = []
        for i in range(len(vertices) - 1): # Get the indices of the textures points of the mids face
            i_plus_1 = i + 2
            i_plus_offset = i + offset + 2
            if i_plus_1 >= len(vertices): i_plus_1 = 1
            if i_plus_offset >= len(vertices) + len(vertices_2): i_plus_offset = offset + 1
            indices_texture_mid.append((i + offset + 1, i_plus_1, i + 1))
            indices_texture_mid.append((i + offset + 1, i_plus_offset, i_plus_1))
        indices_texture_mid.append((len(vertices_2) + offset - 1, 1, i + 1))
        indices_texture_mid.append((len(vertices_2) + offset - 1, offset + 1, i_plus_1))

    faces = []
    for i in range(len(indices) * 3):
        faces.append((0,))

    if use_mid:
        faces_mid = []
        for i in range(len(indices_mid) * 3):
            faces_mid.append((2,))

    faces_2 = []
    for i in range(len(indices_2) * 3):
        faces_2.append((1,))

    for v in vertices:
        for g in v:
            to_return += str(g) + " "
    for v in vertices_2:
        for g in v:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"
    for i in indices:
        for g in i:
            to_return += str(g) + " "
    for i in indices_2:
        for g in i:
            to_return += str(g) + " "
    if use_mid:
        for i in indices_mid:
            for g in i:
                to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"

    for v in vertices_texture:
        for g in v:
            to_return += str(g) + " "
    for v in vertices_texture_2:
        for g in v:
            to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"
    for i in indices_texture:
        for g in i:
            to_return += str(g) + " "
    for i in indices_texture_2:
        for g in i:
            to_return += str(g) + " "
    if use_mid:
        for i in indices_texture_mid:
            for g in i:
                to_return += str(g) + " "
    to_return = to_return[:-1] + "\n"

    for f in faces:
        to_return += str(f[0]) + " "
    for f in faces_2:
        to_return += str(f[0]) + " "
    if use_mid:
        for f in faces_mid:
            to_return += str(f[0]) + " "

    return to_return[:-1]

class VBO_Constructor:
    """Class representating a easy VBO constructor
    """

    def __init__(self) -> None:
        """Create an easy VBO constructor
        """
        self.content = ""

    def add_content(self, content: str) -> None:
        """Add content to the constructor

        Args:
            parts (list): form to add
        """
        if self.content != "" and self.content[-1] != " ": self.content += " "
        self.content += content

    def get_content(self) -> str:
        """Return the content into the constructor

        Returns:
            str: content into the constructor
        """
        return self.content
    
    def save(self, path: str) -> None:
        """Save the vbo into a file

        Args:
            path (str): file where to save the vbo
        """
        file = open(path, "w")
        file.write(self.get_content())
        file.close()

def construct_chair() -> None:
    """Construct a simple chair
    """
    constructor = VBO_Constructor("in_texcoord_0 in_position", "2f 3f")
    constructor.add_form(cube(face_pos = [()], position = (0, 0.1, 0), scale = (0.7, 0.1, 0.7)).split("\n"))
    constructor.add_form(cube(face = [1, 1, 1, 1, 3, 3], indices_start = 8, position = (-0.5, -0.5, -0.5), scale = (0.1, 0.5, 0.1)).split("\n"))
    constructor.add_form(cube(face = [1, 1, 1, 1, 3, 3], indices_start = 16, position = (-0.5, -0.5, 0.5), scale = (0.1, 0.5, 0.1)).split("\n"))
    constructor.add_form(cube(face = [1, 1, 1, 1, 3, 3], indices_start = 24, position = (0.5, -0.5, -0.5), scale = (0.1, 0.5, 0.1)).split("\n"))
    constructor.add_form(cube(face = [1, 1, 1, 1, 3, 3], indices_start = 32, position = (0.5, -0.5, 0.5), scale = (0.1, 0.5, 0.1)).split("\n"))
    constructor.add_form(cube(face = [4, 1, 1, 1, 3, 3], indices_start = 40, position = (0.0, 0.6, -0.6), scale = (0.7, 0.4, 0.1)).split("\n"))
    constructor.save("vbos/chair.vbo")

def construct_polygon(diagonal: float, edge: int = 4) -> None:
    """Construct a simple polygon
    """
    constructor = VBO_Constructor("in_texcoord_0 in_position", "2f 3f")
    constructor.add_form(polygon(diagonal, edge).split("\n"))
    constructor.save("vbos/polygon" + str(edge) + ".vbo")

def construct_polygon_3d(diagonal: float, edge: int = 4) -> None:
    """Construct a simple polygon
    """
    constructor = VBO_Constructor("in_texcoord_0 in_position in_face", "2f 3f f")
    constructor.add_form(polygon_3d(diagonal, edge).split("\n"))
    constructor.save("vbos/polygon_3d" + str(edge) + ".vbo")

def construct_table() -> None:
    """Construct a simple table
    """
    constructor = VBO_Constructor()
    constructor.add_content(cube(face_pos = [(1/2, 19/20), (1/2, 19/20), (1/2, 19/20), (1/2, 19/20), (0, 1/2), (0, 0)], face_size = [(1/2, 1/20), (1/2, 1/20), (1/2, 1/20), (1/2, 1/20), (1/2, 1/2), (1/2, 1/2)], position = (0, 0.45, 0), scale = (1, 0.1, 1)))
    constructor.add_content(cube(face_pos = [(1/2, 0), (1/2, 0), (1/2, 0), (1/2, 0), (11/20, 0), (11/20, 0)], face_size = [(1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/20), (1/20, 1/20)], position = (-0.45, -0.05, -0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(1/2, 0), (1/2, 0), (1/2, 0), (1/2, 0), (11/20, 0), (11/20, 0)], face_size = [(1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/20), (1/20, 1/20)], position = (-0.45, -0.05, 0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(1/2, 0), (1/2, 0), (1/2, 0), (1/2, 0), (11/20, 0), (11/20, 0)], face_size = [(1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/20), (1/20, 1/20)], position = (0.45, -0.05, -0.45), scale = (0.1, 0.9, 0.1)))
    constructor.add_content(cube(face_pos = [(1/2, 0), (1/2, 0), (1/2, 0), (1/2, 0), (11/20, 0), (11/20, 0)], face_size = [(1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/2), (1/20, 1/20), (1/20, 1/20)], position = (0.45, -0.05, 0.45), scale = (0.1, 0.9, 0.1)))
    constructor.save("vbos/table.vbo")

construct_table()