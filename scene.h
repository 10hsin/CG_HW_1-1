#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "mesh.h"
using namespace std;

class scene
{
	public:
		// variable
		struct vertex
		{
			float x, y, z;
		};
		
		float	angle[6];
		vertex	rotation_axis_vector[6];
		vertex	scale_value[6];
		int		obj_num;
		vertex	transfer_vector[6];
		mesh	*object[6];
		
		// function
		scene();
		~scene();
		void load_scene(scene &scene, int test_scene);
};