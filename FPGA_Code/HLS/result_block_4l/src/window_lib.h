#ifndef KER_X
#define KER_X 3
#endif

#ifndef	KER_Y
#define KER_Y 3
#endif

#ifndef KER_Z
#define KER_Z 3
#endif

#ifndef	BR_X
#define BR_X 256
#endif

#ifndef BR_Y
#define BR_Y 7
#endif

#ifndef BR_Z
#define BR_Z 11
#endif

template<int D,int U,int TI,int TD>
  struct ap_axis_fp{
    float            data;
    ap_uint<(D+7)/8> keep;
    ap_uint<(D+7)/8> strb;
    ap_uint<U>       user;
    ap_uint<1>       last;
    ap_uint<TI>      id;
    ap_uint<TD>      dest;
  };

typedef ap_axis_fp <32,1,1,1> AXIS_STRUCT;
typedef hls::stream<AXIS_STRUCT> AXIS_PORT;

void conv_block(
									float kernel_input[KER_X][KER_Y][KER_Z],
									float im_BR[256][7][11],
//									float im_BR_0[256][7][11],float im_BR_1[256][7][11],float im_BR_2[256][7][11],float im_BR_3[256][7][11],
									AXIS_PORT &out_0,AXIS_PORT &out_1,AXIS_PORT &out_2,AXIS_PORT &out_3,
									int im_x,int im_y,
									int rslt_line_nmbr,
//									int rslt_line_nmbr_0,int rslt_line_nmbr_1,int rslt_line_nmbr_2,int rslt_line_nmbr_3
									char busy_0, char busy_1, char busy_2, char busy_3,
									char stall
									);

void result_block_4l(	AXIS_PORT &in_strm_0, AXIS_PORT &in_strm_1, AXIS_PORT &in_strm_2, AXIS_PORT &in_strm_3,
					int im_y,
					AXIS_PORT &out_strm);

void kernel_flip(	float input_kernel[KER_X][KER_Y][KER_Z],
					float flipped_kernel_0[KER_X][KER_Y][KER_Z],
					float flipped_kernel_1[KER_X][KER_Y][KER_Z],
					float flipped_kernel_2[KER_X][KER_Y][KER_Z],
					float flipped_kernel_3[KER_X][KER_Y][KER_Z],
					int ready);
