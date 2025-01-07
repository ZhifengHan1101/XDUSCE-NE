% 定义系统传递函数
num = [1 0];   % 分子多项式的系数
den = [1 3 2]; % 分母多项式的系数
sys = tf(num, den);  % 创建传递函数模型

% 转换为符号表达式
syms s;
Hs = poly2sym(num, s) / poly2sym(den, s);

% 计算冲击响应
impulse_response = ilaplace(Hs);

% 转换为函数句柄
h_impulse = matlabFunction(impulse_response);

% 计算阶跃响应
step_response = ilaplace(Hs/s);

% 转换为函数句柄
h_step = matlabFunction(step_response);

% 定义输入信号
t = 0:0.01:5;   % 时间范围
f = cos(20*t).*(t>=0);  % 输入信号 f(t) = cos(20t)ε(t)

% 计算零状态响应
zero_state_response = lsim(sys, f, t);

% 绘制冲击响应
subplot(3, 1, 1);
plot(t, h_impulse(t));
xlabel('时间');
ylabel('冲击响应');
title('系统的冲击响应');

% 绘制阶跃响应
subplot(3, 1, 2);
plot(t, h_step(t));
xlabel('时间');
ylabel('阶跃响应');
title('系统的阶跃响应');

% 绘制零状态响应
subplot(3, 1, 3);
plot(t, zero_state_response);
xlabel('时间');
ylabel('零状态响应');
title('零状态响应');
beauty_plot

% 显示图形
sgtitle('系统的响应');
