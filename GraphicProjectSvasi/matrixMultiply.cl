__kernel void moltiplicazioneMatrici(__global int *A, __global int *B, __global int *C, int righeA, int colonneB) {

	// Get the index of the current element to be processed
	int i = get_global_id(0);
	int j = get_global_id(1);
	int val = 0;

	for (int k = 0; k < righeA; k++){
		val = val + A[k + j * righeA] * B[k * colonneB + i];
	}

	C[i + righeA * j] = val;
}