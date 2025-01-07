syms s t

% 定义输入函数 H(s)
H = (s+1)*(s+4) / (s*(s+2)*(s+3));

% 求部分分式分解
[A, B, C] = residue([1, 5, 4], [1, 5, 6, 0]);

% 计算每个部分分式的拉普拉斯反变换
H1 = A(1)/s;
H2 = B(1)/(s+2);
H3 = 0; % 初始化 H3
for i = 1:numel(C)
    H3 = H3 + C(i)/(s+3);
end

% 求和得到 H(t)
Ht = ilaplace(H1, s, t) + ilaplace(H2, s, t) + ilaplace(H3, s, t);

% 绘制 H(s) 的图像
subplot(2, 1, 1);
fplot(H, [-10, 10], 'LineWidth', 2);  % 可根据需要调整范围
xlabel('s');
ylabel('H(s)');
title('H(s)图像');
grid on;

% 绘制 H(t) 的图像
subplot(2, 1, 2);
fplot(Ht, [0, 10], 'LineWidth', 2);  % 可根据需要调整时间范围
xlabel('t');
ylabel('H(t)');
title('H(t)图像');
grid on;
beauty_plot
