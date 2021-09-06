#!/bin/bash

mkdir -p build

cd build

cmake ..

case "$OSTYPE" in
	solaris*)
		echo "> SOLARIS Operating System detected!"
		echo "   [Warning] Not suported. Returning..."
	;;
	darwin*)
		echo "> OSX Operating System detected!"
		echo "   [Warning] Not suported. Returning..."
	;; 
	linux*)
		echo "> LINUX Operating System detected!"
		cmake --build .
	;;
	bsd*)
		echo "> BSD Operating System detected!"
		echo "   [Warning] Not suported. Returning..."
	;;
	msys*)
		echo "> WINDOWS Operating System detected!" 
		cmake --build . --target ALL_BUILD --config Release -- /maxcpucount:12
	;;
	*)
		echo "unknown: $OSTYPE"
	;;
esac

cd ..
