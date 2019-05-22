#include <math.h>

#include "CL\opencl.h"
#include "utility.h"

static const size_t vectorSize = 4096; //must be evenly disible by workSize
static const size_t workSize = 256;

//#define EXERCISE1

int main(void)
{
	cl_int err;

	//Setup Platform

	//Get Platform ID
	cl_platform_id myPlatform;
	////////////// Exercise 1 Step 2.3
	err = clGetPlatformIDs(1, &myPlatform, NULL);
	assert(err==CL_SUCCESS);
	print_platform_info(&myPlatform);

	//Setup Device
	//Get Device ID
	cl_device_id myDevice;
	////////////// Exercise 1 Step 2.5
	err = clGetDeviceIDs(myPlatform, CL_DEVICE_TYPE_ALL, 1, &myDevice, NULL);
	assert(err==CL_SUCCESS);
	print_device_info(&myDevice);
	
	//Create Context
	////////////// Exercise 1 Step 2.6 
	cl_context context = clCreateContext(0, 1, &myDevice, NULL, NULL, &err);
	assert(err==CL_SUCCESS);
	
	//Create Command queue
	////////////// Exercise 1 Step 2.7
	cl_command_queue queue = clCreateCommandQueue(context, myDevice, 0, &err);
	assert(err==CL_SUCCESS);


	////////////// Exercise 1 Step 2.8
	cl_mem kernelIn = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * vectorSize, NULL, &err);
	assert(err==CL_SUCCESS);
	cl_mem kernelIn2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * vectorSize, NULL, &err);
	assert(err==CL_SUCCESS);
	cl_mem kernelOut = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * vectorSize, NULL, &err);
	assert(err==CL_SUCCESS);
	
	//Inputs and Outputs to Kernel, X and Y are inputs, Z is output
	void *X, *Y, *Z;
	
	//Allocates memory with value from 0 to 1000
	cl_float LO= 0;   cl_float HI=1000;
	allocate_generate(&X, &Y, &Z, LO, HI, vectorSize);
	//Create Buffers for input and output

	//Write data to device
	////////////// Exercise 1 Step 2.9
	err = clEnqueueWriteBuffer(queue, kernelIn, CL_FALSE, 0, sizeof(cl_float) * vectorSize, X, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(queue, kernelIn2, CL_FALSE, 0, sizeof(cl_float) * vectorSize, Y, 0, NULL, NULL);
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
	cl_program program = clCreateProgramWithBinary(context, 1, &myDevice, &lengths, (const unsigned char**)&binaries, &kernel_status, &err);

	assert(err==CL_SUCCESS);
	  
	// build the program
	//////////////      Compile the Kernel.... For Intel FPGA, nothing is done here, but this comforms to the standard
	//////////////       Exercise 2   Step 2.4    ///////////////////
	err = clBuildProgram(program, 1, &myDevice, "", NULL, NULL);
	assert(err==CL_SUCCESS);


	// create the kernel
	//////////////       Find Kernel in Program
	//////////////       Exercise 2   Step 2.5    ///////////////////
	cl_kernel kernel = clCreateKernel(program, kernel_name, &err);
	assert(err==CL_SUCCESS);

	//////////////     Set Arguments to the Kernels
	//////////////       Exercise 2   Step 2.6    ///////////////////
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&kernelIn);
	assert(err==CL_SUCCESS);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&kernelIn2);
	assert(err==CL_SUCCESS);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&kernelOut);
	assert(err==CL_SUCCESS);
	err = clSetKernelArg(kernel, 3, sizeof(cl_uint), (void*)&vectorSize);
	assert(err==CL_SUCCESS);


	printf("\nLaunching the kernel...\n");
	
	// Launch Kernel
	//////////////       Exercise 2   Step 2.7    ///////////////////
	err = clEnqueueTask(queue, kernel, 0, NULL, NULL);
	assert(err==CL_SUCCESS);

	// read the output
	//////////////       Exercise 2   Step 2.8    ///////////////////
	err = clEnqueueReadBuffer(queue, kernelOut, CL_TRUE, 0, sizeof(cl_float) * vectorSize, Z, 0, NULL, NULL);
	assert(err==CL_SUCCESS);
	
	void * CalcZ = malloc(sizeof(float)*vectorSize);

	for (int i=0; i<vectorSize; i++)
	{
		//////////////  Equivalent Code runnign on CPUs
		//////////////       Exercise 2   Step 2.9    ///////////////////
		((float*) CalcZ)[i]= ((float*) CalcZ)[i]=((float*) X)[i] * ((float*) Y)[i]; 
				
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
	char str[80];
	printf ("\nEnter any character: ");
	scanf ("%79s", str);
    return 1;
}
