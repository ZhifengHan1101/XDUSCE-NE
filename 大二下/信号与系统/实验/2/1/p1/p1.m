%% (1)
t1 = 0:0.01:2;

f1 = (t1 >= 0) - (t1 >= 1);
f2 = 2 * t1 .*((t1 >= 0) - (t1 >= 1));

convolution1 = conv(f1, f2);

t_conv1 = 0:0.01:4;

figure(1);
subplot(2,2,1);
plot(t1, f1);
beauty_plot;
title('f1(t)'), xlabel('t'), ylabel('y');

subplot(2,2,2);
plot(t1,f2);
beauty_plot;
title('f2(t)'), xlabel('t'), ylabel('y');

subplot(2,2,[3,4]);
plot(t_conv1, convolution1);
beauty_plot;
title('Convolution of f1 and f2'), xlabel('t'), ylabel('y');

%% (2)
t2 = -3:0.01:5;

f3 = cos(30 * t2) .* ((t2 >= -2.5) - (t2 >= 2.5));
f4 = (t2 >= 0) - (t2 >= 4);

convolution2 = conv(f3, f4);

t_cov2 = -6:0.01:10;

figure(2);
subplot(2,2,1);
plot(t2, f3);
beauty_plot;
title('f1(t)'), xlabel('t'), ylabel('y');

subplot(2,2,2);
plot(t2, f4);
beauty_plot;
title('f2(t)'), xlabel('t'), ylabel('y');

subplot(2,2,[3,4]);
plot(t_cov2, convolution2);
beauty_plot;
title('Convolution of f1 and f2'), xlabel('t'), ylabel('y');
