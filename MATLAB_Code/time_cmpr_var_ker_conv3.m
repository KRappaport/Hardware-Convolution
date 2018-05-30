
if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 20;
else
    run_amount = floor(run_amount);
end

%diff kernals

in = rand(11,11,11);

my_time_ker = zeros(1,17);
mat_time_ker = zeros(1,17);

statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for k_s = 1:2:33
        ker = rand(k_s,k_s,11);

        tic;
        result = naive_3d_conv_2(in,ker);
        mycode_time = toc;
        my_time_ker(time_index_ker) = my_time_ker(time_index_ker) + mycode_time;



        tic;
        mat_result = convn(in,ker,'same');
        matlabcode_time = toc;
        mat_time_ker(time_index_ker) = mat_time_ker(time_index_ker) + matlabcode_time;

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

my_time_ker = my_time_ker./run_amount;
mat_time_ker = mat_time_ker./run_amount;

% regular plot take

k_s = 1:2:33;

figure
plot(k_s,my_time_ker);
hold on;
plot(k_s,mat_time_ker);
title('My fxn vs. Matlab fxn in time - KERNAL');
xlabel('kernal size');
ylabel('time [s]');
legend('my time','matlab time','Location','northwest');
hold on;


%double window plot
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

fprintf('\n');
