function result = conv2d(input,kernel)

    %NOTE: the majority of the cases dealt with will have a z_dim =3
    %for both input and kernel as most of the cases will deal with RGB
    %matrices for image procesing

    %will be done by padding the input


    %get the size of the input matrix and kernel matrix

    %first and foremost, make sure that the 'z' dimensions are equal so to know
    %that only 2D convolution is going to take place


    [input_x,input_y,input_z] = size(input);
    [kernel_x,kernel_y,kernel_z] = size(kernel);   

    if(kernel_z ~= input_z)
        disp('3D mismatch!');
        exit
    end

    %check if dimensions of kernal are even or odd
    kernel_x_is_even = 0;
    kernel_y_is_even = 0;
    kernel_z_is_even = 0;

    if(mod(kernel_x,2)==0)
        kernel_x_is_even = 1;
    end
    if(mod(kernel_y,2)==0)
        kernel_y_is_even = 1;
    end
    if(mod(kernel_z,2)==0)
        kernel_z_is_even = 1;
    end

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
    %getting center of the kernel as well
    %   odd center = floor(ker_dim/2)+1

    %define these variables so asto only do one calculation of floor fxn
    %instead of 2+

    floor_ker_x = floor(kernel_x/2);
    floor_ker_y = floor(kernel_y/2);
    floor_ker_z = floor(kernel_z/2);

    if(kernel_x_is_even == 1)
        padd_x = kernel_x-1;

 %      kernel_x_center = floor_ker_x;

        snip_left_end_x = floor_ker_x-1;
        snip_right_end_x = floor_ker_x;
    else
        padd_x = 2*floor_ker_x;

 %       kernel_x_center = floor_ker_x+1;

        snip_left_end_x = floor_ker_x;
        snip_right_end_x = floor_ker_x;
    end

    if(kernel_x_is_even == 1)
        padd_y = kernel_y-1;

%       kernel_y_center = floor_ker_y;

        snip_left_end_y = floor_ker_x-1;
        snip_right_end_y = floor_ker_x;
    else
        padd_y = 2*floor((kernel_y/2));

%       kernel_y_center = floor_ker_y+1;

        snip_left_end_y = floor_ker_y;
        snip_right_end_y = floor_ker_y;
    end

 %  if(kernel_x_is_even == 1)
 %       kernel_z_center = floor_ker_z;
 %  else
 %      kernel_z_center = floor_ker_z+1;
 %   end

    %make a new matrix called "padd_input" which adds zero padding to the
    %input matrix

    %NOTE: for memeory considerations, might not be best to calc padd_size_xyz
    padd_size_x = input_x+padd_x;
    padd_size_y = input_y+padd_y;

    padd_input = zeros(padd_size_x,padd_size_y,input_z);

    %place the input matrix inside the zeros matrix to make padded matrix
    padd_input((padd_x+1):(padd_x+input_x),...
        (padd_y+1):(padd_y+input_y),...
        (1:input_z)) = input;


    %now the 2 matrices are ready to be convolved

    %First start in bottom left hand corner, calling the verticle direction 'y'
    %and the horizontal direction 'x'. There will be no movements in the 'z'
    %("into the board") direction.

    %Start by moving the kernal upon image from left to right (negative to
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

    input_snip = zeros(kernel_x,kernel_y,kernel_z);

    summation = 0;
    devider = kernel_x*kernel_y*kernel_z;
    
    for z_slice = 1:input_z

        for result_x = 1:input_x

            % then go in the 'y' direction
            for result_y = 1:input_y

                %start the 3D calculations
                %for each iteration, just take a snippit of the input matrix and mult by
                %kernel

                
                z_slice
                result_x
                result_y
                
                
                input_snip = padd_input( ((result_x+floor_ker_x-snip_left_end_x):(result_x+floor_ker_x+snip_right_end_x)),...
                    ((result_y+floor_ker_y-snip_left_end_y):(result_y+floor_ker_y+snip_right_end_y)),...
                    z_slice);

                multiplications = kernel(:,:,z_slice).*input_snip;
                summation2D = sum(multiplications,3);
                summation1D = sum(summation2D,2);
                summation_single = sum(summation1D);
                result(result_x,result_y,z_slice) = summation_single/devider;

            end
        end
        
    end
end


%                 %start the 3D calculations
%                 %for each iteration, just take a snippit of the input matrix and mult by
%                 %kernel
%                 
%                 input_snip = padd_input( ((result_x-snip_left_end_x):(result_x+snip_right_end_x)),...
%                     ((result_y-snip_left_end_y):(result+snip_right_end_x)),...
%                     (1:input_z));
% 
%                 multiplications = kernel.*input_snip;
%                 sum = sum(multiplications,3);
%                 result(result_x,result_y) = sum/devider;

        
        