# tvm-riscv-deploy
This repository demonstrates how to deploy the TVM compiler on a RISC-V architecture using ResNet18.

## Step 1: Setting up TVM
Follow these steps, which are adapted from the [TVM Install from Source](http://tvm.apache.org/docs/install/from_source.html) page.

### Clone the repository
```bash
git clone --recursive --branch int4_direct_HWNC http://github.com/zachzzc/incubator-tvm.git ~/tvm
```

### Install LLVM
```bash
sudo bash -c "$(wget -O - http://apt.llvm.org/llvm.sh)"
```

### Install build dependencies
```bash
sudo apt-get update
sudo apt-get install -y python3 python3-dev python3-setuptools gcc libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev
```

### Build TVM
Navigate to the TVM directory, create a build directory, and copy the `config.cmake` file.
```bash
cd ~/tvm
mkdir build
cp cmake/config.cmake build
```

Edit `build/config.cmake` to customize compilation options:
- Change `set(USE_LLVM OFF)` to `set(USE_LLVM /path/to/llvm-config)` to build with LLVM (replace `/path/to/llvm-config` with the actual path, such as `/usr/lib/llvm-10/bin/llvm-config`).

Then compile TVM:
```bash
cd build
cmake ..
make -j4
```

### Install the TVM Python package
Add the following lines to `~/.bashrc`:
```bash
export TVM_HOME=/path/to/tvm
export PYTHONPATH=$TVM_HOME/python:$TVM_HOME/topi/python:$TVM_HOME/nnvm/python:${PYTHONPATH}
```
Source `~/.bashrc` to apply the changes:
```bash
source ~/.bashrc
```

### Install Python dependencies
Install the necessary dependencies:
```bash
pip3 install --user numpy decorator attrs
```

## Step 2: Install RISC-V GNU Compiler Toolchain
```bash
sudo apt update
sudo apt install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
```

Compile the TVM runtime for RISC-V:
```bash
riscv64-linux-gnu-g++ -c -std=c++17 -O2 -fPIC -I/home/tvm/include -I/home/tvm/3rdparty/dmlc-core/include -I/home/tvm/3rdparty/dlpack/include -DDMLC_USE_LOGGING_LIBRARY=\<tvm/runtime/logging.h\> -o lib/libtvm_runtime_pack.o  tvm_runtime_pack.cc
```
This will generate the `libtvm_runtime_pack.o` file.

## Step 3: Generate `test_resnet18.so`
Use `prepare_test_libs.py` to generate the `test_resnet18.so` file.

## Step 4: Compile the Deployment Package
```bash
riscv64-linux-gnu-g++ -std=c++17 -O2 -fPIC -I/home/tvm/include -I/home/tvm/3rdparty/dmlc-core/include -I/home/tvm/3rdparty/dlpack/include -DDMLC_USE_LOGGING_LIBRARY=\<tvm/runtime/logging.h\> -o lib/cpp_deploy_pack cpp_deploy.cc lib/test_resnet18.so lib/libtvm_runtime_pack.o -L/home/tvm/build -ldl -pthread
```
This will create the `cpp_deploy_pack` executable.

## Step 5: Running on RISC-V Processor or Emulator
If you do not have a RISC-V processor to execute, you can use the QEMU emulator. For more information, please read `Qemu_usage.md`.

Transfer the `cpp_deploy_pack` executable and `lib/test_resnet18.so` to `ubuntu_riscv64` and execute to get the results.

With these steps, you have successfully deployed ResNet18 using the TVM compiler on a RISC-V processor and performed an inference.
```

Please replace `/path/to/tvm` and `/path/to/llvm-config` with the actual paths in your environment. Also, ensure to check the `Qemu_usage.md` for details on using the QEMU emulator.
