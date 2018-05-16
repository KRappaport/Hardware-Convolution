% Check results, of our convolution function against MATLAB's, for correctness

in = rand(11,11);
B = ones(11);

for k = 1:20
    for k_s = 1:2:33
        ker = rand(k_s,k_s);

        result = conv_2d_naive(in,ker);

        mat_result = convn(in,ker,'same');

        A = ismembertol(result, mat_result, 1E-12);
        if (isequal(A,B) == 0)
            disp('Results do not match up!')
            return
        end

    end
end
