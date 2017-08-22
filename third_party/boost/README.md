wget 'https://dl.bintray.com/boostorg/release/1.65.0/source/boost_1_65_0.zip'
./bootstrap.sh --with-libraries=all --with-toolset=gcc
./b2 install link=static --prefix=$PWD/boost_lib
