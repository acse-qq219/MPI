MAKE = make
TARGET = exe_code_for_hpc
SOURCE = lect0.cpp

default:
	mpicxx -o $(TARGET) $(SOURCE)
