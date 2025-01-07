clc, clear, close all
m=100; %仿真次数
N=70;%学生人数
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
title("生日悖论散点图（70人）");
axis([0,101,0,368]);