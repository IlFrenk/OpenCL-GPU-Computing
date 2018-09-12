#include < stdio.h >
#include < stdlib.h >
#include < time.h >
#include < string.h>
#include < math.h >

#ifdef APPLE
#include < OpenCL/opencl.h >
#else
#include < CL/cl.h >
#endif

#define MAX_SOURCE_SIZE (0x100000)

int widthA = 0;
int heightA = 0;

int widthB = 0;
int heightB = 0;

int widthC = 0;
int heightC = 0;

int main()
{
	//Asking to the user matrix indexes

	/*printf("Dimmi le righe di A: ");
	scanf("%d", &widthA);

	printf("Dimmi le colonne di A: ");
	scanf("%d", &heightA);

	printf("Dimmi le colonne di B: ");
	scanf("%d", &heightB);*/


	//reading RigheA


	char charEstratto = NULL;
	int charCount = 0;
	FILE* file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/righeA.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			charCount++;
		}
		fclose(file);
	}

	charEstratto = NULL;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/righeA.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			widthA += (int)(pow(10, charCount - 1) * atoi(&charEstratto));
			charCount--;
		}
		fclose(file);
	}


	//reading ColonneA


	charEstratto = NULL;
	charCount = 0;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/colonneA.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			charCount++;
		}
		fclose(file);
	}

	charEstratto = NULL;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/colonneA.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			heightA += (int)(pow(10, charCount - 1) * atoi(&charEstratto));
			charCount--;
		}
		fclose(file);
	}


	//reading ColonneB


	charEstratto = NULL;
	charCount = 0;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/colonneB.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			charCount++;
		}
		fclose(file);
	}

	charEstratto = NULL;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/colonneB.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			heightB += (int)(pow(10, charCount - 1) * atoi(&charEstratto));
			charCount--;
		}
		fclose(file);
	}


	widthB = heightA;
	widthC = widthA;
	heightC = heightB;

	int * A = (int *)malloc(sizeof(int)*widthA*heightA);
	int * B = (int *)malloc(sizeof(int)*widthB*heightB);
	int * C = (int *)malloc(sizeof(int)*widthC*heightC);
	int * Res = (int *)malloc(sizeof(int)*widthC*heightC);
	int * D = (int *)malloc(sizeof(int)*widthC*heightC);

	srand(time(NULL));


	//reading path


	int indexPath = 0;
	char path[1000];
	charEstratto = NULL;
	file = NULL;
	file = fopen("C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/Files/path.txt", "r");
	if (file)
	{
		while ((charEstratto = getc(file)) != EOF)
		{
			if (strchr(&charEstratto, '\\'))
			{
				path[indexPath] = '/';
				indexPath++;
			}
			else
			{
				path[indexPath] = charEstratto;
				indexPath++;
			}
		}
		path[indexPath] = '\0';
		fclose(file);
	}


	char* pathA = "/matAdata.txt";
	char* pathB = "/matBdata.txt";
	char* pathResGPU = "/matResGPU.txt";
	char* pathRes = "/matRes.txt";

	char finalPathA[1000];
	strcpy(finalPathA, path);

	strcat(finalPathA, pathA);

	FILE * fp1 = fopen(finalPathA, "w");
	if (!fp1)
	{
		fprintf(stderr, "failed to open matAdata.\n");
		exit(1);
	}

	for (int i = 0; i < widthA; i++)
	{
		for (int j = 0; j < heightA; j++) {
			*(A + i * heightA + j) = (rand() % 10) + 1;
			fprintf(fp1, "%d ", *(A + i * heightA + j));
		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);

	char finalPathB[1000];
	strcpy(finalPathB, path);

	strcat(finalPathB, pathB);

	fp1 = fopen(finalPathB, "w");
	if (!fp1) {
		fprintf(stderr, "Failed to open matBdata.\n");
		exit(1);
	}

	for (int i = 0; i < widthB; i++)
	{
		for (int j = 0; j < heightB; j++) {
			*((B + i * heightB + j)) = (rand() % 10) + 1;
			fprintf(fp1, "%d ", *(B + i * heightB + j));
		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);

	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobjA = NULL;
	cl_mem memobjB = NULL;
	cl_mem memobjC = NULL;
	cl_mem rowA = NULL;
	cl_mem colB = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;

	//char string[MEM_SIZE];

	FILE *fp;
	char fileName[] = "C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/GraphicProjectSvasi/moltiplicazioneMatrici.cl";
	char *source_str;
	size_t source_size;
	int row = heightA;
	int col = heightB;
	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Create Memory Buffer */
	memobjA = clCreateBuffer(context, CL_MEM_READ_WRITE, widthA * heightA * sizeof(int), NULL, &ret);
	memobjB = clCreateBuffer(context, CL_MEM_READ_WRITE, widthB * heightB * sizeof(int), NULL, &ret);
	memobjC = clCreateBuffer(context, CL_MEM_READ_WRITE, widthC * heightC * sizeof(int), NULL, &ret);
	rowA = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	colB = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);

	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, memobjA, CL_TRUE, 0,
		widthA * heightA * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobjB, CL_TRUE, 0,
		widthB * heightB * sizeof(int), B, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, rowA, CL_TRUE, 0, sizeof(int), &row, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, colB, CL_TRUE, 0, sizeof(int), &col, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "matrixMultiply", &ret);

	/* Set OpenCL Kernel Arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobjA);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobjB);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memobjC);
	ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)&row);
	ret = clSetKernelArg(kernel, 4, sizeof(int), (void *)&col);
	/* Execute OpenCL Kernel */
	size_t globalThreads[2] = { widthA, heightB };
	//size_t localThreads[2] = { widthA, heightB };

	clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, globalThreads, NULL, NULL, 0, NULL);
	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, memobjC, CL_TRUE, 0,
		widthA * heightB * sizeof(int), Res, 0, NULL, NULL);

	char finalPathResGPU[1000];
	strcpy(finalPathResGPU, path);

	strcat(finalPathResGPU, pathResGPU);

	fp1 = fopen(finalPathResGPU, "w");
	if (!fp1) {
		fprintf(stderr, "Failed to open matResGPU.\n");
		exit(1);
	}
	printf("\nResult\n");
	for (int i = 0; i < widthA; i++)
	{
		for (int j = 0; j < heightC; j++)
		{

			fprintf(fp1, "%d ", *(Res + i * heightC + j));

		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobjA);
	ret = clReleaseMemObject(memobjB);
	ret = clReleaseMemObject(memobjC);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);
	system("pause");

	int sum = 0;

	for (int i = 0; i < widthA; i++)
	{
		for (int j = 0; j < heightC; j++)
		{
			sum = 0;
			for (int k = 0; k < heightA; k++)
			{
				sum += A[i*heightA + k] * B[k*col + j];
			}
			D[i*heightC + j] = sum;
		}

	}

	char finalPathRes[1000];
	strcpy(finalPathRes, path);

	strcat(finalPathRes, pathRes);

	fp1 = fopen(finalPathRes, "w");
	if (!fp1) {
		fprintf(stderr, "Failed to open matRes.\n");
		exit(1);
	}

	printf("\nResult\n");
	for (int i = 0; i < widthA; i++)
	{
		for (int j = 0; j < heightC; j++)
		{
			fprintf(fp1, "%d ", *(D + i * heightC + j));

		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);
	system("pause");
	return 0;
}
