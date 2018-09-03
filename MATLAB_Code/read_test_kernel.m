function [test_data, Dim] = read_test_kernel(filename)
% function: Read test kernel from file
% The kernel file has the structure of 3 unsigned shorts for the WxHxD.  Then
% the actual kernel valuse of type float.  The kernel is divided up by the
% depths.  The first set of WxH values are for the zeroth depth and the second
% set are for the first depth and so on and so forth
% 

    fid = fopen(filename, 'r');
    if fid == -1
        disp('Error: Could not open file!');
        return
    end

    [Dim, cnt] = fread(fid, 3, 'uint16');
    if cnt ~= 3
        disp('Error: Problem reading from file!');
        cnt
        fclose(fid);
        return
    end

    test_data = zeros(Dim(1),Dim(2),Dim(3));

    position = ftell(fid);
    if position == -1
        disp('Error: Problem getting position in file!');
        fclose(fid);
        return
    end

    for k=1:Dim(3)
        for i=1:Dim(1)
            for j=1:Dim(2)
                [test_data(i,j,k), cnt] = fread(fid, 1, 'single');
                if cnt ~= 1
                    disp('Error: Problem reading from file!');
                    cnt
                    fclose(fid);
                    return
                end
            end
        end
    end



    fclose(fid);


end  % function
