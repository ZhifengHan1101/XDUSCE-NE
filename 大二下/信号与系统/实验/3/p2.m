% 清空环境变量
clear;
clc;

% 加载符号工具箱
syms t;

% 定义函数
f1 = 1/2 * exp(-2*t) * heaviside(t);
f2 = 1/2 * exp(-2*(t-1)) * heaviside(t-1);

% 计算傅里叶变换
F1 = fourier(f1);
F2 = fourier(f2);

% 频率范围
f = -10:0.01:10;

% 将符号表达式转换为函数并进行数值计算
F1_abs_func = matlabFunction(abs(F1));
F2_abs_func = matlabFunction(abs(F2));
F1_phase_func = matlabFunction(angle(F1));
F2_phase_func = matlabFunction(angle(F2));

% 计算幅度谱
F1_abs = F1_abs_func(f);
F2_abs = F2_abs_func(f);

% 绘制幅度谱
figure;
subplot(2,1,1);
plot(f, F1_abs);
title('Amplitude Spectrum of f1(t)');
xlabel('Frequency');
ylabel('Amplitude');

subplot(2,1,2);
plot(f, F2_abs);
title('Amplitude Spectrum of f2(t)');
xlabel('Frequency');
ylabel('Amplitude');
beauty_plot

% 计算相位谱
F1_phase = F1_phase_func(f);
F2_phase = F2_phase_func(f);

% 绘制相位谱
figure;
subplot(2,1,1);
plot(f, F1_phase);
title('Phase Spectrum of f1(t)');
xlabel('Frequency');
ylabel('Phase');

subplot(2,1,2);
plot(f, F2_phase);
title('Phase Spectrum of f2(t)');
xlabel('Frequency');
ylabel('Phase');
beauty_plot
