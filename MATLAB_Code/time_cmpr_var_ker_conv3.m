% This script is used to compare the run time of naive_3d_conv_2 against that of
% convn (MATLAB's function), for 3D convolution between an image with constant
% size and a kernel with different sizes.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 20;
else
    run_amount = floor(run_amount);
end

%diff kernals

in = rand(99,99,11);

naive_3d_conv_2_time = zeros(1,50);
mat_time = zeros(1,50);

statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for k_s = 1:2:99
        ker = rand(k_s,k_s,11);

        tic;
        result = naive_3d_conv_2(in,ker);
        mycode_time = toc;
        naive_3d_conv_2_time(time_index_ker) = naive_3d_conv_2_time(time_index_ker) + mycode_time;



        tic;
        mat_result = convn(in,ker,'same');
        matlabcode_time = toc;
        mat_time(time_index_ker) = mat_time(time_index_ker) + matlabcode_time;

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

naive_3d_conv_2_time = naive_3d_conv_2_time./run_amount;
mat_time = mat_time./run_amount;

% regular plot take

k_s = 1:2:99;

var_ker_3d(1) = figure;
plot(k_s,naive_3d_conv_2_time);
hold on;
plot(k_s,mat_time);
title({'Run Time for Various Kernel Sizes:', 'naive_3d_conv_2 vs. convn (MATLAB)'}, 'Interpreter', 'none');
xlabel('kernel size');
ylabel('time [s]');
legend('naive\_3d\_conv\_2','convn (MATLAB)','Location','northwest');
hold off;


%double window plot
var_ker_3d(2) = figure;
hold on;
subplot(2,1,1);
plot(k_s,naive_3d_conv_2_time);
title('Run Time for Various Kernel Sizes: naive_3d_conv_2', 'Interpreter', 'none');
xlabel('kernel size');
ylabel('time [s]');

subplot(2,1,2);
plot(k_s,mat_time.*1E3,'red');
title('Run Time for Various Kernel Sizes: convn (MATLAB)');
xlabel('kernel size');
ylabel('time [ms]');
hold off;

fprintf('\n');
