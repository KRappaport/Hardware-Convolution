% This script is used to compare the run times of conv_2d, conv2d, and
% convn (MATLAB's function), for 2D convolution between an image with different
% size and a kernel with constant sizes.


if ((exist('run_amount')==0) || (isnumeric(run_amount)==0) || (run_amount <= 0))
    run_amount = 10;
else
    run_amount = floor(run_amount);
end


depth = 3;
pad = bitshift(depth, -1);

conv_2d_time = zeros(1,252);
conv2d_time = zeros(1,252);
mat_time = zeros(1,252);

basenameimg = './test_data/var_img/depth3/img';
basenameker = './test_data/var_img/depth3/ker3';

statuss = 'Status:    ';
updstat = ['0%%  [' repmat(' ', 1, run_amount) ']'];
delstr = repmat('\b', 1, length(updstat) - 1);
fprintf([statuss, updstat]);

for run_number = 1:run_amount
    time_index_ker = 1;
    for i_s = 5:256
        filename = sprintf('%s_%dx%dx%d_%d.tdatb', basenameimg, i_s, i_s, depth, cast(run_number,'uint16'));
        [in,dimimg] = read_test_image(filename);
        filename = sprintf('%s_%dx%dx%d_%d.tdatb', basenameker, i_s, i_s, depth, cast(run_number,'uint16'));
        [ker,dimker] = read_test_kernel(filename);

        tic;
        result = conv_2d(in,ker);
        mycode_time = toc;
        conv_2d_time(time_index_ker) = conv_2d_time(time_index_ker) + mycode_time;

        tic;
        result = conv2d(in,ker);
        mycode_time = toc;
        conv2d_time(time_index_ker) = conv2d_time(time_index_ker) + mycode_time;

        net = matlab_NN_create(i_s, depth, 3, 1, pad, run_number);
        tic;
        ret = net.predict(in) - net.Layers(2).Bias;
        matlabcode_time = toc;
        mat_time(time_index_ker) = mat_time(time_index_ker) + matlabcode_time;

        if (isequal(ker, net.Layers(2).Weights) == 0)
            disp('Kernels do not match up!')
            return
        end

        time_index_ker = time_index_ker+1;


    end

    pdn = run_number*100/run_amount;
    updstat = [num2str(pdn, '%4.2f') '%%  [' repmat('*', 1, run_number) repmat(' ', 1, run_amount-run_number) ']'];
    fprintf([delstr, updstat]);
    delstr = repmat('\b', 1, length(updstat) - 1);

end

conv_2d_time = conv_2d_time./run_amount;
conv2d_time = conv2d_time./run_amount;
mat_time = mat_time./run_amount;

% regular plot take

i_s = 5:256;

var_img_2d(1) = figure;
plot(i_s,conv_2d_time.*1E3);
hold on;
plot(i_s,conv2d_time.*1E3);
plot(i_s,mat_time.*1E3);
title({'Run Time for Various Image Sizes:', 'conv_2d vs. conv2d vs. MATLAB NN'}, 'Interpreter', 'none');
xlabel('image size');
ylabel('time [ms]');
legend('conv\_2d','conv2d','MATLAB NN','Location','northwest');
hold off;


%double window plot
var_img_2d(2) = figure;
hold on;
subplot(3,1,1);
plot(i_s,conv_2d_time.*1E3);
title('Run Time for Various Image Sizes: conv_2d', 'Interpreter', 'none');
xlabel('image size');
ylabel('time [ms]');

subplot(3,1,2);
plot(i_s,conv2d_time.*1E3);
title('Run Time for Various Image Sizes: conv2d', 'Interpreter', 'none');
xlabel('image size');
ylabel('time [ms]');

subplot(3,1,3);
plot(i_s,mat_time.*1E3,'red');
title('Run Time for Various Image Sizes: MATLAB NN');
xlabel('image size');
ylabel('time [ms]');
hold off;

fprintf('\n');
