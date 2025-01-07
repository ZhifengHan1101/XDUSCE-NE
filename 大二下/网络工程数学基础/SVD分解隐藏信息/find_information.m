% 读取修改后的图
reconstructed_figure = imread("modified_figure.jpeg");
reconstructed_figure_gray = rgb2gray(reconstructed_figure);

% 图像SVD分解
[U_modified, S_modified, V_modified] = svd(double(reconstructed_figure_gray));

% 提取隐藏信息
column_to_modify = 500;
alpha = 0.1;
hidden_info = U_modified(:, column_to_modify) / alpha;
