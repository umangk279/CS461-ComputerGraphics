The scene consists of three spheres.
The left one is a textured sphere. For texture an image is mapped on the sphere. For loading the image as texture SDL-Image library is used.
The middle sphere is a plastic sphere. It has low specular reflection, high diffusion and no shininess. Hence no reflection highlight can be seen on it.
The right sphere is a glossy sphere. It has high specular reflection and high shininess. hence it has the refection highlight.

Command to run:
g++ main.cpp -lGL -lGLU -lglut -lSDL -lSDL_image
./a.out

Libraries required:
1. GLUT: sudo apt-get install freeglut3-dev
2. SDL: sudo apt-get install libsdl2-dev
3. SDL-Image: sudo apt install libsdl2-image-dev
