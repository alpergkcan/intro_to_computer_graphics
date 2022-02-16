#ifndef HW3_INPUT_H
#define HW3_INPUT_H

#include <vector>

class Input {
  public:
	static Input* I;
  public:
	bool getKey(unsigned int glfw_key_code);
	void KeyboardInput();
	
  public:
	Input();
	std::vector<bool>* keys;
};

#endif

