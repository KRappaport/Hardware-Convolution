function resultant = naive_3d_conv_2(input,kernel)


%flip the input to match matlab conv later on


%get dimensions of input and kernel
[in_x,in_y,in_z] = size(input);
[ker_x,ker_y,ker_z] = size(kernel);

%create a new "padded" input matrix to account for edge values
%we need to padd each edge by the lower half of kernels dimension size

kx = floor(ker_x/2);
ky = floor(ker_y/2);
kz = floor(ker_z/2);

%create a blank matrix of input_dim + padd_dim
%note: we add 2x the padd_dim to account for both directions (pos/neg) of
%the dimension


%NOTE CHANGED HERE THE X AND Y COORDINATES
padded_input = zeros((in_y + 2*ky),(in_x + 2*kx),(in_z +2*kz));

%insert the original input in center of new padded_matrix

padded_input( ((1+kx):(in_x+kx)),((1+ky):(in_y+ky)),((1+kz):(in_z+kz))) = input;

%define a "resultant" matrix to give us our answer
%same size as the original input

resultant = zeros(in_x,in_y,in_z);


%now for the fun....
%A:move the kernel on input via 3 loops
%B:multiply via 3 for loops: one for each dimension

%A: first 3 loops
%going from:
%1. "left" to "right" in the "x" dimension (neg to pos)
%2. then dropping down in the "y" dimension (pos to neg)
%3. moving behind in the "z" direction" (pos to neg)

% the for loop structure will have the reversed order of the steps
%ie "z" = outermost loop, then "y" then "x" = innermost

%it goes only dim_input times in each direction

%B: start with x then y then z direction movement
%x: left to right (neg to pos)
%y: up to down (pos to neg)
%z: front to back (pos to neg)

% this is dependent on the size of the kernel dimensions

%flip the kernel along the "x"  and "y" axis to match
%the method in which matlab convolves

kernel = flip(kernel,3);
kernel = rot90(kernel,2);

for z = 1 : in_z

    for y = 1: in_y

        for x = 1 : in_x

            %end of movement loops
            %start of multiplication loops

            %keep track of #mult in conv set for debug
            tracker = 0;

            for m_z = 1 : ker_z

                for m_y = 1 : ker_y

                    for m_x = 1 : ker_x
                        tracker = tracker +1;

                        %the formula m_x+x-1 etc... follows from the shift
                        %of the kernel along the padded_input (x)
                        %subtracting "1" for the given offset of "1"
                        % in any matlab matrix

                        %debugging stuff
                        kern = kernel(m_y,m_x,m_z);
                        padd  = padded_input(m_y+y-1,m_x+x-1,m_z+z-1);

                        mult =0;
                        mult = kernel(m_y,m_x,m_z)*padded_input(m_y+y-1,m_x+x-1,m_z+z-1);

                        %add back to the resultant
                        resultant(x,y,z) = resultant(x,y,z)+mult;


                    end
                end
            end
            %end of multiplications
            %start of movements


        end
    end

    for tr = 1 : in_z
        resultant(:,:,tr) = transpose(resultant(:,:,tr));
    end
end
