#define ker_x 3
#define ker_y 3
#define ker_z 3

#define im_x 3
#define im_y 3
#define im_z 3

#define flr_kx 1
#define flr_ky 1
#define flr_kz 1

#define padd_x  2
#define padd_y  2

#define padd_size_x  5
#define padd_size_y  5



void conv_window( float ker[3][3][3], float im[3][3][3], float conv_rslt[3][3])
{


	int x,y,z,wndw_x,wndw_y,wndw_z;

	float conv[3][3] = {0};
	float swap = 0;

//	int im_x = 3;
//	int im_y = 3;
//	int im_z = 3;
//
//	int ker_x = 3;
//	int ker_y = 3;
//	int ker_z = 3;

//	int flr_kx = (ker_x/2);
//	int flr_ky = (ker_y/2);
//	int flr_kz = (ker_z/2);


//	int padd_x = 2*flr_kx;
//	int padd_y = 2*flr_ky;

//	int padd_size_x = im_x + padd_x;
//	int padd_size_y = im_y + padd_y;

	float padd_im[padd_size_x][padd_size_y][3] = {0};

	//putting the image inside the padding
#pragma unroll
	for(x=0;x<im_x;x++)
#pragma unroll
		for(y=0;y<im_y;y++)
#pragma unroll
			for(z=0;z<im_z;z++)
				padd_im[x+flr_kx][y+flr_ky][z]=im[x][y][z];


				// the following is for a changing kernel size and hence non-constant variables in the for-loops

	//flip the kernel along x/y for all z-slices
		int x_stop = 0;
		int y_stop = flr_ky+1;
#pragma unroll
		for(z=0;z<ker_z;z++)
#pragma unroll
			for(y=0;y<y_stop;y++){
				if(y==flr_kx)
					x_stop = flr_kx;
				else
					x_stop = ker_x;
#pragma unroll
				for(x=0;x<x_stop;x++){
					swap = ker[x][y][z];
					ker[x][y][z]=ker[ker_x-1-x][ker_y-1-y][z];
					ker[ker_x-1-x][ker_y-1-y][z] = swap;
				}
			}



	//convolution part
	//going thru every element that gets its own resultant convolution of add/accumulate
#pragma unroll
	for(x=0;x<im_x;x++)
#pragma unroll
		for(y=0;y<im_y;y++){
#pragma unroll
			for(z=0;z<im_z;z++){
				//make the image window to multiply kernel in each iteration
#pragma unroll
				for(wndw_x = 0  ; wndw_x < ker_x  ; wndw_x++)
#pragma unroll
					for(wndw_y = 0; wndw_y < ker_y ; wndw_y++){

						//perform each multiplication of the convolution by identifying where on the padded image
						//the window for the multiplication of window with kernel will be
						conv[x][y] += padd_im[x+wndw_x][y+wndw_y][z]*ker[wndw_x][wndw_y][z];
					}
			}
		}
#pragma unroll
	for(x=0;x<im_x;x++)
#pragma unroll
		for(y=0;y<im_y;y++)
			conv_rslt[x][y] = conv[x][y];
}
