# Sort-Tile-Recursive-Partitioning
Sort Tile Recursive Partitioning : Partitioning using RTree from libSpatialIndex.

### Hello World of libSpatialIndex C++ library
https://github.com/libspatialindex/libspatialindex/wiki/1.-Getting-Started
```
g++ -std=c++0x helloLibSpatial.cpp -lspatialindex_c -lspatialindex -o helloLibSpatial
./helloLibSpatial 
```


### How to run partitioning algorithm
```
g++ -std=c++0x str_3d.cpp -lspatialindex_c -lspatialindex -o str
./str testObj.mbb 
```

### Hello World of OpenGL C++ library with installation instructions
http://www.linuxjournal.com/content/introduction-opengl-programming

* **Installation**
```
apt-cache search opengl
sudo apt-get install freeglut3 freeglut3-dev libglew-dev
sudo apt-get install mesa-utils
```
* **Check your OpenGL installation**
```
glxinfo | grep OpenGL
```

* **Draw Triangle and Circle as a HelloWorld program.**
  * Opens Triangle and Circle in two openGL windows.
```
g++ openGLHelloWorld.cpp -lglut -lGL -o hello
./hello
```

* **Draw Rotated Cube using OpenGL.**
```
g++ openGLCube.cpp -lglut -lGL -lGLU -o cube
./cube
```
