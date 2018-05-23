rng('default');
rng(123);

%diff kernals

in = rand(11,11,11);
time_index_ker = 1;

my_time_ker = zeros(1,17);
mat_time_ker = zeros(1,17);


for k_s = 1:2:33
    ker = rand(k_s,k_s,11);
    
    tic
    result = parfor_naive_3d_conv_2(in,ker);
    toc
    mycode_time = toc;
    my_time_ker(time_index_ker) = mycode_time;



    tic
    my_result = naive_3d_conv_2(in,ker);
    toc
    matlabcode_time = toc;
    mat_time_ker(time_index_ker) = matlabcode_time;
    
    time_index_ker = time_index_ker+1;


end
% regular plot take 

k_s = 1:2:33;

figure
plot(k_s,my_time_ker,'Linewidth',3);
hold on;
plot(k_s,mat_time_ker,'Linewidth',3);
title('My fxn parfor vs. my fxn in time - KERNAL');
xlabel('kernal size');
ylabel('time (s)');
legend('parfor time','w/o parfor time','Location','northwest');
hold on;


%double window plot take 
figure
subplot(2,1,1);
plot(k_s,my_time_ker,'Linewidth',3);
title('my fxn parfor time - KERNAL');
xlabel('kernal size');
ylabel('time (s)');

subplot(2,1,2);
plot(k_s,mat_time_ker,'red','Linewidth',3);
title('my function w/o parfor time - KERNAL');
xlabel('kernal size');
ylabel('time (s)');




%different image sizes 

ker = rand(3,3,3);
time_index_in_1 = 1;
time_index_in_2 = 1;

my_time_in_1 = zeros(1,7);
my_time_in_2 = zeros(1,7);
mat_time_in_2 = zeros(1,7);
mat_time_in_1 = zeros(1,7);
for i_s = 3:9
    for turn = 1:2
        if(turn ==1)
            in = rand(11,10^(i_s-2)+1,10);
        else
            in = rand(11,11,10^(i_s-2));
        end

        in = rand(1,1,3);
        tic
        result = parfor_naive_3d_conv_2(in,ker);
        toc
        mycode_time = toc;

        tic
        my_result = naive_3d_conv_2(in,ker);
        toc
        matlabcode_time = toc;
        
        % TODO: Call for naive-noloops
    
        if(turn ==1)
            my_time_in_1(time_index_in_1) = mycode_time;
            mat_time_in_1(time_index_in_1) = matlabcode_time;
            time_index_in_1 =  time_index_in_1+1;
        else
         	my_time_in_2(time_index_in_2) = mycode_time;
            mat_time_in_2(time_index_in_2) = matlabcode_time;
            time_index_in_2 =  time_index_in_2+1;
        end
        
    end
end

i_s = 3:9;
% regular plot take 1

figure
plot(i_s,my_time_in_1,'Linewidth',3);
hold on;
plot(i_s,mat_time_in_1,'Linewidth',3);
title('My fxn parfor vs. My fxn w/o parfor in time - INPUT - Y DIM LOADED');
xlabel('image size (log_10(size))');
ylabel('time');
legend('my time','matlab time','Location','northwest');
hold on;



%double window plot take 1
figure
subplot(2,1,1);
plot(i_s,my_time_in_1,'Linewidth',3);
title('My fxn parfor time - INPUT - Y DIM LOADED');
xlabel('image size log_10(size)');
ylabel('time');

subplot(2,1,2);
plot(i_s,mat_time_in_1,'red','Linewidth',3);
title('My fxn no w/o parfor time - INPUT - Y DIM LOADED');
xlabel('image size log_10(size)');
ylabel('time');
%}


% regular plot take 2

figure
plot(i_s,my_time_in_2,'Linewidth',3);
hold on;
plot(i_s,mat_time_in_2,'Linewidth',3);
title('My fxn parfor vs. My fxn w/0 parfor in time - INPUT - Z DIM LOADED');
xlabel('image size log_10(size)');
ylabel('time');
legend('my time','my time w/o parfor time','Location','northwest');
hold on;


%double window plot take 2
figure
subplot(2,1,1);
plot(i_s,my_time_in_2,'Linewidth',3);
title('My fxn parfor time - INPUT - Z DIM LOADED');
xlabel('image size log_10(size)');
ylabel('time');

subplot(2,1,2);
plot(i_s,mat_time_in_2,'red','Linewidth',3);
title('My fxn w/o parfor time - INPUT  - Z DIM LOADED');
xlabel('image size');
ylabel('time');
