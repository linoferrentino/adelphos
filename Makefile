TAG=$(shell date +%y_%m_%d_%H_%M)
GITREV=$(shell git rev-parse HEAD 2>/dev/null || echo "nogit")
PWD=$(shell pwd)

all: debug

src/cmake_cache.m4: src/cmake_cache_proto.m4
	cp -u src/cmake_cache_proto.m4 src/cmake_cache.m4

src/cmake_cache.txt: src/cmake_cache.m4
	m4 src/cmake_cache.m4 > src/cmake_cache.txt

debug: src/cmake_cache.txt
	mkdir -p build
	mkdir -p build_debug
	cd build_debug; cmake -C ../src/cmake_cache.txt -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH=$(PWD)/build ../src -DGITREV=$(GITREV) -DADELPHOSTAG=$(TAG) 
	cd build_debug; make
	cd build_debug; make install



clean:
	rm -rf build build_debug test_data


