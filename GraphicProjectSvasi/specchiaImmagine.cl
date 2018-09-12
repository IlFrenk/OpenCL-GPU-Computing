__kernel
void specchia_immagine(__global float* dest_image, __global float* src_image, int W, int H)
{
  const int ix = get_global_id(0);
  const int iy = get_global_id(1);

  int specchiaX = (int)(W - ix);

  if ((specchiaX >= 0) && (specchiaX < W) && (iy >= 0) && (iy < H)) 
  {
    dest_image[iy*W + ix] = src_image[iy*W + specchiaX];
  }
}