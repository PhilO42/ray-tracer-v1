#install:
sudo apt-get install cmake libqt4-dev libqt4-core libqt4-gui qt4-dev-tools g++

#build
cmake -G"Unix Makefiles"
make -j

#run
./bin/ray_tracer
