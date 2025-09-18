## How it works

### print_legendre_roots.py
First, you must create a csv list of the desired number of gauss quadrature points. This can be done by: 

    $> python3 print_legendre_roots.py 160

Where in this example, the gauss quad weights and abcissa for N = 1,...,160 will be printed to the file 'gauss_quad_points_160.dat'. 

Then all of the numerical integration / plotting is done in cpp. 
In the CMakeLists.txt file, you can see all the relevant cpp source files to use. 

### integrators.cpp/.hpp 
This header / implementation file contains the implementation of 'trapez' and 'simpson', which are the trapezoid and simpson numerical integration rules, respectivley. 

These files also contain the definition of the **GaussInt** class. The **GaussInt** class has two jobs. First, on its construction, it attempts to parse the gauss quad points from the '.dat' file created by the python script above. Then, if these points are parsed successfully, it can perform gauss-quad numerical integration via the **GaussInt::Integral()** method. 

Note that when I wrote this a week ago, I didn't know how to use function pointers in C++, so I just used the std::function<> template from the <functional> lib to pass functions.

### test_integration.cpp
This is where the 'main()' routine actually tests the different numiercal integration methods. This is done by first creating an array of N-values, which can be done with linear-spacing with **Get_integration_steps** (done for gauss quads), or with log-spacing with **Get_integration_steps_exponential** (done for simpson and trapez). 

There are a lot of parameters you can edit in 'test_integrators.cpp' if you want to change the function being evaluated, you can change the definition of the **fcn(double)**, function, and the corresponding **fcn_indef_integral(double)**, which is the corresponding indefinite integral. 




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
