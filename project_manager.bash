#!/bin/bash

PROJECT_PATH=$PWD


#
# Checking flags
#

CLEAN=false
BUILD=false
HELP=false

while test $# -gt 0; do
    case "$1" in
        -c|-clean )
            CLEAN=true
            shift
        ;;
        -b|-build )
            BUILD=true
            shift
        ;;
        -h|-help )
            HELP=true
            shift
        ;;
        * )
            break; 
        ;;
    esac
done

#
# Flags execution
#

if [ "$CLEAN" = true ]; then
	echo "> Cleanning the project..."

    # Checks if there is already a build folder
    if [ -d "$PROJECT_PATH/build/" ]; then
        # Deletes all the contents of build folder
        rm -rfv build/
	fi

	# Checks if there is already a bin folder
    if [ -d "$PROJECT_PATH/bin/" ]; then
        # Deletes all the contents of build folder
        rm -rfv bin/
	fi

	# Checks if there is already a lib folder
    if [ -d "$PROJECT_PATH/lib/" ]; then
        # Deletes all the contents of build folder
        rm -rfv lib/
	fi

    echo "> Project cleaned!"
fi

if [ "$BUILD" = true ]; then
    echo "> Building the project..."

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

    echo "> Project built!"

    UPDATE=true
fi

if ([ "$CLEAN" = false ] && [ "$BUILD" = false ]) || [ "$HELP" = true ]; then
    echo "> Nothing done. Please specify a flag:"
    echo "   - Clean: (-c / -clean) Clears the project directories."
    echo "   - Build: (-b / -build) Builds the project."
    echo "   - Help: (-h / -help) Prints the help text :D"
fi
