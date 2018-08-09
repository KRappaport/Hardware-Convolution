//METHOD 3: "HALF LOGIC ZEROS"-NO PADDED INPUT + PARSE THE IMAGE INTO CENTER AND BORDER REGION IN 2 SEPERATE FOR-LOOPS
// 				THE BORDER REGION REQUIRING THE SAME LOGIC INSTANCE AS IN THE 5-LOOP FOR LOOP, AND THE CENTER REGION REQUIRING
//				NO ADDITIONAL LOGIC, JUST PURE MULTIPLICATIONS

//NOTE: Due to the restriction of UNROLL and PIPELINE when accessing memory in arrays, its best to make a sum and then
// 		access the array by a single placement of the sum rather than adding/accessing the array every iteration
//		This is relevant only to METHODs 2&3

//stream the piecemeal to stream output peicemeal as well

#define KER_X 3
#define KER_Y 3
#define KER_Z 3

#define FLR_KX KER_X/2
#define FLR_KY KER_Y/2
#define FLR_KZ KER_Z/2

#define PADD_X  2
#define PADD_Y  2

#define PADD_SIZE_X  5
#define PADD_SIZE_Y  5

#define Y_STOP FLR_KY+1


void conv_window( float ker_input[3][3][3] , float im[3][3][3], float conv_rslt[3][3] , int im_x, int im_y
																			)  	// Commented out convrslt and im for the optimization of the ker flip loop
{
//make the proper interface-ports for ker_input,im,conv_rslt
#pragma HLS interface m_axi port=ker_input bundle=ker_input depth=27
#pragma HLS interface m_axi port=im  bundle=im depth=32*3*3


//Commented out wndw_xy,index_sum im/ker_val for the optimization of the ker flip loop
	int wndw_x,wndw_y,wndw_z,index_sum,index_sum_z,im_val,ker_val;
	int x,y,z;
	float swap = 0;


//for purpose of not having issues of accessing the ports copy the flipped kernel into a new kernel register
	float ker[3][3][3];

// the following is for a changing kernel size and hence non-constant variables in the for-loops
//flip the kernel along x/y for all z-slices

	//int x_stop = 0;
	int x_stop=KER_X;

KER_Z_loop:
	for(z=0;z<KER_Z;z++)


KER_Y_loop:
			for(y=0;y<Y_STOP;y++){

KER_X_loop:
//				if(y==FLR_KX)
//					x_stop = FLR_KX;
//				else
//					x_stop = KER_X;

				if(y==FLR_KX)
					for(x=0;x<x_stop;x++)
						if(x==(x_stop/2))
							ker[x][y][z]=ker_input[x][y][z];
						else{
							ker[x][y][z]=ker_input[KER_X-1-x][KER_Y-1-y][z];
							ker[KER_X-1-x][KER_Y-1-y][z] = ker_input[x][y][z];
						}
				else
					for(x=0;x<x_stop;x++){
						ker[x][y][z]=ker_input[KER_X-1-x][KER_Y-1-y][z];
						ker[KER_X-1-x][KER_Y-1-y][z] = ker_input[x][y][z];
					}
			}

//convolution part
//going thru every element that gets its own resultant convolution of add/accumulate

	int wndw_x_comp,wndw_y_comp;

//1. Compute conv of CENTER of image

IM_X_inner_loop:
	for(x=1;x<(im_x-FLR_KX);x++)		//initial 'x' is dependent on kernel size and not definite 1

IM_Y_inner_loop:
		for(y=1;y<(im_y-FLR_KY);y++){
			index_sum_z=0;

IM_Z_inner_loop:
			for(z=0;z<KER_Z;z++){		//change IM_Z to KER_Z bc will always br the same

				index_sum=0;


WDNW_X_inner_loop:
				for(wndw_x=0;wndw_x<KER_X;wndw_x++)

WNDW_Y_inner_loop:
					for(wndw_y=0;wndw_y<KER_Y;wndw_y++)
						index_sum += im[x+wndw_x-FLR_KX][y+wndw_y-FLR_KY][z]*ker[wndw_x][wndw_y][z];
//				conv_rslt[x][y] += index_sum;//1
				index_sum_z += index_sum;//2
			}
			conv_rslt[x][y] = index_sum_z;//2
		}




//2. Compute conv of BORDER of image

	int side_x,side_y;

IM_Z_border_loop:
	for(z=0;z<KER_Z;z++){

		//working on 'x' border
IM_Y_border_XSIDE_loop:
		for(side_y=0; side_y<im_x; side_y += (im_x-1))

IM_X_border_XSIDE_loop:
			for(x=0;x<im_x;x++)

WNDW_X_border_XSIDE_loop:
				for(wndw_x = -FLR_KX  ; wndw_x < (FLR_KX+1)  ; wndw_x++) //NOTE:cant unroll these loops bc not a static variable, must change "FLR_KX+1" to a static

WNDW_Y_border_XSIDE_loop:
					for(wndw_y = -FLR_KY; wndw_y < (FLR_KY+1) ; wndw_y++){

							wndw_x_comp = x+wndw_x;
							wndw_y_comp = side_y+wndw_y;

							if((wndw_x_comp<0) || (wndw_x_comp > (im_x-1)) || (wndw_y_comp<0) || (wndw_y_comp > (im_y-1)))
								conv_rslt[x][side_y] += 0;
							else
								conv_rslt[x][side_y] += im[wndw_x_comp][wndw_y_comp][z]*ker[wndw_x+FLR_KX][wndw_y+FLR_KY][z];
							}

		//working on 'y' border

IM_X_border_YSIDE_loop:
		for(side_x=0;side_x<im_y;side_x+=(im_y-1))

IM_Y_border_YSIDE_loop:
			for(y=1;y<im_y-1;y++)

WNDW_X_border_YSIDE_loop:
				for(wndw_x = -FLR_KX  ; wndw_x < (FLR_KX+1)  ; wndw_x++) 		//NOTE:cant unroll these loops bc not a static variable, must change "FLR_KX+1" to a static

WNDW_Y_border_YSIDE_loop:
					for(wndw_y = -FLR_KY; wndw_y < (FLR_KY+1) ; wndw_y++){

						wndw_x_comp = side_x+wndw_x;
						wndw_y_comp = y+wndw_y;

						if((wndw_x_comp<0) || (wndw_x_comp > (im_x-1)) || (wndw_y_comp<0) || (wndw_y_comp > (im_y-1)))
							conv_rslt[side_x][y] += 0;
						else
							conv_rslt[side_x][y] += im[wndw_x_comp][wndw_y_comp][z]*ker[wndw_x+FLR_KX][wndw_y+FLR_KY][z];
								}

	}
}
