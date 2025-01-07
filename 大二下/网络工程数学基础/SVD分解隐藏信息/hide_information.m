% 载入图像，并将图像转换为灰度图像
origin_figure = imread("origin_figure.jpeg");
figure_gray = rgb2gray(origin_figure);

% 将文本信息转换为ASCII码
text_to_hide = "Congratulations, you've successfully found it";
text_to_hide = char(text_to_hide);
text_ascii = double(text_to_hide);

% 图像SVD分解
[U, S, V] = svd(double(figure_gray));

% 嵌入信息
row_num_to_modify = numel(text_ascii);
column_to_modify = 500;
U_modified = U;
alpha = 0.01;
U_modified(:,column_to_modify) = 0;
for i = 1:row_num_to_modify
    U_modified(i, column_to_modify) = alpha*double(text_ascii(i));
end

% 重建图像
reconstructed_figure_gray = uint8(U_modified * S * V');
reconstructed_figure = repmat(reconstructed_figure_gray, [1, 1, 3]);
imwrite(reconstructed_figure, "modified_figure.jpeg");
