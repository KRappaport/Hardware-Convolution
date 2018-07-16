# Hardware-Convolution
Examine convolution used for image processing and implement them on hardware to increase efficiency both in terms of timing and power consumption.

### Development Process  
The development process has two main parts:
    1.  Test the timing to perform the convolution on a CPU.  This step was performed in MATLAB and the relevant code can be found in the folder [MATLAB_Code](https://github.com/KRappaport/Hardware-Convolution/tree/master/MATLAB_Code).
    2.  Implementing the convolution on an FPGA.  The [ZedBoard](http://zedboard.org/product/zedboard) was used to implement and test the design which was done using the [Vivado Design Suite](https://www.xilinx.com/products/design-tools/vivado.html).  The relevant code can be found in the folder [FPGA_Code](https://github.com/KRappaport/Hardware-Convolution/tree/master/FPGA_Code).

### Motivation
The motivation for this project is to design an efficient method to perform the convolution necessary for image processing that can be implemented on an FPGA.  The intention is that it will be possible to use the design in a convolutional neural network for the purpose of machine learning.  
