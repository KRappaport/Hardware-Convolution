#include <ap_int.h>
#include<stdio.h>
#include <hls_stream.h>
#include "window_lib.h"

#define FLR_KX KER_X/2
#define FLR_KY KER_Y/2
#define FLR_KZ KER_Z/2

#define Y_STOP FLR_KY+1

void conv_block_1(
									float input_kernel[KER_X_MAX][KER_Y_MAX][KER_Z_MAX],
									float im_BR[256][KER_Y_MAX][11],
									AXIS_PORT &out_stream,
									int im_x,
									int im_y,
									int rslt_line_nmbr,
//									int block_nmbr,
//									int last_line_itr,
									int ker_new
									){

#pragma HLS interface s_axilite port=input_kernel bundle=INPUT_KERNEL
#pragma HLS interface axis port=out_stream bundle=STREAM_OUT

#pragma HLS interface s_axilite port=im_y bundle=IM_DATA
#pragma HLS interface s_axilite port=im_x bundle=IM_DATA

	//blocked-pragmas
//#pragma HLS interface s_axilite port=last_line_itr bundle=LAST_LINE
//#pragma HLS interface s_axilite port=block_nmbr bundle=BLOCK_NMBR

#pragma HLS interface bram port=im_BR  bundle=IM_INPUT
#pragma HLS interface s_axilite port=rslt_line_nmbr bundle=RSLT_LINE_NMBR
#pragma HLS interface s_axilite port=return bundle=IM_DATA
#pragma HLS interface s_axilite port=ker_new bundle=KER_NEW


//testing this pragma
//#pragma HLS ARRAY_PARTITION variable=input_kernel complete dim=0


	short x,y,z,i;

	static float flipped_kernel[KER_X][KER_Y][KER_Z];

#pragma HLS ARRAY_PARTITION variable=flipped_kernel complete dim=3


	if(ker_new==1){
		float temp;
		KER_Z_loop:
			for(z=0;z<KER_Z;z++)

		KER_Y_loop:
					for(y=0;y<Y_STOP;y++){

		KER_X_loop:
						if(y==FLR_KX)
							conv_block_1_label4:for(x=0;x<KER_X;x++)
								if(x==(KER_X/2)){
									flipped_kernel[x][y][z]=input_kernel[x][y][z];
//											continue;
								}
								else{
									flipped_kernel[x][y][z]=input_kernel[KER_X-1-x][KER_Y-1-y][z];

//											temp=flipped_kernel[x][y][z];
//											flipped_kernel[x][y][z]=flipped_kernel[KER_X-1-x][KER_Y-1-y][z];
//											flipped_kernel[KER_X-1-x][KER_Y-1-y][z]=temp;

									flipped_kernel[KER_X-1-x][KER_Y-1-y][z] = input_kernel[x][y][z];
								}
						else
							conv_block_1_label5:for(x=0;x<KER_X;x++){
										flipped_kernel[x][y][z]=input_kernel[KER_X-1-x][KER_Y-1-y][z];

//								temp=flipped_kernel[x][y][z];
//								flipped_kernel[x][y][z]=flipped_kernel[KER_X-1-x][KER_Y-1-y][z];
//								flipped_kernel[KER_X-1-x][KER_Y-1-y][z]=temp;

										flipped_kernel[KER_X-1-x][KER_Y-1-y][z] = input_kernel[x][y][z];
							}
					}
	}

	float conv_BR[256][KER_Y][11];

	short y_zero_top,y_zero_bott,x_zero_right,x_zero_left;

	short wndw_x,wndw_y;

	short index_sum;

	short y_line_start=0;
	short y_line_stop =0;
	short x_line_start=0;
	short x_line_stop =0;


	short im_x_short,im_y_short;
	im_x_short = (short)im_x;
	im_y_short = (short)im_y;

	short x_lower_cond;
	AXIS_STRUCT result_out;
    result_out.strb = 0xF;
    result_out.keep = 0xF;
    result_out.last = 0;


    //condition for marking the last line of operation for this convolution block
//    short im_mod;
//	im_mod = im_y_short%4;
//
//    short line_mod;
//    line_mod = rslt_line_nmbr % 4;
//
//    short im_dev_4;
//    im_dev_4 = im_y_short/4;
//
//    short ln_dev_4;
//    ln_dev_4 = rslt_line_nmbr/4;
//
//    short extra_line=0;
//
//    short last_line=0;
//
//    if(im_mod > block_nmbr)
//    	extra_line =1;
//    else
//    	extra_line =0;
//
//    if( ( ((im_dev_4)==(ln_dev_4)) && (extra_line==1) ) || ( ( (ln_dev_4+1)==im_dev_4 ) && extra_line == 0 ) )
//    	last_line =1;
//
//
//    last_line_itr = last_line;

		IM_BR:for(z=0;z<KER_Z;z++)
			IM_BR_0:for(y=0;y<KER_Y;y++)
				IM_BR_1:for(x=0;x<im_x;x++)
					conv_BR[x][y][z] = im_BR[x][y][z];

		y_zero_top = rslt_line_nmbr - (FLR_KY);
		y_zero_bott = rslt_line_nmbr + (FLR_KY)+1;

		if(y_zero_top<0)
			y_line_start = -1*y_zero_top;
		else if (y_zero_bott>im_y_short)
			y_line_stop = y_zero_bott-im_y_short;

		BLOCK_IM_X_inner_loop:
		for(x=0;x<im_x_short;x++){
			result_out.data=0;

			BLOCK_IM_Z_inner_loop:
			for(z=0;z<KER_Z;z++){
				index_sum=0;

				BLOCK_WDNW_X_inner_loop:
				for(wndw_x=0;wndw_x<KER_X;wndw_x++){
					x_lower_cond = x-(FLR_KX)+wndw_x;
					if( x_lower_cond<0 || (x+wndw_x)>im_x_short){
						//skip this iteration if the 'x' is out of bound of the original image. This is zero-ing out
						continue;
					}
					else

						BLOCK_WNDW_Y_inner_loop:
						for(wndw_y=y_line_start;wndw_y<((KER_Y)-y_line_stop);wndw_y++){
							index_sum += conv_BR[x_lower_cond][wndw_y][z]*flipped_kernel[wndw_x][wndw_y][z];
						}
					}
					result_out.data += index_sum;
				}
				if(x==(im_x-1))
					result_out.last = 1;
				out_stream.write(result_out);
		}

}

