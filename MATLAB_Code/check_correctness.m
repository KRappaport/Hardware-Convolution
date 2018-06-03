% Check results, of our convolution function against MATLAB's, for correctness

in = rand(11,11,11);
B = ones(11,11);


for k = 1:20
    for k_s = 1:2:33
        ker = rand(k_s,k_s,11);

        result = conv_2d_naive(in,ker);
        resultmc = conv_2d_naive_mult_core(in,ker);

        mat_result = zeros(11,11);
        for z = 1:11
            mat_result = mat_result + convn(in(:,:,z),ker(:,:,z),'same');
        end

        A = ismembertol(result, mat_result, 1E-12);
        C = ismembertol(resultmc, mat_result, 1E-12);
        if (isequal(A,B,C) == 0)
            disp('Results do not match up!')
            return
        end

    end
end
