% This script is used to compare the run time of conv_2d_mult_core, conv_2d,
% conv2d_mult_core, and conv2d, for 2D convolution between an image and kernel
% with constant dimensions except for the depth.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 10;
else
    run_amount = floor(run_amount);
end


% The base names should be changed to reflect the base name that was used in
% creating the test data.  See the misc directory for more.
basenameimg = './test_data/var_dep/dim3/img';
basenameker = './test_data/var_dep/dim3/ker3';

if ((exist('ncores')==0) || (isnumeric(ncores)==0) || (ncores <= 0))
    ncores = 2;
else
    ncores = floor(ncores);
end


conv_2d_time = zeros(1,11);
conv_2d_mult_core_time = zeros(1,11);
conv2d_time = zeros(1,11);
conv2d_mult_core_time = zeros(1,11);
bytes1 = zeros(ncores,2,11);
bytes2 = zeros(ncores,2,11);


statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for d_s = 1:11
        filename = sprintf('%s_%dx%dx%d_%d.tdatb', basenameimg, 32, 32, d_s, cast(run_number,'uint16'));
        [in,dimimg] = read_test_image(filename);
        filename = sprintf('%s_%dx%dx%d_%d.tdatb', basenameker, 32, 32, d_s, cast(run_number,'uint16'));
        [ker,dimker] = read_test_kernel(filename);

        tic;
        reg_result = conv_2d(in,ker);
        reg_time = toc;
        conv_2d_time(time_index_ker) = conv_2d_time(time_index_ker) + reg_time;

        ticBytes(gcp);
        tic;
        mcore_result = conv_2d_mult_core(in,ker,ncores);
        mcore_time = toc;
        bytes1(:,:,time_index_ker) = bytes1(:,:,time_index_ker) + tocBytes(gcp);
        conv_2d_mult_core_time(time_index_ker) = conv_2d_mult_core_time(time_index_ker) + mcore_time;

        tic;
        reg_result = conv2d(in,ker);
        reg_time = toc;
        conv2d_time(time_index_ker) = conv2d_time(time_index_ker) + reg_time;

        ticBytes(gcp);
        tic;
        mcore_result = conv2d_mult_core(in,ker);
        mcore_time = toc;
        bytes2(:,:,time_index_ker) = bytes2(:,:,time_index_ker) + tocBytes(gcp);
        conv2d_mult_core_time(time_index_ker) = conv2d_mult_core_time(time_index_ker) + mcore_time;

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

conv_2d_time = conv_2d_time./run_amount;
conv_2d_mult_core_time = conv_2d_mult_core_time./run_amount;
bytes1 = sum(bytes1,1)./run_amount;

conv2d_time = conv2d_time./run_amount;
conv2d_mult_core_time = conv2d_mult_core_time./run_amount;
bytes2 = sum(bytes2,1)./run_amount;

% regular plot take

d_s = 1:11;

% conv_2d vs. conv_2d_mult_core
mcore_var_dep(1) = figure;
plot(d_s,conv_2d_time.*1E3);
hold on;
plot(d_s,conv_2d_mult_core_time.*1E3);
title({'Run Time for Various Depths:', 'conv_2d vs. conv_2d_mult_core'}, 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
legend('conv\_2d','conv\_2d\_mult\_core','Location','best');
hold off;

%double window plot
mcore_var_dep(2) = figure;
hold on;
subplot(2,1,1);
plot(d_s,conv_2d_time.*1E3);
title('Run Time for Various Depths: conv_2d', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');

subplot(2,1,2);
plot(d_s,conv_2d_mult_core_time.*1E3,'red');
title('Run Time for Various Depths: conv_2d_mult_core', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
hold off;

mcore_var_dep_bytes(1) = figure;
hold on;
subplot(2,1,1);
plot(d_s, squeeze(bytes1(1,1,:)));
title('Bytes Transferred');
xlabel('depth');
ylabel('Bytes');
subplot(2,1,2);
plot(d_s, squeeze(bytes1(1,2,:)));
title('Bytes Received')
xlabel('depth');
ylabel('Bytes');
hold off;


% conv2d vs. conv2d_mult_core
mcore_var_dep(3) = figure;
plot(d_s,conv2d_time.*1E3);
hold on;
plot(d_s,conv2d_mult_core_time.*1E3);
title({'Run Time for Various Depths:', 'conv2d vs. conv2d_mult_core'}, 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
legend('conv2d','conv2d\_mult\_core','Location','best');
hold off;

%double window plot
mcore_var_dep(4) = figure;
hold on;
subplot(2,1,1);
plot(d_s,conv2d_time.*1E3);
title('Run Time for Various Depths: conv2d', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');

subplot(2,1,2);
plot(d_s,conv2d_mult_core_time.*1E3,'red');
title('Run Time for Various Depths: conv2d_mult_core', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
hold off;

mcore_var_dep_bytes(2) = figure;
hold on;
subplot(2,1,1);
plot(d_s, squeeze(bytes2(1,1,:)));
title('Bytes Transferred');
xlabel('depth');
ylabel('Bytes');
subplot(2,1,2);
plot(d_s, squeeze(bytes2(1,2,:)));
title('Bytes Received')
xlabel('depth');
ylabel('Bytes');
hold off;


% conv_2d_mult_core vs. conv2d_mult_core
mcore_var_dep(5) = figure;
plot(d_s,conv_2d_mult_core_time.*1E3);
hold on;
plot(d_s,conv2d_mult_core_time.*1E3);
title({'Run Time for Various Depths:', 'conv_2d_mult_core vs. conv2d_mult_core'}, 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
legend('conv\_2d\_mult\_core','conv2d\_mult\_core','Location','best');
hold off;

%double window plot
mcore_var_dep(6) = figure;
hold on;
subplot(2,1,1);
plot(d_s,conv_2d_mult_core_time.*1E3);
title('Run Time for Various Depths: conv_2d_mult_core', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');

subplot(2,1,2);
plot(d_s,conv2d_mult_core_time.*1E3,'red');
title('Run Time for Various Depths: conv2d_mult_core', 'Interpreter', 'none');
xlabel('depth');
ylabel('time [ms]');
hold off;

fprintf('\n');
