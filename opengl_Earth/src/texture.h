
#ifndef HW3_TEXTURE_H
#define HW3_TEXTURE_H


#include <string>

class Texture {
public:
	Texture(const char* filename, unsigned int shader, const char* texture_name, unsigned int texture_no);
	unsigned int textureID;	
private:
	float imageWidth, imageHeight;
};


#endif
