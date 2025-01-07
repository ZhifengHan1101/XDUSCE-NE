%% f3
t3 = -10:0.01:10; 
tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1(t3 <= 1) = 1;
tmp2(t3 >= -1) = 1;
g2 = tmp1 - tmp2;

tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1(t3 <= 2) = 1;
tmp2(t3 >= -2) = 1;
g4 = tmp1 - tmp2;

f3 = g2 + g4; 

%% (1)
figure(1);
tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1(2 * t3 <= 1) = 1;
tmp2(2 * t3 >= -1) = 1;
g21 = tmp1 - tmp2;

tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1(2 * t3 <= 2) = 1;
tmp2(2 * t3 >= -2) = 1;
g41 = tmp1 - tmp2;

f31 = g21 + g41; 

plot(t3, f31);
xlabel('t3');
ylabel('f3(2t)');
title('f3(2t) = g2(2t) + g4(2t)');
beauty_plot;

%% (2)
figure(2);
tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1((4 - 2 * t3) <= 1) = 1;
tmp2((4 - 2 * t3) >= -1) = 1;
g22 = tmp1 - tmp2;

tmp1 = zeros(size(t3));
tmp2 = zeros(size(t3));
tmp1((4 - 2 * t3) <= 2) = 1;
tmp2((4 - 2 * t3) >= -2) = 1;
g42 = tmp1 - tmp2;

f32 = g22 + g42; 

plot(t3, f32);
xlabel('t3');
ylabel('f3(2t)');
title('f3(2t) = g2(2t) + g4(2t)');
beauty_plot;

%% (3)
figure(3);

f33 = diff(f32);

plot([-10:0.01:9.99] ,f33);
xlabel('t3');
ylabel('f3(4-2t)');
title('f3(4-2t)');
beauty_plot;
