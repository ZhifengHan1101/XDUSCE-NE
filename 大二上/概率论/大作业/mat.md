```matlab
clc ,clear, close all
P = ones(365, 1);
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
```

```matlab
clc, clear, close all
m=100; %仿真次数
N=30;%学生人数
A = zeros(N);
for j = 1:m
    B = zeros(365);
    for i=1:N
        A(i)=unidrnd(365);%生日的365天
        B(A(i))= B(A(i))+1;
        if (B(A(i))>1)
            plot(j,A(i),'*','color',[1 0 0]);
            hold on;
        else 
            plot(j,A(i),'*','color',[0.6 0.6 0.6]);
            hold on;
        end
    end
end


xlabel("次数");
ylabel("日期");
title("生日悖论散点图");
axis([0,101,0,368]);
```

