% This script is used to compare the run time of naive_3d_conv_2 against that of
% convn (MATLAB's function), for 3D convolution between an image with different
% sizes and a kernel with constant size.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 20;
else
    run_amount = floor(run_amount);
end

%different image sizes

ker = rand(3,3,3);

naive_3d_conv_2_time = zeros(1,100);
mat_time = zeros(1,100);

statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_in = 1;
    for i_s = 10:20:1990

        in = rand(i_s,i_s,3);
        tic;
        result = naive_3d_conv_2(in,ker);
        mycode_time = toc;
        naive_3d_conv_2_time(time_index_in) = naive_3d_conv_2_time(time_index_in) + mycode_time;

        tic;
        mat_result = convn(in,ker,'same');
        matlabcode_time = toc;
        mat_time(time_index_in) = mat_time(time_index_in) + matlabcode_time;

        time_index_in =  time_index_in + 1;

    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

naive_3d_conv_2_time = naive_3d_conv_2_time./run_amount;
mat_time = mat_time./run_amount;

i_s = 10:20:1990;
% regular plot take 1

var_img_3d(1) = figure;
plot(i_s,naive_3d_conv_2_time);
hold on;
plot(i_s,mat_time);
title({'Run Time for Various Image Sizes:', ' aive_3d_conv_2 vs. convn (MATLAB)'}, 'Interpreter', 'none');
xlabel('image size');
ylabel('time [s]');
legend('naive\_3d\_conv\_2','convn (MATLAB)','Location','northwest');
hold off;



%double window plot take 1
var_img_3d(2) = figure;
hold on;
subplot(2,1,1);
plot(i_s,naive_3d_conv_2_time);
title('Run Time for Various Image Sizes: naive_3d_conv_2', 'Interpreter', 'none');
xlabel('image size');
ylabel('time [s]');

subplot(2,1,2);
plot(i_s,mat_time.*1E3);
title('Run Time for Various Kernel Sizes: convn (MATLAB)');
xlabel('image size');
ylabel('time [ms]');
hold off;


fprintf('\n');
