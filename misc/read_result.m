function [test_data, Dim] = read_result(filename)
% function: read_result takes a filename for a result file and returns the
% results written in the file along with the dimensions of the result.

    fid = fopen(filename, 'r');
    if fid == -1
        disp('Error: Could not open file!');
        return
    end

    [Dim, cnt] = fread(fid, 2, 'uint16');
    if cnt ~= 2
        disp('Error: Problem reading from file!');
        cnt
        fclose(fid);
        return
    end

    test_data = zeros(Dim(1),Dim(2));

    for j=1:Dim(1)
        [test_data(j,:), cnt] = fread(fid, [1, Dim(2)], 'single');
        if cnt ~= Dim(2)
            disp('Error: Problem reading from file!');
            cnt
            fclose(fid);
            return
        end
    end

    fclose(fid);


end  % function
