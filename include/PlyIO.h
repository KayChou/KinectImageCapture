#ifndef PLYIO_H_
#define PLYIO_H_
#include <stdio.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

static const unsigned int MAX_COMMENT_SIZE = 256;

struct Point3f { 
    Point3f() {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }
    float X, Y, Z;
};

struct TriIndex {
	TriIndex() {
		this->v1 = 0;
		this->v2 = 0;
		this->v3 = 0;
	}
	int v1, v2, v3;
};

struct RGB {
    RGB() {
        this->R = 0;
        this->G = 0;
        this->B = 0;
    }
	unsigned char R, G, B;
};

enum PLYFormat {
    BINARY_BIG_ENDIAN_1,
    BINARY_LITTLE_ENDIAN_1,
    ASCII_1
};

template <class T>
void bigLittleEndianSwap (T * v, unsigned int numOfElements);

bool parasePlyHeader(std::ifstream &in, const char* filename, unsigned int &vn, unsigned int &fn, 
                   PLYFormat &format, unsigned int &numVertProperties, bool &hasColor, int &headerSize);
bool readPlyFile(const char* filename, std::vector<Point3f> &verts, std::vector<RGB> &colors);
void savePlyFile(const char* filename, std::vector<Point3f> vertices, bool BINARY=true, std::vector<RGB> colors=std::vector<RGB>());

#endif
