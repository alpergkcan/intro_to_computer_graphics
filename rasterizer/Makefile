all : configure compile

configure:
	cmake -S ./src -B ./build -DCMAKE_BUILD_TYPE=Release 

compile:
	cd ./build; make;
