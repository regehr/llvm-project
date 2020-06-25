set -e

export DIR=$HOME/llvm-project
export CMAKE='cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON ../llvm -DLLVM_ENABLE_PROJECTS="llvm;clang;compiler-rt"'

rm -rf $DIR/build-*

# this LLVM should behave identically to its twin on the main branch
(
set -e
mkdir $DIR/build-default
cd $DIR/build-default
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-default -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
# only fails if LLVM is broken
ninja check > check.out 2>&1
ninja install
)

# this makes sure our default LLVM can build a working LLVM
(
set -e
export PATH=$DIR/install-default/bin:$PATH
mkdir $DIR/build-default2
cd $DIR/build-default2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-peeps -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
# only fails if LLVM is broken
ninja check > check.out 2>&1
ninja install
)

(
set -e
export PATH=$DIR/install-peeps/bin:$PATH
mkdir $DIR/build-peeps2
cd $DIR/build-peeps2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-peeps2 > cmake.out 2>&1
ninja > build.out 2>&1
# only fails if LLVM is broken
ninja check > check.out 2>&1
ninja install
)

(
set -e
mkdir $DIR/build-no-peeps
cd $DIR/build-no-peeps
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-peeps > cmake.out 2>&1
ninja > build.out 2>&1
# don't test this one! it fails on a huge number of tests, by design
ninja install
)

(
set -e
export PATH=$DIR/install-no-peeps/bin:$PATH
mkdir $DIR/build-no-peeps2
cd $DIR/build-no-peeps2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-peeps2 > cmake.out 2>&1
ninja > build.out 2>&1
# this is the risky one!
ninja check > check.out 2>&1
ninja install
)

size $DIR/build-peeps2/bin/clang
size $DIR/build-no-peeps2/bin/clang
