function net = matlab_NN_create(input_W_H, input_C, kernel_dim, n_kernels, padding_size, seed)
    rand('seed', seed);
    weights = rand([ kernel_dim kernel_dim input_C ]);
    stride = 1;
    convla = convolution2dLayer(kernel_dim,n_kernels,'Name', 'conv2d','Stride',[stride stride], 'Padding', [padding_size padding_size padding_size padding_size]);
    convla.Weights = weights;
    convla.Bias = 0;

    layers = [imageInputLayer([input_W_H input_W_H input_C], 'Name', 'input', 'Normalization','none')
              convla
              regressionLayer('Name','output')
              ];
    lgraph = layerGraph(layers);
    % plot(lgraph)
    new_output_dim = (2*padding_size+input_W_H-kernel_dim)/stride+1;
    XTrain = rand([input_W_H input_W_H input_C 1000]);
    YTrain = rand([new_output_dim new_output_dim n_kernels 1000]);


    options = trainingOptions('sgdm',...
        'MaxEpochs',1,...
        'Shuffle','every-epoch',...
        'Verbose',false,...
        'Plots','training-progress');
    net = trainNetwork(XTrain,YTrain,lgraph,options);
