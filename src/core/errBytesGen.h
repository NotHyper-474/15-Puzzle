#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}

/*
* Utility that reads a BMP file and converts it into hardcoded elements of a char array inside an header file
* Used to store an error texture inside the memory.
*/
void regenerate_errBytes()
{
	ifstream errImage("Resources/err.bmp", ios::binary);
	ofstream generatedFile("Resources/hardcoded_data.h", ios::out);
	vector<char> buffer;
	if (errImage)
	{
		generatedFile << "namespace Rendering\n{";
		generatedFile << "\nunsigned const static char errIMG[] = {\n";

		errImage.seekg(0, std::ios::end);
		const unsigned int len = errImage.tellg();
		errImage.seekg(0, std::ios::beg);

		buffer.resize(len);
		errImage.read(&buffer[0], len);

		for (size_t i = 0; i < buffer.size(); i++)
		{
			generatedFile << "\t\t0x" << n2hexstr(buffer[i]) << ",\n";
		}
		generatedFile << "\t};\n}";
		errImage.close();
	}
	else
	{
		generatedFile.clear();
		generatedFile.close();
	}
}