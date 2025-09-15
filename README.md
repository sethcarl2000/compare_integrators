### Compiling

Clone the github repo to a new folder. cd into the folder and make an new repo called 'build'. then you can build it with CMake: 

    $> mkdir build
    $> cmake -B build -S .
    $> cmake --build build

then, you can create a 'link' to the executable, which is in your new 'build' folder: 

    $> ln -s test_integrators build/test_integrators

You can then execute it with: 

    $> ./test_integrators

### Editing
Most of the parameters that you might want to change can be found in 'test_integrators.cpp'. Before the 'main' function, you can see lots of parameters that you can mess with. 
