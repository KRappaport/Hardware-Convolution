
run_amount = 20;

%different image sizes

ker = rand(3,3,3);

my_time_in_1 = zeros(1,7);
my_time_in_2 = zeros(1,7);
mat_time_in_2 = zeros(1,7);
mat_time_in_1 = zeros(1,7);

for run_number = 1:run_amount
    time_index_in_1 = 1;
    time_index_in_2 = 1;
    for i_s = 3:9
        for turn = 1:2
            if(turn ==1)
                in = rand(11,10^(i_s-2)+1,10);
            else
                in = rand(11,11,10^(i_s-2));
            end

            in = rand(1,1,3);
            tic;
            result = naive_3d_conv_2(in,ker);
            mycode_time = toc;

            tic;
            mat_result = convn(in,ker,'same');
            matlabcode_time = toc;

            % TODO: Call for naive-noloops

            if(turn ==1)
                my_time_in_1(time_index_in_1) = my_time_in_1(time_index_in_1) + mycode_time;
                mat_time_in_1(time_index_in_1) = mat_time_in_1(time_index_in_1) + matlabcode_time;
                time_index_in_1 =  time_index_in_1+1;
            else
             	my_time_in_2(time_index_in_2) = my_time_in_2(time_index_in_2) + mycode_time;
                mat_time_in_2(time_index_in_2) = mat_time_in_2(time_index_in_2) + matlabcode_time;
                time_index_in_2 =  time_index_in_2+1;
            end

        end
    end
end

my_time_in_1 = my_time_in_1./run_amount;
my_time_in_2 = my_time_in_2./run_amount;
mat_time_in_1 = mat_time_in_1./run_amount;
mat_time_in_2 = mat_time_in_2./run_amount;

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


%{
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


% regular plot take 2

figure
plot(i_s,my_time_in_2);
hold on;
plot(i_s,mat_time_in_2);
title('My fxn vs. Matlab fxn in time - INPUT - Z DIM LOADED');
xlabel('image size (log(size))');
ylabel('time');
legend('my time','matlab time','Location','northwest');
hold on;

%{
%double window plot take 2
figure
subplot(2,1,1);
plot(i_s,my_time_in_2);
title('my functions time - INPUT - Z DIM LOADED');
xlabel('image size');
ylabel('time');

subplot(2,1,2);
plot(i_s,mat_time_in_2);
title('matlab functions time - INPUT  - Z DIM LOADED');
xlabel('image size');
ylabel('time');
%}
%}
