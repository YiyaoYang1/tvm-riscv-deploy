# tvm-riscv-deploy
A demo about how to deploy TVM compiler on RISC-V with Resnet18.

step1:
A lot of these steps are taken from the TVM Install from Source page.

Clone and checkout the specific branch in the following repo.
git clone --recursive --branch int4_direct_HWNC http://github.com/zachzzc/incubator-tvm.git ~/tvm
Install LLVM.

sudo bash -c "$(wget -O - http://apt.llvm.org/llvm.sh)"
Install build dependencies.

sudo apt-get update
sudo apt-get install -y python3 python3-dev python3-setuptools gcc libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev
Change directory to the TVM directory, create a build directory and copy cmake/config.cmake to the directory.

cd ~/tvm
mkdir build
cp cmake/config.cmake build
Edit build/config.cmake to customize compilation options:

Change set(USE_LLVM OFF) to set(USE_LLVM /path/to/llvm-config) to build with LLVM. The /path/to/llvm-config should be something like /usr/lib/llvm-10/bin/llvm-config.

cd build
cmake ..
make -j4
Install the TVM Python package by appending the following lines to ~/.bashrc:

export TVM_HOME=/path/to/tvm
export PYTHONPATH=$TVM_HOME/python:$TVM_HOME/topi/python:$TVM_HOME/nnvm/python:${PYTHONPATH}
and then source ~/.bashrc to apply the changes.

Install Python dependencies.

Necessary dependencies: pip3 install --user numpy decorator attrs

step2:
'''bash
riscv64-linux-gnu-g++ -c -std=c++17 -O2 -fPIC -I/home/tvm/include -I/home/tvm/3rdparty/dmlc-core/include -I/home/tvm/3rdparty/dlpack/include -DDMLC_USE_LOGGING_LIBRARY=\<tvm/runtime/logging.h\> -o lib/libtvm_runtime_pack.o  tvm_runtime_pack.cc
'''
得到libtvm_runtime_pack.o文件，它就是TVM runtime for risc-v

step3:
用prepare_test_libs.py生成test_resnet18.so文件
step4:
'''bash
riscv64-linux-gnu-g++ -std=c++17 -O2 -fPIC -I/home/tvm/include -I/home/tvm/3rdparty/dmlc-core/include -I/home/tvm/3rdparty/dlpack/include -DDMLC_USE_LOGGING_LIBRARY=\<tvm/runtime/logging.h\> -o lib/cpp_deploy_pack cpp_deploy.cc lib/test_resnet18.so lib/libtvm_runtime_pack.o -L/home/tvm/build -ldl -pthread
'''bash
得到cpp_deploy_pack可执行文件

step5:
如果没有RISC-V处理器用于执行，可以使用qemu模拟器，相关信息请阅读Qemu_usage.md
把cpp_deploy_pack可执行文件和lib/test_resnet18.so放到ubuntu_riscv64里，执行得到结果
至此，我们完成了将ResNet18通过TVM编译器部署在RISC-V处理器上，执行一次inference
