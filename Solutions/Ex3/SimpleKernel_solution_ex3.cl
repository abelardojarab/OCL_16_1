//ACL Kernel
__kernel 
void SimpleKernel(__global const float *restrict in, __global const float *restrict in2, __global float * restrict out)
{
	//Perform the Math Operation
	size_t i = get_global_id(0);
	out[i] = in[i] * in2[i];
}