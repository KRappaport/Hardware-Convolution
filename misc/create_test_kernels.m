function bytes_written = create_test_kernels(img_dim, ker_dim, depth, amount, base_name)
% function: create_test_kernels generates kernels and writes them to files
%
% This is used to create a set of kernels used for testing.  This function
% requires the function matlab_NN_create which can be found in the MATLAB_Code
% directory.  The argument img_dim is to specify what image size the kernel is
% for.  ker_dim is to set the kernel size.  depth is to set the depth of the
% image and kernel.  amount is to set how many should be created.  base_name is
% to specify the path and beginning of the file name where the kernel will be
% stored.

    row_amnt = cast(img_dim, 'uint16');
    col_amnt = cast(img_dim, 'uint16');
    depthc = cast(depth, 'uint16');
    kerdim = cast(ker_dim, 'uint16');

    for k=1:amount
        filename = sprintf('%s%d_%dx%dx%d_%d.tdatb', base_name, kerdim, row_amnt, col_amnt, depthc, cast(k,'uint16'));

        fid = fopen(filename, 'w');
        if fid == -1
            disp('Error: Could not open file!');
            return
        end

	    pad = bitshift(ker_dim, -1);
        net = matlab_NN_create(img_dim, depth, ker_dim, 1, pad, k);
	    test_data = net.Layers(2).Weights;
        test_data = cast(test_data, 'single');

        cnt = fwrite(fid, [kerdim,kerdim,depthc], 'uint16');
        if cnt ~= 3
            disp('Error: Problem writting to file!');
            cnt
            fclose(fid);
            return
        end

        for z=1:depth
            for i=1:ker_dim
                for j=1:ker_dim
                    cnt = fwrite(fid, test_data(i,j,z), 'single');
                    if cnt ~= 1
                        disp('Error: Problem writting to file!');
                        cnt
                        fclose(fid);
                        return
                    end
                end
            end
        end


        fclose(fid);
    end

    bytes_written = cast(((3*16)+(row_amnt*col_amnt*depth*32))*amount/8, 'uint64');

end  % function
