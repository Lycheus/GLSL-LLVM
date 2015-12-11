LIB_DIR=-L/usr/systemc/lib-linux64
INC_DIR=-I/usr/systemc/include
LIB=-lsystemc
CXX=g++


all: IO_VS.o IO_FS.o run_shader.cpp
	$(CXX) SC_MAIN.cpp *.o -o test_shader $(LIB_DIR) $(INC_DIR) $(LIB)
	$(CXX) run_shader.cpp -o run_shader
IO_VS.o: src_VS6/IO_VS.cpp src_VS6/IO_VS.h src_VS6/SC_VS.cpp src_VS6/SC_VS.h src_VS6/clip.cpp src_VS6/clip.h
	cd src_VS6/;g++ -c *.cpp $(LIB_DIR) $(INC_DIR) $(LIB)
	cp src_VS6/*.o .
IO_FS.o: src_FS5/IO_FS.cpp src_FS5/IO_FS.h src_FS5/FS.cpp src_FS5/FS.h
	cd src_FS5/;g++ -c *.cpp $(LIB_DIR) $(INC_DIR) $(LIB)
	cp src_FS5/*.o .
clean:
	rm -rf test_shader *.o
