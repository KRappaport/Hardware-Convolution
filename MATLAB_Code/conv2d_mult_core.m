function final_result = conv2d_mult_core(input,kernel)

    [input_x,input_y,input_z] = size(input);
    [kernel_x,kernel_y,kernel_z] = size(kernel);

    %get the size of the input matrix and kernel matrix and check that 'z'
    %dimensions match, if dont then exit

    if(kernel_z ~= input_z)
        disp('3D mismatch!');
        exit
    end

%{
    WE ARE ASSUMING ALL KERNELS ARE ODD SIZED so this is commented out

    %check if dimensions of kernel are even or odd

    kernel_x_is_even = 0;
    kernel_y_is_even = 0;

    if(mod(kernel_x,2)==0)
        kernel_x_is_even = 1;
    end
    if(mod(kernel_y,2)==0)
        kernel_y_is_even = 1;
    end
%}

    %padding amount per dimension will be the lower half of the kernel
    %size, bc at the edges the kernel will only overlap with the image for
    %the majority (ceiling) of the dimension

    %NOTE: take account of both EVEN and ODD cases

    %even dim of kernel cases will be dealt with in the following manner:
    %  in every corner and border, we want to have as many multiplications as
    %  possible
    % => split up into "corner cases" and "border cases"
    %(corner is a special instance of border)
    %EVEN: paddamount = 2*(1/2) ker dim -1 = ker din -1
    %ODD: paddamount = 2*floor(ker dim /2)

    %getting the size of padding additions to input matrix
    %   odd center = floor(ker_dim/2)+1

    %define these variables so as to only do one calculation of floor fxn
    %instead of 2+

    floor_ker_x = floor(kernel_x/2);
    floor_ker_y = floor(kernel_y/2);

%%%% THE FOLLOWING IS FOR A CONSIDERATION OF POSSIBILITY OF EVEN KERNEL
%     if(kernel_x_is_even == 1)
%         padd_x = kernel_x-1;
%         snip_left_end_x = floor_ker_x-1;
%         snip_right_end_x = floor_ker_x;
%     else
%         padd_x = 2*floor_ker_x;
%         snip_left_end_x = floor_ker_x;
%         snip_right_end_x = floor_ker_x;
%     end
%
%     if(kernel_x_is_even == 1)
%         padd_y = kernel_y-1;
%         snip_left_end_y = floor_ker_x-1;
%         snip_right_end_y = floor_ker_x;
%     else
%         padd_y = 2*floor((kernel_y/2));
%         snip_left_end_y = floor_ker_y;
%         snip_right_end_y = floor_ker_y;
%     end

%%% THE Following IS FOR CONSIDERATION OF ONLY ODD KERNELS

    padd_x = 2*floor_ker_x;
    snip_left_end_x = floor_ker_x;
    snip_right_end_x = floor_ker_x;

    padd_y = 2*floor_ker_y;
    snip_left_end_y = floor_ker_y;
    snip_right_end_y = floor_ker_y;



    %make a new matrix called "padd_input" which adds zero padding to the
    %input matrix

    padd_size_x = input_x+padd_x;
    padd_size_y = input_y+padd_y;

    padd_input = zeros(padd_size_x,padd_size_y,input_z);

    %place the input matrix inside the zeros matrix to make padded matrix
    padd_input((padd_x/2 +1):(padd_x/2+input_x),...
        (padd_y/2+1):(padd_y/2+input_y),...
        (1:input_z)) = input;

    %First start in bottom left hand corner, calling the vertical direction 'y'
    %and the horizontal direction 'x'. There will be no movements in the 'z'
    %("into the board") direction.

    %Start by moving the kernel upon image from left to right (negative to
    %positive 'x') then down to up (negative to positive 'y')

    %due to the starting of the movements in the "bottom-left"
    %in case of even dimensions, the left and bottom part of the padding will
    %have less zeros than the right and bottom, if necessary

    % The correct starting placement of the kernel upon the padd_input
    %is essential. This is broken up into two cases:
    %evens and odds of the kernels dimensions

    %odds is easy, its the actual center of the 2D plane of the kernel
    %evens we DEFINE the center of the 2D plane to be the lower index
    %ie, if its 6x6 for xy plane,center = (3,3)

    %movement of the kernel on the input on the input starting in the 'x'
    %direction

    %each slice of the 'z' direction gets its own convolution


     %flip kernel in 'x' and 'y' dimension
    for z = 1:kernel_z
        kernel(:,:,z)=flip(kernel(:,:,z),1);
        kernel(:,:,z)=flip(kernel(:,:,z),2);
    end

    input_snip = zeros(kernel_x,kernel_y,kernel_z);
    result = zeros(input_x,input_y,input_z);
    final_result = zeros(input_x,input_y);
    multiplications = 0;
    summation1D =0;
    summation2D =0;

    parfor z_slice = 1:input_z

        for result_x = 1:input_x

            % then go in the 'y' direction
            for result_y = 1:input_y

                %start the 3D calculations
                %each iteration take a snippet of input and mult by kernel

                input_snip = padd_input( ((result_x+floor_ker_x-snip_left_end_x):(result_x+floor_ker_x+snip_right_end_x)),...
                        ((result_y+floor_ker_y-snip_left_end_y):(result_y+floor_ker_y+snip_right_end_y)),...
                        z_slice);

                multiplications = kernel(:,:,z_slice).*input_snip;
                summation2D = sum(multiplications,2);
                summation1D = sum(summation2D);
                result(result_x,result_y,z_slice) = summation1D;

            end
        end
    end

final_result = sum(result,3);%./input_z;
%the final result is a summation along the "z" axis
%z dimension of that x,y index
end
