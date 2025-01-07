N = 1000;
noise_mean = 0;
noise_std = 1;
noise1 = randn(1, N) * noise_std + noise_mean;
noise2 = wgn(1, N, 0);

autocorr1 = xcorr(noise1, 'biased');
autocorr2 = xcorr(noise2, 'biased');

figure(1);
subplot(2,1,1);
plot(noise1);
title('高斯白噪声(randn)')

subplot(2,1,2);
plot(-N+1:N-1, autocorr1);
title('自相关系数(randn)');

figure(2);
subplot(2,1,1);
plot(noise2);
title('高斯白噪声(wgn)');

subplot(2,1,2);
plot(-N+1:N-1, autocorr2);
title('自相关系数(wgn)');