#include <iostream>
#include <string>
#include "FsFat32.hpp"

using namespace std;

int main(int argc, char** argv)
{
	string path;
	if (argc == 1) path = "C:/Users/user/Documents/GitHub/FAT32_Analysis/FAT32_simple.mdf";
	else path = argv[1];

	// open an image file
	FILE* fp = fopen(path.c_str(), "rb");

	// build a file system
	FsFat32 fs(fp);
	if (fs.build_file_system()) cout << "Successfully built a file system" << endl;
	else cout << "Error occured while building a file system" << endl;
	fclose(fp);

	//
	bool ing = true;
	while (ing)
	{
		int key = 0;
		cout << "Press the number you want to do \n (1: show all the files/ 2: export a file/ 3: quit)" << endl;
		cin >> key;

		switch (key)
		{
		case 1:
		{
			// show all the files and directory
			cout << "Show all the files" << endl;
			vector<Inode*> v;
			fs.show_all(fs.root_node, v);
			cout << endl;
			break;
		}

		case 2:
		{
			// export a file
			cout << "Which file do you want to export?" << endl;
			string file_path;
			cin >> file_path;
			if (fs.export_to(file_path)) cout << "Successfully export the file" << file_path << endl;
			else cout << "Error occured while exporting the file" << endl;
			break;
		}

		case 3:
		{
			ing = false;
			break;
		}

		default:
		{
			cout << key << "is not available key\n Please, press again" << endl;
			break;
		}

		}
	}

	// close an image file
	return 0;
}