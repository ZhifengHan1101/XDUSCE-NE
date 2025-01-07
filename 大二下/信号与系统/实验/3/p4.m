% 定义系统函数 H1(s) 和 H2(s)
num1 = [1 2];  % H1(s) 的分子多项式系数
den1 = [1 1 2 6];  % H1(s) 的分母多项式系数

num2 = [1 0 1];  % H2(s) 的分子多项式系数
den2 = [3 5 4 6];  % H2(s) 的分母多项式系数

% 创建系统模型
sys1 = tf(num1, den1);
sys2 = tf(num2, den2);

% 绘制零点和极点分布
figure;

% 绘制 H1(s) 的零点和极点分布
subplot(2, 1, 1);
pzmap(sys1);
hold on;

% 获取 H1(s) 的零点和极点的坐标
[z1, p1] = pzmap(sys1);

% 绘制 H1(s) 的零点和极点
scatter(real(z1), imag(z1), 'ro', 'LineWidth', 2);
scatter(real(p1), imag(p1), 'bx', 'LineWidth', 2);

% 添加标题和图例
title('H1(s) 零点和极点分布');
legend('H1(s) 零点', 'H1(s) 极点');

% 绘制 H2(s) 的零点和极点分布
subplot(2, 1, 2);
pzmap(sys2);
hold on;

% 获取 H2(s) 的零点和极点的坐标
[z2, p2] = pzmap(sys2);

% 绘制 H2(s) 的零点和极点
scatter(real(z2), imag(z2), 'ro', 'LineWidth', 2);
scatter(real(p2), imag(p2), 'bx', 'LineWidth', 2);

% 添加标题和图例
title('H2(s) 零点和极点分布');
legend('H2(s) 零点', 'H2(s) 极点');

% 判断系统的稳定性
if all(real(z1) < 0)
    disp('H1(s) 是稳定系统');
else
    disp('H1(s) 是不稳定系统');
end

if all(real(z2) < 0)
    disp('H2(s) 是稳定系统');
else
    disp('H2(s) 是不稳定系统');
end

