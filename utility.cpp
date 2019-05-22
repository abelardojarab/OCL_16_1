// This file
#include "utility.h"
#include <math.h>


// 
unsigned char* get_binary(const char * name, size_t* length)
{
	FILE *fp=fopen(name, "rb");
	assert (fp != NULL);
	fseek (fp, 0, SEEK_END);
	*length = ftell (fp);
	unsigned char *binaries = (unsigned char*)malloc(sizeof(unsigned char) **length);
	rewind (fp);
	fread (binaries, *length, 1, fp);
	fclose (fp);
	return binaries;
}


void print_platform_info(cl_platform_id* myPlatform)
{
	cl_int err;
	//Grab Platform Info
	char myPlatformName[128];
	char myPlatformProfile[128];
	char myPlatformVersion[128];
	char myPlatformVendor[128];
	err = clGetPlatformInfo(*myPlatform, CL_PLATFORM_NAME, 128 * sizeof(char), myPlatformName, NULL);
	err = clGetPlatformInfo(*myPlatform, CL_PLATFORM_PROFILE, 128 * sizeof(char), myPlatformProfile, NULL);
	err = clGetPlatformInfo(*myPlatform, CL_PLATFORM_VERSION, 128 * sizeof(char), myPlatformVersion, NULL);
	err = clGetPlatformInfo(*myPlatform, CL_PLATFORM_VENDOR, 128 * sizeof(char), myPlatformVendor, NULL);
	printf ("\nPlatform Name: %s\n", myPlatformName);
	printf ("Platform Profile: %s\n", myPlatformProfile);
	printf ("Platform Version: %s\n", myPlatformVersion);
	printf ("Platform Vendor: %s\n", myPlatformVendor);
}


void print_device_info(cl_device_id* myDevice)
{
	cl_int err;
	//Get Device Properties
	char myDeviceVendor[128];
	cl_uint myDeviceMaxCU;
	cl_uint myDeviceMaxWID;
	char myDeviceName[128];
	cl_bool myDeviceAvailable;
	cl_uint myAddrBits;
	cl_bool myEndianLittle;
	cl_ulong myMemAllocSize;
	cl_uint myMaxFreq;
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_NAME, 128 * sizeof(char), myDeviceName, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_VENDOR, 128 * sizeof(char), myDeviceVendor, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &myDeviceMaxCU, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &myDeviceMaxWID, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &myDeviceAvailable, NULL);

	err = clGetDeviceInfo(*myDevice, CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint), &myAddrBits, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool), &myEndianLittle, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &myMemAllocSize, NULL);
	err = clGetDeviceInfo(*myDevice, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &myMaxFreq, NULL);
	
	printf ("\nDevice Name: %s\n", myDeviceName);
	printf ("Device Vendor: %s\n", myDeviceVendor);
	printf ("Is Device Available: %d\n", myDeviceAvailable);
	printf ("Is Device Little Endian: %d\n", myEndianLittle);
	printf ("Device Max Compute Units: %u\n", myDeviceMaxCU);
	printf ("Device Max Work Item Dimensions: %u\n", myDeviceMaxWID);
	printf ("Device Max Frequency: %u\n", myMaxFreq);
	printf ("Device Address Bits: %u\n", myAddrBits);
	printf ("Device Max Mem Alloc Size: %lu\n", myMemAllocSize);
}

void allocate_generate(void** X, void** Y, void** Z, cl_float LO, cl_float HI, size_t vectorSize)
{
	// allocate and initialize the input and output vectors
	*X = malloc(sizeof(cl_float)*vectorSize);
	*Y = malloc(sizeof(cl_float)*vectorSize);
	*Z = malloc(sizeof(cl_float)*vectorSize);


	//Assigns randome number from LO to HI to all locatoin of X and Y
	for (int i = 0; i < vectorSize; ++i) {
		((cl_float *) *X)[i] =  LO + (cl_float)rand()/((cl_float)RAND_MAX/(HI-LO));
		((cl_float *) *Y)[i] =  LO + (cl_float)rand()/((cl_float)RAND_MAX/(HI-LO));
	}
}

bool verification (void * X, void * Y, void * Z, void * CalcZ, size_t vectorSize)
{
	//Verify if OpenCL Calculation is Same as C Result
	for(int i = 0; i < vectorSize-4; i++) {
		if(fabs(((float*)CalcZ)[i] - ((float*)Z)[i]) > EPSILON) {
			 printf("\nVERIFICATION FAILED! index %d, X:%f, Y:%f, OpenCL Result:%f != Result %f)",
				  i, ((float*)X)[i], ((float*)Y)[i], ((float*)Z)[i], ((float*)CalcZ)[i]);
			 return false;
		}
    }

	// Print 10 Sample Data to Standard Out
	printf("\n\nVERIFICATION PASSED!!!\n\nSome Sample of Results\n");
	printf("------------------------------------\n");
	for (int i = 0; i < (int)vectorSize; i=i+((int)vectorSize)/5) {
		printf("Index %d: Input 1 is %f, Input 2 is %f, Result is %f\n", i, ((float*)X)[i], ((float*)Y)[i], ((float*)Z)[i]);
	}
	return true;
}
