% This script is used to compare the run time of conv_2d_naive against that of
% convn (MATLAB's function), for 2D convolution between an image with constant
% time and a kernel with different sizes.  

run_amount = 100;

%diff kernals

in = rand(11,11,11);

mat_result = zeros(11,11,11);

conv_2d_naive_time = zeros(1,27);
mat_time = zeros(1,27);

for run_number = 1:run_amount
    time_index_ker = 1;
    for k_s = 1:2:33
        ker = rand(k_s,k_s,11);

        tic;
        result = conv_2d_naive(in,ker);
        mycode_time = toc;
        conv_2d_naive_time(time_index_ker) = conv_2d_naive_time(time_index_ker) + mycode_time;


        tic;
        for z = 1:11
            mat_result(:,:,z) = convn(in(:,:,z),ker(:,:,z),'same');
        end
        matlabcode_time = toc;
        mat_time(time_index_ker) = mat_time(time_index_ker) + matlabcode_time;

        time_index_ker = time_index_ker+1;


    end
end

conv_2d_naive_time = conv_2d_naive_time./run_amount;
mat_time = mat_time./run_amount;

% regular plot take

k_s = 1:2:53;

figure
plot(k_s,conv_2d_naive_time.*1E3);
hold on;
plot(k_s,mat_time.*1E3);
title('Run Time for Various Kernel Sizes: conv_2d_naive vs. convn (MATLAB)', 'Interpreter', 'none');
xlabel('kernel size');
ylabel('time [ms]');
legend('conv\_2d\_naive','convn (MATLAB)','Location','northwest');
hold off;


%double window plot
figure
hold on;
subplot(2,1,1);
plot(k_s,conv_2d_naive_time.*1E3);
title('Run Time for Various Kernel Sizes: conv_2d_naive', 'Interpreter', 'none');
xlabel('kernel size');
ylabel('time [ms]');

subplot(2,1,2);
plot(k_s,mat_time.*1E3,'red');
title('Run Time for Various Kernel Sizes: convn (MATLAB)');
xlabel('kernel size');
ylabel('time [ms]');
hold off;
