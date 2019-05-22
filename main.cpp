#include <math.h>
#include <CL/opencl.h>
#include "utility.h"
#include <AOCLUtils/aocl_utils.h>

using namespace aocl_utils;

static const size_t vectorSize = 4096; //must be evenly disible by workSize
static const size_t workSize = 256;

#define EXERCISE1

void cleanup() {
}

int main(void)
{
	cl_int err = CL_SUCCESS;

	//Setup Platform

	//Get Platform ID
	cl_platform_id myPlatform;
	////////////// Exercise 1 Step 2.3

	// Get the OpenCL platform.
	myPlatform = findPlatform("fpga");
  	if(myPlatform == NULL) {
		printf("ERROR: Unable to find Altera OpenCL platform.\n");
		return false;
	}

	assert(err==CL_SUCCESS);
	print_platform_info(&myPlatform);

	//Setup Device
	//Get Device ID
	cl_device_id myDevice;
	////////////// Exercise 1 Step 2.5
	err = 0;
	assert(err==CL_SUCCESS);
	print_device_info(&myDevice);

	//Create Context
	////////////// Exercise 1 Step 2.6 
	cl_context context = 0;
	assert(err==CL_SUCCESS);

	//Create Command queue
	////////////// Exercise 1 Step 2.7
	cl_command_queue queue = 0;
	assert(err==CL_SUCCESS);


	////////////// Exercise 1 Step 2.8
	cl_mem kernelIn = 0;
	assert(err==CL_SUCCESS);
	cl_mem kernelIn2 = 0;
	assert(err==CL_SUCCESS);
	cl_mem kernelOut = 0;
	assert(err==CL_SUCCESS);

	//Inputs and Outputs to Kernel, X and Y are inputs, Z is output
	void *X, *Y, *Z;

	//Allocates memory with value from 0 to 1000
	float LO= 0;   float HI=1000;
	allocate_generate(&X, &Y, &Z, LO, HI, vectorSize);
	//Create Buffers for input and output

	//Write data to device
	////////////// Exercise 1 Step 2.9
	err = 0;
	err = 0;
	clFinish(queue);
	assert(err==CL_SUCCESS);

#ifndef EXERCISE1
	// create the kernel
	const char *kernel_name = "SimpleKernel";

	size_t lengths;
	unsigned char* binaries = get_binary("SimpleKernel.aocx", &lengths);
	cl_int kernel_status;

	// Create the Program from the AOCX file.
	////////////////////// Exercise 2 Step 2.3    ///////////////////
	cl_program program =

	assert(err==CL_SUCCESS);

	// build the program
	//////////////      Compile the Kernel.... For Intel FPGA, nothing is done here, but this comforms to the standard
	//////////////       Exercise 2   Step 2.4    ///////////////////
	err = 0;
	assert(err==CL_SUCCESS);


	// create the kernel
	//////////////       Find Kernel in Program
	//////////////       Exercise 2   Step 2.5    ///////////////////
	cl_kernel kernel = 
	assert(err==CL_SUCCESS);

	//////////////     Set Arguments to the Kernels
	//////////////       Exercise 2   Step 2.6    ///////////////////
	err = 0;
	assert(err==CL_SUCCESS);
	err = 0;
	assert(err==CL_SUCCESS);
	err = 0;
	assert(err==CL_SUCCESS);
	err = 0;
	assert(err==CL_SUCCESS);


	printf("\nLaunching the kernel...\n");

	// Launch Kernel
	//////////////       Exercise 2   Step 2.7    ///////////////////
	err = 0;
	assert(err==CL_SUCCESS);

	// read the output
	//////////////       Exercise 2   Step 2.8    ///////////////////
	err = 0;
	assert(err==CL_SUCCESS);

	void * CalcZ = malloc(sizeof(float)*vectorSize);

	for (int i=0; i<vectorSize; i++)
	{
		//////////////  Equivalent Code runnign on CPUs
		//////////////       Exercise 2   Step 2.9    ///////////////////
		((float*) CalcZ)[i]=

	}

	//Print Performance Results

	verification (X, Y, Z, CalcZ, vectorSize);

	// Clean up Stuff
	if(kernel)
		clReleaseKernel(kernel);
	if(program)
		clReleaseProgram(program);
	if(queue)
		clReleaseCommandQueue(queue);
	if(context)
		clReleaseContext(context);
	if(kernelIn)
		clReleaseMemObject(kernelIn);
	if(kernelOut)
		clReleaseMemObject(kernelOut);
	if(X)
		free (X);
	if(Y)
		free (Y);
	if(Z)
		free (Z);
	if(CalcZ)
		free (CalcZ);

#endif

    return 1;
}
