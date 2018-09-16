function write_result(row_amnt, col_amnt, depth, kerdim, base_name, result, rn)
% function: write_result is used to write convolution results to a file
%
% The arguments row_amnt and col_amnt are used to specify the dimensions of the
% the result.  depth and kerdim is used to only in the file name in order to
% know the depth of the image and kernel from which the result comes from and
% the dimension of the kernel used.  base_name is to specify a path and the
% beginning of the file name.  result is to pass along the actual results.  rn
% is for the file name to which number image and kernel were used for this
% convolution (testing was done on 10 unique images/kernels for each size).

    row_amnt = cast(row_amnt, 'uint16');
    col_amnt = cast(col_amnt, 'uint16');
    depth = cast(depth, 'uint16');

    resname = sprintf('%s_%dx%dx%d_ker%d_%d.tdatb', base_name, row_amnt, col_amnt, depth, kerdim, cast(rn,'uint16'));

    fid = fopen(resname, 'w');
    if fid == -1
        disp('Error: Could not open file!');
        return
    end

    cnt = fwrite(fid, [row_amnt,col_amnt], 'uint16');
    if cnt ~= 2
        disp('Error: Problem writting to file!');
        cnt
        fclose(fid);
        return
    end

    for i=1:row_amnt
        cnt = fwrite(fid, result(i,:), 'single');
        if cnt ~= col_amnt
            disp('Error: Problem writting to file!');
            cnt
            fclose(fid);
            return
        end
    end

    fclose(fid);

end  % function
