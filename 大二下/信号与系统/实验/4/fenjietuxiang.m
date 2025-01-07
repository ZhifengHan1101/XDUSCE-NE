RGB = imread('m_a.png');
I = im2gray(RGB);
% 使用dct2对灰度图像执行2-D DCT
J = dct2(I);
% 使用对数标度显示变换后的图像
% 注意到大部分能量位于左上角
imshow(log(abs(J)),[])
colormap parula
colorbar
% 高低频分界线
d = 340;
% 去除高频
J_low = J;
J_low(abs(J) <= d) = 0;
% 去除低频
J_high = J;
J_high(abs(J) > d) = 0;
% 使用逆DCT函数idct2重建图像
K_low = idct2(J_low);
K_low = rescale(K_low);
K_high = idct2(J_high);
K_high = rescale(K_high);
% 并排显示图像
montage({K_low,K_high})
title('低频图像/高频图像');