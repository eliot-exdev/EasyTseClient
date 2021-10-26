# About
Simple C++ library for communicating with the EasyTse middleware (see https://www.tse-easy.de)  
Currently this library is heavily under development.  
Supported TSE features:
- Ping
- StartTransaction

# Requirements
- CMake 3.18 (https://cmake.org)
- Boost 1.74 (https://www.boost.org)

# Build
mkdir build  
cd build  
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=<path/to/your/boost/installation>  
cmake --build .  

# Create a server socket for testing
netcat -k -l 4444
