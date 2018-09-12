__kernel
void matrixMultiply(__global int* A, __global int* B, __global int* C, int heightA, int heightB)
{
	int i = get_global_id(0);
	int j = get_global_id(1);
	int value = 0;

	for (int k = 0; k < heightA; k++)
	{
		value = value + A[k + i * heightA] * B[k*heightB + j];
	}
	C[j + heightB * i] = value;
}