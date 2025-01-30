# regman
Learning about Orbital Mechanics

## Setting up project
git clone <project_url>
git submodule update --recursive

## Builiding Libraries
### Building EnTT
cd lib/entt
mkdir build
cd build
cmake ..
sudo make install

### Building Assimp
cd lib/assimp
cmake CMakeLists.txt
cmake --build .

## Compiling for Desktop
cd <project_root>
make clean
make run

## Compiling for the Web
make -f Makefile.em clean
make -f Makefile.em serve