
in_1 = [1;2;3];
in_2 = [4;5;6];
in_3 = [7;8;9];
in_4 = [10,11,12];
in_5 = [13,14,15];


in = zeros(5,3,3);

in(1,:,1) = in_1;
in(2,:,2) = in_2;
in(3,:,3) = in_3;
in(4,:,4) = in_4;
in(5,:,5) = in_5;

ker = [1,2,3;4,5,6;7,8,9];



        tic
        result = naive_3d_conv_2(in,ker);
        toc
        mycode_time = toc;

        tic
        mat_result = convn(in,ker,'same');
        toc
        matlabcode_time = toc;

