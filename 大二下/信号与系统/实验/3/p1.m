% 求解 f(t) = e^(-2|t|) 的傅立叶变换
syms t w
f_t = exp(-2*abs(t));
F_w = fourier(f_t, t, w);

% 绘制 f(t) 和 F(w) 的图像
figure;
subplot(2, 1, 1);
fplot(f_t, [-5, 5], 'LineWidth', 2);
title('f(t) = e^{-2|t|}');
xlabel('t');
ylabel('f(t)');

subplot(2, 1, 2);
fplot(F_w, [-5, 5], 'LineWidth', 2);
title('F(w) = Fourier Transform of f(t)');
xlabel('w');
ylabel('F(w)');


% 求解 F(w) = 1/(1+w^2) 的傅立叶反变换
F_w = 1/(1+w^2);
f_t = ifourier(F_w, w, t);

% 绘制 F(w) 和 f(t) 的图像
figure;
subplot(2, 1, 1);
fplot(F_w, [-5, 5], 'LineWidth', 2);
title('F(w) = 1/(1+w^2)');
xlabel('w');
ylabel('F(w)');

subplot(2, 1, 2);
fplot(f_t, [-5, 5], 'LineWidth', 2);
title('f(t) = Inverse Fourier Transform of F(w)');
xlabel('t');
ylabel('f(t)');

