% This script is used to compare the run time of conv_2d_naive_mult_core against
% that of conv_2d_naive, for 2D convolution between an image with constant
% size and a kernel with different sizes.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 20;
else
    run_amount = floor(run_amount);
end


%diff kernals

in = rand(33,33,11);

if ((exist('ncores')==0) || (isnumeric(ncores)==0) || (ncores <= 0))
    ncores = 2;
else
    ncores = floor(ncores);
end


conv_2d_naive_time = zeros(1,17);
conv_2d_naive_mult_core_time = zeros(1,17);


statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for k_s = 1:2:33
        ker = rand(k_s,k_s,11);

        tic;
        result = conv_2d_naive(in,ker);
        reg_time = toc;
        conv_2d_naive_time(time_index_ker) = conv_2d_naive_time(time_index_ker) + reg_time;


        tic;
        mcore_result = conv_2d_naive_mult_core(in,ker,ncores);
        mcore_time = toc;
        conv_2d_naive_mult_core_time(time_index_ker) = conv_2d_naive_mult_core_time(time_index_ker) + mcore_time;

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

conv_2d_naive_time = conv_2d_naive_time./run_amount;
conv_2d_naive_mult_core_time = conv_2d_naive_mult_core_time./run_amount;

% regular plot take

k_s = 1:2:33;

figure
plot(k_s,conv_2d_naive_time.*1E3);
hold on;
plot(k_s,conv_2d_naive_mult_core_time.*1E3);
title({'Run Time for Various Kernel Sizes:', 'conv_2d_naive vs. conv_2d_naive_mult_core'}, 'Interpreter', 'none');
xlabel('kernel size');
ylabel('time [ms]');
legend('conv\_2d\_naive','conv\_2d\_naive\_mult\_core','Location','northwest');
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
plot(k_s,conv_2d_naive_mult_core_time.*1E3,'red');
title('Run Time for Various Kernel Sizes: conv\_2d\_naive\_mult\_core');
xlabel('kernel size');
ylabel('time [ms]');
hold off;

fprintf('\n');
