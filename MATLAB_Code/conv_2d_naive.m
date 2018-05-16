function resultant = conv_2d_naive(input,kernel)

    % Get dimensions of input and kernel
    [in_row_lgth, in_column_lgth] = size(input);
    [ker_row_lgth, ker_column_lgth] = size(kernel);

    % Find kernel center element
    ker_center_row = floor(ker_row_lgth/2) + 1;
    ker_center_column = floor(ker_column_lgth/2) + 1;

    % Initialize resultant to size of input
    resultant = zeros(in_row_lgth, in_column_lgth);

    % Actual convolution starts here:
    % First 2 for loops determine the element of the result to find
    % and the second 2 match up the proper input and kernel elements.
    % In total 4 nested for loops.

    for result_row = 1:in_row_lgth
        ker_base_row = ker_center_row + result_row;

        % Set start row value such that input and kernel are in bounds
        if ker_base_row > ker_row_lgth
            start_row = ker_base_row - ker_row_lgth;
        else
            start_row = 1;
        end

        % Set end row value such that input and kernel are in bounds
        if (last_row = ker_base_row - 1) > in_row_lgth
            last_row = in_row_lgth;
        end

        for result_column = 1:in_column_lgth
            ker_base_column = ker_center_column + result_column;

            % Set start row value such that input and kernel are in bounds
            if ker_base_column > ker_column_lgth
                start_column = ker_base_column - ker_column_lgth;
            else
                start_column = 1;
            end

            % Set end row value such that input and kernel are in bounds
            if (last_column = ker_base_column - 1) > in_column_lgth
                last_column = in_column_lgth;
            end

            for in_row = start_row:last_row
                ker_row = ker_base_row - in_row;

                for in_column = start_column:last_column

                    resultant(result_row, result_column) = resultant(result_row, result_column) ...
                        + (input(in_row, in_column)*kernel(ker_row, (ker_base_column - in_column)));

                end

            end
        end
    end

end
