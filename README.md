# Binary Space Partitioning (BSP)
Recursively divides the space into two parts until the leaf nodes satisfy the constraints such as minimum number of spatial objects in a partition.
### How to run Binary Space Partitioning algorithm
```
g++ -std=c++0x bsp_3d.cpp -o bsp
./bsp testObj.mbb 
```

# Sort Tile Recursive Partitioning (STR)
Sort Tile Recursive Partitioning : Partitioning using RTree from libSpatialIndex.

### Hello World of libSpatialIndex C++ library
https://github.com/libspatialindex/libspatialindex/wiki/1.-Getting-Started
```
g++ -std=c++0x helloLibSpatial.cpp -lspatialindex_c -lspatialindex -o helloLibSpatial
./helloLibSpatial 
```


### How to run Sort Tile Partitioning algorithm
```
g++ -std=c++0x str_3d.cpp -lspatialindex_c -lspatialindex -o str
./str testObj.mbb 
```

# Visualization with OpenGL (trials)
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

# Visualization with Matlab
After trying several things in OpenGL, I found stackoverflow links directing to Matlab. The APIs were simpler and easy to implement. So, I implemented the partitioning algorithm visualizations in Matlab and not in OpenGL.
```
Run simplecube.m
```
The visualizations are for both BSP and STR partitioning algorithms. 
1. **BSP** <br>
Input for BSP visualization is
   * testObj.dat containing spatial MBBs
   * boundingBoxBSP.dat has output partitioned MBBs.
Output is a bounding box visualization. Check project report or bsp.png for the visualization.

2. **STR** <br>
Input for STR visualization is 
   * testObj.dat containing spatial MBBs
   * boundingBoxSTR.dat has output partitioned MBBs.
Output is a bounding box visualization. Check project report or str.png for the visualization.
