set -e

export CLANGBIN=$HOME/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04/bin

export DIR=$HOME/llvm-project
export CMAKE='cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON ../llvm -DLLVM_ENABLE_PROJECTS="llvm;clang;compiler-rt"'

rm -rf $DIR/build-*

function is_bin_in_path {
  builtin type -P "$1" &> /dev/null
}

if is_bin_in_path clang; then
    echo "oops, please make sure clang is not in PATH";
    exit -1;
fi

if is_bin_in_path clang++; then
    echo "oops, please make sure clang++ is not in PATH";
    exit -1;
fi

# this LLVM should behave identically to its twin on the main branch
(
set -e
export PATH=$CLANGBIN:$PATH
mkdir $DIR/build-default
cd $DIR/build-default
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-default -DCMAKE_EXECUTABLE_SUFFIX='-default' -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1
ninja install
)

# this makes sure the default LLVM we just built can build a working LLVM
(
set -e
export PATH=$DIR/install-default/bin:$PATH
mkdir $DIR/build-default2
cd $DIR/build-default2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-peeps -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1
ninja install
)

exit 0;

# this LLVM only disables unsound optimizations, it fails tests that look for those optimizations
(
set -e
mkdir $DIR/build-disable-unsound
cd $DIR/build-disable-unsound
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-disable-unsound -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=true -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1
ninja install
)

# this makes sure the LLVM without unsound optimizations can build a working LLVM
(
set -e
mkdir $DIR/build-disable-unsound2
cd $DIR/build-disable-unsound2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-disable-unsound -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1
ninja install
)

size $DIR/build-peeps2/bin/clang
size $DIR/build-no-peeps2/bin/clang
