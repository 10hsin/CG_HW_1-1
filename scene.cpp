#include "scene.h"
#include <iostream>
#include <fstream>
#include <string>

scene::scene()
{
	obj_num = 0;
}

scene::~scene()
{
	
}

void scene::load_scene(scene &scene, int test_scene)
{
	ifstream ifs;
	string	token;

	if (test_scene == 1)
		ifs.open("Scene\\TestScene1\\scene.scene");
	else if (test_scene == 2)
		ifs.open("Scene\\TestScene2\\scene2.scene");
	else
		cout << "File not found";


	while (ifs >> token)
	{
		if (token != "model")
			break;

		ifs >> token;
		if (test_scene == 1)
			token = "Scene\\TestScene1\\" + token;
		if (test_scene == 2)
			token = "Scene\\TestScene2\\" + token;

		object[obj_num] = new mesh(token.c_str());

		ifs >> scene.scale_value[obj_num].x >> scene.scale_value[obj_num].y >> scene.scale_value[obj_num].z;
		ifs >> scene.angle[obj_num] >> scene.rotation_axis_vector[obj_num].x >> scene.rotation_axis_vector[obj_num].y >> scene.rotation_axis_vector[obj_num].z;
		ifs >> scene.transfer_vector[obj_num].x >> scene.transfer_vector[obj_num].y >> scene.transfer_vector[obj_num].z;

		scene.obj_num++;
	}

	ifs.close();
	
}
