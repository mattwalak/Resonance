#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include "Util.h"
#include "hsvtorgb.h"

#define SMOOTH(start, end, t) (start) + ((end) - (start)) * (float)(-2 * pow((t), 3) + 3 * pow((t), 2))

// Animation variables
int width = 1920;
int height = 1080;
float fps = 30.0;
int numLines;
float * speeds;
float * lineColors;

// Zoom controllers
float seconds_to_cross = 20.0f;
float freq = .25f;
float pong_time = 8.0f;

float clamp(float x){
	if(x > 1.0f)
		return 1.0f;
	else if(x < 0.0f)
		return 0.0f;
	else
		return x;
}

float pingpong(float t_in, float d){
	float t = fmod(t_in, 2*d);
	if(t > d){
		t = t - d;
		return 1.0f - t/d;
	}else{
		return t/d;
	}
}

void genImage(float t, float * pixels){
	// interpolate speeds
	float speed_interp;
	float newSpeed[numLines]; 
	for(int i = 0; i < numLines; i++) 
		newSpeed[i] = speeds[i];

	if(pong_time > 0.0f){
		speed_interp = pingpong(t, pong_time);
		for(int i = 0; i < numLines; i++) 
			newSpeed[i] = SMOOTH(1.0f, speeds[i], speed_interp);
	}
	


	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			// float x_off = (float)x/width;
			float thisColor[3];
			thisColor[0] = 0.0f; thisColor[1] = 0.0f; thisColor[2] = 0.0f;

			for(int i = 0; i < numLines; i++){
				float k = freq*2.0f*M_PI;
				float b = seconds_to_cross/(width * newSpeed[i]);
				float sample = sin(k*(t - x*b));
				sample = sample*0.5f + 0.5f;

				thisColor[0] += lineColors[i*3 + 0] * sample;
				thisColor[1] += lineColors[i*3 + 1] * sample;
				thisColor[2] += lineColors[i*3 + 2] * sample;
			}

			pixels[3*(y*width + x) + 0] = clamp(thisColor[0]) * 255.0f;
			pixels[3*(y*width + x) + 1] = clamp(thisColor[1]) * 255.0f;
			pixels[3*(y*width + x) + 2] = clamp(thisColor[2]) * 255.0f;
		}
	}
}

void solveColors(){
	lineColors = new float[numLines * 3];
	float component_sum = 0.0f;
	for(int i = 0; i < numLines; i++){
		float thisColor[3];
		float h = 360.0f*(float)i/numLines;
		HSVtoRGB(h, 1.0f, 1.0f, thisColor);
		lineColors[3*i + 0] = thisColor[0];
		lineColors[3*i + 1] = thisColor[1];
		lineColors[3*i + 2] = thisColor[2];
		component_sum += thisColor[0];
	}
	for(int i = 0; i < numLines*3; i++){
		lineColors[i] /= component_sum;
	}
	for(int i = 0; i < numLines; i++){
		std::cout << "colors " << i << "): [" << lineColors[i*3+0] << ", " << lineColors[i*3+1] << ", " << lineColors[i*3+2]<<"]\n";
	}
}

int main(){
	// Set variables
	numLines = 6;
	speeds = new float[]{.5, 1, 2, 4, 8, 16};
	solveColors();

	// Calculate important things
	float * pixels = new float[width * height * 3];
	float dt = 1.0f/fps;

	// Render!
	for(int frame_num = 0; frame_num < 16*30; frame_num++){
		genImage(frame_num*dt, pixels);
		std::string filename = "oven/reson_" + std::to_string(frame_num)+".ppm";
		writePPM(filename, width, height, pixels);
		if(frame_num % 30 == 0)
			std::cout << "rendered frame " << frame_num << std::endl;
	}

	return 1;
}