clc ,clear, close all
P = ones(365, 1); %不同人数对应的概率
x = [1:100];
for i = 1:365
    for j = 1:i
        P(i) = P(i) * (365 - j + 1);
    end
    P(i) = 1 - P(i) / 365^i;
end
plot(x,P(x), "Color",[1, 0, 0], 'LineWidth', 1.5);
xlabel('人数');
ylabel('概率');
title('生日悖论概率图');