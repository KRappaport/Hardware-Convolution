#ifndef KER_X
#define KER_X 3
#endif

#ifndef	KER_Y
#define KER_Y 3
#endif

#ifndef KER_Z
#define KER_Z 1
#endif

#ifndef KER_X_MAX
#define KER_X_MAX 5
#endif

#ifndef	KER_Y_MAX
#define KER_Y_MAX 5
#endif

#ifndef KER_Z_MAX
#define KER_Z_MAX 11
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

void conv_block_1(
									float input_kernel[KER_X_MAX][KER_Y_MAX][KER_Z_MAX],
									float im_BR[256][KER_Y_MAX][11],
									AXIS_PORT &out_stream,
									int im_x,
									int im_y,
									int rslt_line_nmbr,
									int ker_new
									);

 void result_block(	AXIS_PORT &in_strm_0, AXIS_PORT &in_strm_1, AXIS_PORT &in_strm_2,
    						int im_y,
    						AXIS_PORT &out_strm);

void result_block_4l(	AXIS_PORT &in_strm_0, AXIS_PORT &in_strm_1, AXIS_PORT &in_strm_2,
		 	 	 	 AXIS_PORT &in_strm_3,
    						int im_y,
    						AXIS_PORT &out_strm);


