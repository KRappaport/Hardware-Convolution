#include<stdio.h>
#include<stdlib.h>

void conv_window( float ker[3][3][3], float im[3][3][3], float conv_rslt[3][3],int im_x,int im_y);


int main(){
	float conv_result[3][3]={0};

	//ker 3x3x3
	float ker[3][3][3] = {{{1,10,19},{4,13,22},{7,16,25}}, //x=0 	// {y=1},{y=2},{y=3} // {z=0,z=1,z=2}
							{{2,11,20},{5,14,23},{8,17,26}}, //x=1
							{{3,12,21},{6,15,24},{9,18,27}}}; //x=2


	//the following is for a 3x3x3 image
	float im[3][3][3] =  {{{1,10,19},{4,13,22},{7,16,25}},   {{2,11,20},{5,14,23},{8,17,26}},   {{3,12,21},{6,15,24},{9,18,27}}};
	conv_window(ker,im,conv_result,3,3);

	return 0;

}

