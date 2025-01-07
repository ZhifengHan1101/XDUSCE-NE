clear

% 加水印
img = double(imread("合成图像.jpg"));
wm = double(imread("水印.jpg"));
wm = imresize(wm, size(img)); % 调整水印图像大小与原始图像相同
imshow(wm);
title('水印图像');

d = 5; % 水印深度
fimgre = fft2(img) + wm*d;
imgre = uint8(real(ifft2(fimgre)));

figure(1)
% 并排显示加水印前后图像
montage({uint8(img), imgre})
title('原始图像/加入水印后的图像');

figure(2)
% 解水印
wm_recovered = fft2(imgre) - fft2(img);
imshow(mat2gray(real(wm_recovered)/d));
title('解出水印');
