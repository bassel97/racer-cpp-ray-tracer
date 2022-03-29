# Racer Engine
![Logo](https://github.com/bassel97/racer-cpp-ray-tracer/blob/main/assets/logo/racer-logo.png "Racer Logo")
A practice project using c++ to create a Ray tracer.
This was an assignment and in development into a Ray tracing engine.
Currently optimizing this project and adding more real time features. 

## Development documentation:
### 1. Simple spheres ray tracing:
Rendering an image given a file with spheres positions (Position, Scale, Color, Reflection properties), lights and camera properties.
| Input file|Image|
|-------------|:-------------:|
|NEAR 1<br>LEFT -1<br>RIGHT 1<br>BOTTOM -1<br>TOP 1<br>RES 600 600<br>SPHERE s1 0 0 -10 2 4 2 0.5 0 0 1 1 0.9 0 50<br>SPHERE s2 4 4 -10 1 2 1 0 0.5 0 1 1 0.9 0 50<br>SPHERE s3 -4 2 -10 1 2 1 0 0 0.5 1 1 0.9 0 50<br>SPHERE s4 0 0 -1 0.25 0.25 0.5 0 0 0.5 1 1 1 0 50<br>LIGHT l1 0 0 -1 0.3 0.3 0.3<br>LIGHT l2 10 10 -10 0.9 0.9 0<br>LIGHT l3 -10 5 -5 0 0 0.9<br>BACK 1 1 1<br>AMBIENT 0.2 0.2 0.2|![Spheres ray tracing](https://github.com/bassel97/racer-cpp-ray-tracer/blob/main/tests/illum/testIllum.png "Spheres")|
---

## TODO list and improvements:
- [x] Correct reflection.
- [x] Seperate files and create application structue.
- [ ] More seperation files and create application structue.
- [ ] Add more complex shapes.
- [ ] Implement OBJ importing with materials.
- [x] Replace the input file to be in a JSON formula.
- [x] Use OpenGL to live show effects.
- [ ] Use ImGUI to make changes in scene.
- [ ] Use ImGizmos to actively manipulate the scene.
- [ ] Introduce threading to the project.
- [ ] Try to use graphics card in accelerating.
- [ ] Commenting.
- [ ] Styling using google sheets.
