# Resonance
See what this code makes!!!

6 sin waves with a 1:2:4:8:16:32 velocity ratio:
https://youtu.be/Vry77OWy_KI

3 sin waves with a 1:2:3 velocity ratio:
https://youtu.be/cSneg0I7hb4

3 sin waves with a 1:2:4 velocity ratio:
https://youtu.be/sniauc_vMlQ

How to pick your own velocity ratios!
in the main() function in resonance.cpp, set numLines to the number of sine waves you want, and set speeds[] to a float array with your velocity ratio.

Exmaple: 1:2:4 velocity ratio
numLines = 3;
speeds = new float[]{1, 2, 4};

Other notes:
Use 'make all' to compile
Frames render as a .ppm sequence in the oven/ directory. Use ./convert (bash script) to convert to a video
Use ./clear to clear the oven/ directory
