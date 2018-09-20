function bytes_written = create_test_images(rngh, rngl, row_amnt, col_amnt, depth, amount, base_name)
% function: create_test_images is used to generate images and write them to file
%
% The images are for testing purposes and do not create view-able images.  The
% arguments rngh and rngl set the high end and low end respectively for the
% range used for the numbers that make up the image.  row_amnt, col_amnt, and
% depth set the dimensions the images to be created.  amount specifies how many
% test images to create.  base_name sets the path and the beginning of the
% filename in which the images will be stored.  

    row_amnt = cast(row_amnt, 'uint16');
    col_amnt = cast(col_amnt, 'uint16');
    depth = cast(depth, 'uint16');

    for k=1:amount
        filename = sprintf('%s_%dx%dx%d_%d.tdatb', base_name, row_amnt, col_amnt, depth, cast(k,'uint16'));

        fid = fopen(filename, 'w');
        if fid == -1
            disp('Error: Could not open file!');
            return
        end

        test_data = (rngh - rngl).*rand(row_amnt,col_amnt,depth) + rngl;
        test_data = cast(test_data, 'single');

        cnt = fwrite(fid, [row_amnt,col_amnt,depth], 'uint16');
        if cnt ~= 3
            disp('Error: Problem writting to file!');
            cnt
            fclose(fid);
            return
        end

        for i=1:row_amnt
            for j=1:col_amnt
                cnt = fwrite(fid, squeeze(test_data(i,j,:)), 'single');
                if cnt ~= depth
                    disp('Error: Problem writting to file!');
                    cnt
                    fclose(fid);
                    return
                end
            end
        end

        fclose(fid);
    end

    bytes_written = cast(((3*16)+(row_amnt*col_amnt*depth*32))*amount/8, 'uint64');

end  % function
