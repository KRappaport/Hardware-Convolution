#include<stdio.h>
#include<stdlib.h>
#include <hls_stream.h>
#include <ap_int.h>
#include "window_lib.h"

#define KER_X 5
#define KER_Y 5
#define KER_Z 11

#define BR_X 266
#define BR_Y 11
#define BR_Z 11

#define FLR_KX KER_X/2
#define FLR_KY KER_Y/2
#define FLR_KZ KER_Z/2

#define IM_X 3
#define IM_Y 3

void conv_block_1(
									float flipped_kernel[KER_X][KER_Y][KER_Z],
									float im_BR[256][5][11],
									AXIS_PORT &out_stream,
									int im_x,
									int im_y,
									int rslt_line_nmbr,
//									int block_nmbr,
//									int last_line_itr,
									int ker_new
									);


int main(){

	int i,j,k,x,y,z;
	//ker 3x3x3
	float kernel[5][5][11] = {  {{1,10,19,0,0,0,0,0,0,0,0},{4,13,22,0,0,0,0,0,0,0,0},{7,16,25,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}}, 	//x=0 	// {y=1},{y=2},{y=3} // {z=0,z=1,z=2}
							{	{2,11,20,0,0,0,0,0,0,0,0},{5,14,23,0,0,0,0,0,0,0,0},{8,17,26,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}}, 	//x=1
							{	{3,12,21,0,0,0,0,0,0,0,0},{6,15,24,0,0,0,0,0,0,0,0},{9,18,27,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}},
							{	{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}},
							{	{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0}}}; //x=2



	float flipped_kernel[3][3][3] = {	{{9,18,27},{6,15,24},{3,12,21}},
								{{8,17,26},{5,14,23},{2,11,20}},
								{{7,16,25},{4,13,22},{1,10,19}}	};

	float im[256][5][11]= {0};

	short num=1;

	int imx=3;
	int imy=3;

	short stop =0;
	short index_0=0;

	int rslt_line_nmbr[32]= {0} ;
	int ker_new=1;

	//AXIS_PORT result_out_0,result_out_1,result_out_2,result_out_3;
	AXIS_PORT result_out;
	AXIS_STRUCT pixel;

    short pipe_nmbr;
	/////////////////////////////////////////////
	//5x5x3 image
	imx=5;
	imy=5;
	float conv_tot_553[5][5]={0};

	int busy = 0;
	int last_line=77;
	int block_nmbr = 0;
	for(i=0;i<5;i++){

		index_0=0;
		block_nmbr = i % 4;
		num = (-4)+(i*5);

		rslt_line_nmbr[i] = i;

		for(z=0;z<KER_Z;z++){
			for(y=0;y<KER_Y;y++)
				for(x=0;x<imx;x++){
					im[x][y][z] = num;
					num++;
				}
			num+= (imy-KER_Y)*imx;
		}

		conv_block_1(
						kernel,
						im,
						result_out,
						imx,
						imy,
						rslt_line_nmbr[i],
//						block_nmbr,
//						last_line,
						ker_new);

		while(stop == 0){
			pixel = result_out.read();
			conv_tot_553[i][index_0]=pixel.data;
			stop = pixel.last;
			index_0++;
		}
		stop=0;
		ker_new=0;
	}
	return 0;
}

