% Use to create a neural net from MATLAB.  The aruments of the function are
% input_W_H for the image dimensions (square image only), input_C for the depth,
% kernel_dim for the kernel dimensions (square odd kernel only), n_kernels for
% the number of kernels to create, padding_size in order to keep the result
% dimensions the same as the input image, and seed in order to be able to
% recreate the same kernel

function net = matlab_NN_create(input_W_H, input_C, kernel_dim, n_kernels, padding_size, seed)
    rand('seed', seed);
    weights = rand([ kernel_dim kernel_dim input_C ])*2-1.0;
    stride = 1;
    convla = convolution2dLayer(kernel_dim,n_kernels,'Name', 'conv2d','Stride',[stride stride], 'Padding', [padding_size padding_size padding_size padding_size]);
    convla.Weights = weights;
    convla.Bias = 0;

    layers = [imageInputLayer([input_W_H input_W_H input_C], 'Name', 'input', 'Normalization','none')
              convla
              regressionLayer('Name','output')
              ];
    lgraph = layerGraph(layers);

    new_output_dim = (2*padding_size+input_W_H-kernel_dim)/stride+1;
    num_training_samples = 2;
    XTrain = rand([input_W_H input_W_H input_C num_training_samples]);
    YTrain = rand([new_output_dim new_output_dim n_kernels num_training_samples]);

    options = trainingOptions('sgdm',...
        'MaxEpochs',1,...
        'InitialLearnRate', realmin,...
        'Shuffle','every-epoch',...
        'Verbose',0,...
        'L2Regularization',0.5,...
        'MiniBatchSize', num_training_samples);

    net = trainNetwork(XTrain,YTrain,lgraph,options);
