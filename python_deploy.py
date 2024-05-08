import tvm
from tvm import relay
import numpy as np

def deploy_resnet18(mod, input_data):
    # Load the graph executor module
    graph_executor = mod["default"](tvm.cpu())

    # Get the input and output names
    input_name = "data"
    output_name = "softmax"

    # Get the graph executor's set_input and get_output functions
    set_input, get_output, run = graph_executor["set_input"], graph_executor["get_output"], graph_executor["run"]

    # Set the input data
    set_input(input_name, input_data)

    # Run the model
    run()

    # Get the output
    output = get_output(0)

    # Convert output to numpy array
    output_np = output.asnumpy()

    # Add your post-processing logic here

    # Example: Print the first 10 elements of the output
    print(output_np[0][:10])

if __name__ == "__main__":
    # Load the ResNet18 module
    mod_dylib = tvm.runtime.load_module("lib/test_resnet18.so")
    
    # Define input data (replace with your input data preparation logic)
    # For simplicity, set some dummy data here
    input_data = tvm.nd.array(np.ones((1, 3, 224, 224), dtype=np.float32))

    # Deploy ResNet18 model
    deploy_resnet18(mod_dylib, input_data)
