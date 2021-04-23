
GXX49_VERSION := $(shell g++-4.9 --version 2>/dev/null)

ifdef GXX49_VERSION
	CXX_COMMAND := g++-4.9
else
	CXX_COMMAND := g++
endif

FILENAME ?= $(shell bash -c 'read -p "Input Filename: " file; echo $$file')

OUTPUT_DIRECTORY := outputFiles/

OUTPUT_FILENAME ?= $(shell bash -c 'read -p "Output Filename: " ofile; echo $$ofile')

CXX = ${CXX_COMMAND} -std=c++17

all: run_testC run_testV

run_testC: calculate
	./calculate ${FILENAME}

run_testV: verify
	./verify ${FILENAME}

calculate: calculateAlgorithm.cpp
	${CXX} calculateAlgorithm.cpp -o calculate

verify: verifyAlgorithm.cpp
	${CXX} verifyAlgorithm.cpp -o verify 

outputC: calculate
	./calculate ${FILENAME} > ${OUTPUT_DIRECTORY}${OUTPUT_FILENAME}
	echo "\nThe file was saved into the outputFiles folder.\n"

outputV: verify
	./verify ${FILENAME} > ${OUTPUT_DIRECTORY}${OUTPUT_FILENAME}
	echo "\nThe file was saved into the outputFiles folder.\n"

clean:
	rm -f calculate verify
