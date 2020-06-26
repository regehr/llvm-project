set -e

export SPEC=$HOME/cpu2017
export DIR=$HOME/llvm-project
export ARGS="--rebuild --input test 600.perlbench_s 602.gcc_s 605.mcf_s 620.omnetpp_s 623.xalancbmk_s 625.x264_s 631.deepsjeng_s 641.leela_s 657.xz_s"

cd $SPEC
. ./shrc

sed -e 's/LLVMDIR/\/home\/regehr\/llvm-project\/install-default2/g' < $DIR/souper-scripts/default.cfg > $SPEC/config/default.cfg
runcpu $ARGS

sed -e 's/LLVMDIR/\/home\/regehr\/llvm-project\/install-no-ub2/g' < $DIR/souper-scripts/default.cfg > $SPEC/config/default.cfg
runcpu $ARGS

sed -e 's/LLVMDIR/\/home\/regehr\/llvm-project\/install-no-ub-no-peeps2/g' < $DIR/souper-scripts/default.cfg > $SPEC/config/default.cfg
runcpu $ARGS

echo
echo =======
echo SUCCESS
echo =======
echo
