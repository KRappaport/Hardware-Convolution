% This script is used to compare the run time of conv_2d_naive against that of
% convn (MATLAB's function), for 2D convolution between an image with different
% size and a kernel with constant sizes.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 20;
else
    run_amount = floor(run_amount);
end


%diff image

ker = rand(3,3,3);

conv_2d_naive_time = zeros(1,50);
mat_time = zeros(1,50);


statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for i_s = 10:20:1000
        in = rand(i_s,i_s,3);

        tic;
        result = conv_2d_naive(in,ker);
        mycode_time = toc;
        conv_2d_naive_time(time_index_ker) = conv_2d_naive_time(time_index_ker) + mycode_time;

        mat_result = zeros(i_s, i_s);
        tic;
        for z = 1:3
            mat_result = mat_result + convn(in(:,:,z),ker(:,:,z),'same');
        end
        matlabcode_time = toc;
        mat_time(time_index_ker) = mat_time(time_index_ker) + matlabcode_time;

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

conv_2d_naive_time = conv_2d_naive_time./run_amount;
mat_time = mat_time./run_amount;

% regular plot take

i_s = 10:20:1000;

figure
plot(i_s,conv_2d_naive_time.*1E3);
hold on;
plot(i_s,mat_time.*1E3);
title({'Run Time for Various Image Sizes:', 'conv_2d_naive vs. convn (MATLAB)'}, 'Interpreter', 'none');
xlabel('image size');
ylabel('time [ms]');
legend('conv\_2d\_naive','convn (MATLAB)','Location','northwest');
hold off;


%double window plot
figure
hold on;
subplot(2,1,1);
plot(i_s,conv_2d_naive_time.*1E3);
title('Run Time for Various Image Sizes: conv_2d_naive', 'Interpreter', 'none');
xlabel('image size');
ylabel('time [ms]');

subplot(2,1,2);
plot(i_s,mat_time.*1E3,'red');
title('Run Time for Various Image Sizes: convn (MATLAB)');
xlabel('image size');
ylabel('time [ms]');
hold off;

fprintf('\n');
