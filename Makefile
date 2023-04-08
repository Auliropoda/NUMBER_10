all: 
	mkdir -p buildfolder && cd buildfolder && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j4 && ./demo
clean: 
	rm -r buildfolder
	rm a.out
