#include "view.h"
#include <iostream>
#include <fstream>
#include <string>

view::view()
{

}

view::~view()
{
	
}

void view::load_view(view &view, int test_scene)
{
	ifstream ifs;
	string	token;

	if (test_scene == 1)
		ifs.open("Scene\\TestScene1\\view.view");
	else if (test_scene == 2)
		ifs.open("Scene\\TestScene2\\scene2.view");
	else
		cout << "File not found";

	ifs >> token >> eye.x >> eye.y >> eye.z;
	ifs >> token >> vat.x >> vat.y >> vat.z;
	ifs >> token >> vup.x >> vup.y >> vup.z;
	ifs >> token >> fovy;
	ifs >> token >> dnear;
	ifs >> token >> dfar;
	ifs >> token >> viewport_x >> viewport_y >> viewport_width >> viewport_height;

	ifs.close();
	
}