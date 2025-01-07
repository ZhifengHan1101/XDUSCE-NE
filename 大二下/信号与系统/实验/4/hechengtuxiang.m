RGB1 = imread("我的照片.jpg");
I1 = im2gray(RGB1);
RGB2 = imread("小孩照片.jpg");
I2 = im2gray(RGB2);
% 使用dct2对灰度图像执行2-D DCT
J1 = dct2(I1);
J2 = dct2(I2);
% 高低频分界线
d = 100;
% 去除低频
J_high = J1;
J_high(abs(J1) > d) = 0;
% 去除高频
J_low = J2;
J_low(abs(J2) <= d) = 0;
% 使用逆DCT函数idct2重建图像
K = idct2(J_low+J_high);
K = rescale(K);
% 显示图像
montage({I1,I2})
title('原始图像');
montage({K})
title('合并图像');
imwrite(K,"合成图像.jpg")