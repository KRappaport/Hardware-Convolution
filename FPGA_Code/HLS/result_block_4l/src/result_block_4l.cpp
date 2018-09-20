//to check if the last line of the entire image has been read and pushed out, we use the variable
// 'line' - the line number we are up to. if it = the image height in the ;y; direction
// then we know we have reached the end of the image and can quit the process

#include <ap_int.h>
#include<stdio.h>
#include <hls_stream.h>
#include "window_lib.h"

void result_block_4l(	AXIS_PORT &in_strm_0, AXIS_PORT &in_strm_1, AXIS_PORT &in_strm_2,
		AXIS_PORT &in_strm_3,
					int im_y,
					AXIS_PORT &out_strm){


#pragma HLS interface axis port=in_strm_0 bundle=IN_STREAM_0
#pragma HLS interface axis port=in_strm_1 bundle=IN_STREAM_1
#pragma HLS interface axis port=in_strm_2 bundle=IN_STREAM_2
#pragma HLS interface axis port=in_strm_3 bundle=IN_STREAM_3

#pragma HLS interface axis port=out_strm bundle=OUT_STREAM
#pragma HLS interface s_axilite port=im_y bundle=IM_Y
#pragma HLS interface s_axilite port=return bundle=RETURN
	short line=0;
	AXIS_STRUCT output;
	short finished = 0;

	while(line != (im_y-1)){
	//stream out the 0-stream
		if( line % 4 == 0 ){
//		if( line % 3 == 0 ){
			while(finished != 1){
				in_strm_0.read(output);
				finished = output.last;


				output.last=0;

				out_strm.write(output);
			}
			finished = 0;
		}
		//stream out the 1-stream
		else if ( line % 4 ==1 ){
//		else if ( line % 3 ==1 ){
			while(finished != 1){
				in_strm_1.read(output);
				finished = output.last;

				output.last=0;

				out_strm.write(output);
			}
			finished =0;
		}
		//stream out the 2-stream
		else if ( line % 4 ==2 ){
//		else{
			while(finished != 1){
				in_strm_2.read(output);
				finished = output.last;

				output.last=0;

				out_strm.write(output);
			}
			finished =0;
		}
		//stream out the 3-stream
		else{
			while(finished != 1){
				in_strm_3.read(output);
				finished = output.last;

				output.last=0;

				out_strm.write(output);
			}
			finished=0;
		}

		line++;
	}

		//final line of the image processed- to generate t-last

		//stream out the 0-stream
		if( line % 4 == 0 ){
//		if( line % 3 == 0 ){
			while(finished != 1){
				in_strm_0.read(output);

				finished = output.last;
				out_strm.write(output);
			}
			finished = 0;
		}
		//stream out the 1-stream
		else if ( line % 4 ==1 ){
//	else if ( line % 3 ==1 ){
			while(finished != 1){
				in_strm_1.read(output);

				finished = output.last;
				out_strm.write(output);
			}
			finished =0;
		}
		//stream out the 2-stream
		else if ( line % 4 ==2 ){
//		else{
			while(finished != 1){
				in_strm_2.read(output);


				finished = output.last;
				out_strm.write(output);
			}
			finished =0;
		}
		//stream out the 3-stream
		else{
			while(finished != 1){
				in_strm_3.read(output);
				finished = output.last;
				out_strm.write(output);
			}
			finished=0;
		}





}
