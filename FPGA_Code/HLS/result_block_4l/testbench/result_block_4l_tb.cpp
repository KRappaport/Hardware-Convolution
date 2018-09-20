#include <stdio.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "window_lib.h"

 void result_block_4l(	AXIS_PORT &in_strm_0, AXIS_PORT &in_strm_1, AXIS_PORT &in_strm_2,
		 	 	 	 AXIS_PORT &in_strm_3,
    						int im_y,
    						AXIS_PORT &out_strm);



int main(){
	int im_x = 3;
	int im_y = 4;

	float result_block_array[4][3];
	float check[4][3];

	AXIS_STRUCT image_data_0,image_data_1,image_data_2,image_data_3, result_data;
	AXIS_PORT image_0,image_1,image_2,image_3, result;

    image_data_0.last = 0;
    image_data_0.keep = 0xF;
    image_data_0.strb = 0xF;

    image_data_1.last = 0;
    image_data_1.keep = 0xF;
    image_data_1.strb = 0xF;

    image_data_2.last = 0;
    image_data_2.keep = 0xF;
    image_data_2.strb = 0xF;

    image_data_3.last = 0;
    image_data_3.keep = 0xF;
    image_data_3.strb = 0xF;

    int x,y,z,i,j,k;
    float num=1;
    short last=0;

    	//testing full loaded but not cyclical

    //make the test streams

    AXIS_STRUCT image_data,image_rcvd;
    image_data.last = 0;
    image_data.keep = 0xF;
    image_data.strb = 0xF;

    image_rcvd.last = 0;
    image_rcvd.keep = 0xF;
    image_rcvd.strb = 0xF;

//    AXIS_PORT in_strms[4];

//    for(y=0;y<4;y++){
//    	num += 100*y;
//    	for(i=0;i<im_x;i++){
//
//    		if(i== (im_x-1))
//    			image_data.last =1;
//
//    		image_data.data = num;
//    		in_strms[y].write(image_data);
//    		num++;
//
//    	}
//    }
    for(x=0;x<im_x;x++){
		if(x== (im_x-1)){
			image_data_0.last =1;
			image_data_1.last =1;
			image_data_2.last =1;
			image_data_3.last =1;
		}
		image_data_0.data = num;
		check[0][x]=image_data_0.data;
		image_0.write(image_data_0);


		image_data_1.data = num+100;
		check[1][x]=image_data_1.data;
		image_1.write(image_data_1);


		image_data_2.data = num+200;
		check[2][x]=image_data_2.data;
		image_2.write(image_data_2);

		image_data_3.data = num+300;
		check[3][x]=image_data_3.data;
		image_3.write(image_data_3);


		num++;
		}

//    for(x=0;x<im_x;x++){
//		if(x== (im_x-1)){
//			image_data_0.last =1;
//		}
//		else
//			image_data_0.last=0;
//
//		image_data_0.data = num;
//		check[3][x]=image_data_0.data;
//		image_0.write(image_data_0);
//
//		num++;
//    }
    result_block_4l(image_0,image_1,image_2,
    		image_3,
			im_y,
			result );

//	for(i=0;i<4;i++){
    	i=0;

		last =0;
		j=0;
//		if(i==0){
			while(last != 1){
				image_rcvd = result.read();
				result_block_array[i][j]=image_rcvd.data;
				if(image_rcvd.last==1)
					last = 1;
				j++;
			}
//		}
//		else if(i==1){
//			while(last != 1){
//				image_rcvd = result.read();
//				result_block_array[i][j]=image_rcvd.data;
//				if(image_rcvd.last==1)
//					last = 1;
//				j++;
//			}
//		}
//		else if(i==2){
//			while(last != 1){
//				image_rcvd = result.read();
//				result_block_array[i][j]=image_rcvd.data;
//				if(image_rcvd.last==1)
//					last = 1;
//				j++;
//			}
//		}
//		else{
//			while(last != 1){
//				image_rcvd = result.read();
//				result_block_array[i][j]=image_rcvd.data;
//				if(image_rcvd.last==1)
//					last = 1;
//				j++;
//			}
//		}
//	}
	return 0;

}
