set -e -x

CLANGBIN=$HOME/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04/bin

DIR=$HOME/llvm-project
CMAKE='cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON ../llvm -DLLVM_ENABLE_PROJECTS="llvm;clang;compiler-rt"'

rm -rf $DIR/build-*
rm -rf $DIR/install-*

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
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-default -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

# this makes sure the default LLVM we just built can build a working LLVM
(
set -e
export PATH=$DIR/install-default/bin:$PATH
mkdir $DIR/build-default2
cd $DIR/build-default2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-default2 -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

# this LLVM avoids optimizations that introduce UB, it will fail tests that look for those optimizations
(
set -e
export PATH=$CLANGBIN:$PATH
mkdir $DIR/build-no-ub
cd $DIR/build-no-ub
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-ub -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=true -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

# this makes sure the no-UB LLVM we just built can build a working LLVM
(
set -e
export PATH=$DIR/install-no-ub/bin:$PATH
mkdir $DIR/build-no-ub2
cd $DIR/build-no-ub2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-ub2 -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

# this LLVM avoids optimizations that do peephole-like things and that
# introduce UB, it will fail tests that look for those optimizations
(
set -e
export PATH=$CLANGBIN:$PATH
mkdir $DIR/build-no-ub-no-peeps
cd $DIR/build-no-ub-no-peeps
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-ub-no-peeps -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=true -DDISABLE_PEEPHOLES_DEFAULT_VALUE=true' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

# this makes sure the no-UB, no-peephole LLVM we just built can build a working LLVM
(
set -e
export PATH=$DIR/install-no-ub-no-peeps/bin:$PATH
mkdir $DIR/build-no-ub-no-peeps2
cd $DIR/build-no-ub-no-peeps2
$CMAKE -DCMAKE_INSTALL_PREFIX=$DIR/install-no-ub-no-peeps2 -DCMAKE_CXX_FLAGS='-DDISABLE_WRONG_OPTIMIZATIONS_DEFAULT_VALUE=false -DDISABLE_PEEPHOLES_DEFAULT_VALUE=false' > cmake.out 2>&1
ninja > build.out 2>&1
ninja check > check.out 2>&1 || true
ninja install
)

size $DIR/install-default2/bin/clang-12 $DIR/install-no-ub2/bin/clang-12 $DIR/install-no-ub-no-peeps2/bin/clang-12

grep 'Failed Tests' build-default2/check.out build-no-ub2/check.out build-no-ub-no-peeps2/check.out

exit 0
