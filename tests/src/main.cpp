int outline_main(int argc, char** argv);
int shader_main(int argc, char** argv);
int obj_main(int argc, char** argv);

int main(int argc, char** argv)
{
	//outline_main(argc, argv);

	shader_main(argc, argv);
	
	//char* args[2] = { "", "..\\data\\chess_set.obj" };
	//obj_main(2, args);
}