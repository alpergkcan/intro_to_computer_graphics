hw3: configure compile mv-exec

clean:
	rm -rf ./build/ ./hw3

configure:
	cmake -DCMAKE_BUILD_TYPE=Release -S ./src/ -B ./build/

compile:
	cd ./build; make;

mv-exec:
	cp ./build/hw3 ./
