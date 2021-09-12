cd libtins
mkdir build
cd build
cmake ../ -DLIBTINS_ENABLE_CXX11=1
make
sudo make install
sudo ldconfig
