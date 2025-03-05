cd cmake-build-debug

cmake -DCMAKE_C_COMPILER=/software/gcc/13.1.0/bin/gcc -DCMAKE_CXX_COMPILER=/software/gcc/13.1.0/bin/g++ -DCMAKE_CXX_FLAGS=-I\ /home/utr_lefebvre/xsd-4.0.0-i686-linux-gnu/libxsd -DMOSEK_DIR=/home/utr_lefebvre/mosek/11.0/tools/platform/linux64x86 -DEIGEN_DIR=/home/utr_lefebvre/eigen-3.4.0 ..

make -j 6

cd ..

