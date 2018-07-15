run_amount = 1;
kernal_max_z = 11;
image_x = 6;
image_y = 6;
%diff kernals

in = floor(10*rand(image_x,image_y,3));
%changed it to 16 length for k start at 3

my_time_ker = zeros(1,ceil(kernal_max_z/2));
mat_time_ker = zeros(1,ceil(kernal_max_z/2));

mat_result = zeros(image_x,image_y,3);
mat_result_final = zeros(image_x,image_y);

for run_number = 1:run_amount
    time_index_ker = 1;
    for k_s = 1:2:kernal_max_z
        ker = floor(10*rand(k_s,k_s,3));

        tic;
        result = conv2d(in,ker);
        mycode_time = toc;
        my_time_ker(time_index_ker) = my_time_ker(time_index_ker) + mycode_time;



        tic;
        for i=1:3
            mat_result(:,:,i) = conv2(in(:,:,i),ker(:,:,i),'same');
        end
            
        mat_result_final = sum(mat_result,3);%./3;
           
        matlabcode_time = toc;
        
        mat_time_ker(time_index_ker) = mat_time_ker(time_index_ker) + matlabcode_time;

        time_index_ker = time_index_ker+1;
        
        checker = abs(result-mat_result);
    end
end

my_time_ker = my_time_ker./run_amount;
mat_time_ker = mat_time_ker./run_amount;

% regular plot take

k_s = 1:2:kernal_max_z;

figure
plot(k_s,my_time_ker);
hold on;
plot(k_s,mat_time_ker);
title('My fxn vs. Matlab fxn in time - KERNAL');
xlabel('kernal size');
ylabel('time [s]');
legend('my time','matlab time','Location','northwest');
hold on;


%double window plot take
figure
subplot(2,1,1);
plot(k_s,my_time_ker);
title('my functions time - KERNAL');
xlabel('kernal size');
ylabel('time [s]');

subplot(2,1,2);
plot(k_s,mat_time_ker.*1E3,'red');
title('matlab functions time - KERNAL');
xlabel('kernal size');
ylabel('time [ms]');

%}

%{
%different image sizes

ker = floor(10*rand(3,3,3);

my_time_in_1 = zeros(1,3);
mat_time_in_1 = zeros(1,3);

for run_number = 1:run_amount
    time_index_in_1 = 1;
    for i_s = 3:5
            mat_result = zeros((10^i_s)+1,(10^i_s)+1,3);
            in = floor(10*rand((10^i_s)+1,(10^i_s)+1,3));
            tic;
            result = conv2d(in,ker);
            mycode_time = toc;
            my_time_in_1(time_index_in_1) = my_time_in_1(time_index_in_1) + mycode_time;

            tic;
            for i=1:3
                mat_result(:,:,i) = conv2(in(:,:,i),ker(:,:,i),'same');
            end
            matlabcode_time = toc;
            mat_time_in_1(time_index_in_1) = mat_time_in_1(time_index_in_1) + matlabcode_time;
            
            time_index_in_1 =  time_index_in_1+1;
    end
end

my_time_in_1 = my_time_in_1./run_amount;
mat_time_in_1 = mat_time_in_1./run_amount;

i_s = 3:9;
% regular plot take 1

figure
plot(i_s,my_time_in_1);
hold on;
plot(i_s,mat_time_in_1);
title('My fxn vs. Matlab fxn in time - INPUT - Y DIM LOADED');
xlabel('image size (log(size))');
ylabel('time');
legend('my time','matlab time','Location','northwest');
hold on;



%double window plot take 1
figure
subplot(2,1,1);
plot(i_s,my_time_in_1);
title('my functions time - INPUT - Y DIM LOADED');
xlabel('image size (log(size))');
ylabel('time');

subplot(2,1,2);
plot(i_s,mat_time_in_1);
title('matlab functions time - INPUT - Y DIM LOADED');
xlabel('image size (log(size))');
ylabel('time');


%}