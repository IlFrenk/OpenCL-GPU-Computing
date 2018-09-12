__kernel
void convoluzione(__read_only image2d_t sourceImage, __write_only image2d_t outputImage,
int rows, int cols, __constant float* filter, int filterWidth, sampler_t sampler){

	int ix = get_global_id(0);
	int iy = get_global_id(1);

	int halfWidth = (int)(filterWidth/2);

	float4 sum = {0.0f, 0.0f, 0.0f, 0.0f};

	int filterIdx = 0; //Iteratore per il filtro

	int2 coords; //coordinate per accedere all'immagine

	for(int i = -halfWidth; i <= halfWidth; i++){
		coords.y = ix + i;

		for(int j = -halfWidth; j <= halfWidth; j++){
			coords.x = iy + j;

			//Leggo il pixel dell'immagine sorgente relativo alle coordinate coords
			float4 pixel = read_imagef(sourceImage, sampler, coords);
			sum.x += pixel.x * filter[filterIdx++];
		}
	}

	//Copio il pixel nell'immagine di output se le coordinate sono all'interno del bound
	if(ix < rows && iy < cols){
		coords.x = iy;
		coords.y = ix;
		write_imagef(outputImage, coords, sum);
	}
}