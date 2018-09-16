function [test_data, Dim] = read_test_image(filename)
% function: Read test image file
% The image file has the structure of 3 unsigned shorts for the WxHxD.  Then
% the actual image values of type float.  The image is stored row by row with
% each depth of a given row and column kept together (similar to an actual
% image file.)
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

    for i=1:Dim(3)
        for j=1:Dim(1)
            [test_data(j,:,i), cnt] = fread(fid, [1, Dim(2)], 'single', 4*(Dim(3)-1));
            if cnt ~= Dim(2)
                disp('Error: Problem reading from file!');
                cnt
                fclose(fid);
                return
            end
        end
        position = position+4;
        status = fseek(fid, position, 'bof');
        if status == -1
            disp('Error: Problem seeking in file!');
            fclose(fid);
            return
        end
    end


    fclose(fid);


end  % function
