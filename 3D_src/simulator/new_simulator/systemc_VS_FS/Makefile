LIB_DIR=-L../../../src_systemc/systemc_obj/src/sysc/.libs
LIB_DIR2=-L../../src_systemc/systemc_obj/src/sysc/.libs
INC_DIR=-I../../../src_systemc/systemc-2.3.0/src
INC_DIR2=-I../../src_systemc/systemc-2.3.0/src
LIB=-lsystemc
CXX=g++
CPPFLAG=-fpermissive -g


#all: IO_VS.o IO_FS.o run_shader.cpp
all: IO_VS.o IO_FS.o
	#$(CXX) -Wall $(CPPFLAG) SC_MAIN.cpp *.o -o test_shader $(LIB_DIR) $(LIB_DIR2) $(INC_DIR) $(INC_DIR2) $(LIB) -g
	$(CXX) -Wall $(CPPFLAG) SC_MAIN.cpp *.o -o test_shader $(LIB_DIR2) $(INC_DIR2) $(LIB) -g
#$(CXX) run_shader.cpp -o run_shader
IO_VS.o: src_VS6/IO_VS.cpp src_VS6/IO_VS.h src_VS6/SC_VS.cpp src_VS6/SC_VS.h src_VS6/clip.cpp src_VS6/clip.h testbench.h
	cd src_VS6/;g++ -c $(CPPFLAG) *.cpp $(INC_DIR) -g
	cp src_VS6/*.o .
IO_FS.o: src_FS5/IO_FS.cpp src_FS5/IO_FS.h src_FS5/FS.cpp src_FS5/FS.h testbench.h
	cd src_FS5/;g++ -c $(CPPFLAG) *.cpp $(INC_DIR) -g
	cp src_FS5/*.o .
clean:
	rm -rf test_shader *.o
	rm -rf src_VS6/*.o
	rm -rf src_FS5/*.o
	rm -rf see
	rm -rf output/*
