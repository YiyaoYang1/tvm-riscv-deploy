#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/registry.h>

#include <cstdio>

void DeployResNet18() {
  // load the ResNet18 library
  DLDevice dev{kDLCPU, 0};
  tvm::runtime::Module mod_factory = tvm::runtime::Module::LoadFromFile("lib/test_resnet18.so");
  
  // create the ResNet18 module
  tvm::runtime::Module resnet18_mod = mod_factory.GetFunction("default")(dev);
  tvm::runtime::PackedFunc set_input = resnet18_mod.GetFunction("set_input");
  tvm::runtime::PackedFunc get_output = resnet18_mod.GetFunction("get_output");
  tvm::runtime::PackedFunc run = resnet18_mod.GetFunction("run");

  // Use the C++ API
  // Replace the input size and data type according to your ResNet18 model
  tvm::runtime::NDArray input = tvm::runtime::NDArray::Empty({1, 3, 224, 224}, DLDataType{kDLFloat, 32, 1}, dev);
  tvm::runtime::NDArray output = tvm::runtime::NDArray::Empty({1, 1000}, DLDataType{kDLFloat, 32, 1}, dev);

  // Set input data (replace with your input data preparation logic)
  // For simplicity, set some dummy data here
  float* input_data = static_cast<float*>(input->data);
  for (int i = 0; i < 3 * 224 * 224; ++i) {
    input_data[i] = 1.0;
  }

  // Set the input
  set_input("data", input);
  
  // Run the ResNet18 model
  run();

  // Get the output
  get_output(0, output);

  // Add your post-processing logic here

  // Example: Print the first 10 elements of the output
  for (int i = 0; i < 10; ++i) {
    printf("%f ", static_cast<float*>(output->data)[i]);
  }
  printf("\n");
}

int main(void) {
  DeployResNet18();
  return 0;
}
