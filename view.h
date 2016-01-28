#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;


class view
{
	public:
		// variable
		struct 	vertex { float x, y, z;};
		vertex	eye;	// position of camera
		vertex	vat;	// position where camera look at
		vertex	vup;	// vector of camera
		int		fovy;	// field of vision in angle
		int		dnear;	// near plane
		int		dfar;	// far plane
		int		viewport_x, viewport_y;	// position of viewport
		int		viewport_width, viewport_height;	// size of view port

		// function
		view();
		~view();
		void load_view(view &view, int test_scene);
};