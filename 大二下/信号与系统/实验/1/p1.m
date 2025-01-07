%% p1
figure(1);
t1 = -1:0.01:10;
f1 = zeros(size(t1));
f1(t1>=0) = 1;
plot(t1, f1);
xlabel('t');
ylabel('f1(t)');
title('f1(t) = ε(t)');
beauty_plot;

%% p2
figure(2);
t2 = 0:0.01:10;
f2 = 4*exp(-0.5*t2).*cos(pi*t2);
plot(t2, f2);
xlabel('t');
ylabel('f2(t)');
title('f2(t) = 4e^{-0.5t}cos(\pit)');
beauty_plot;

%% p3
figure(3);
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
plot(t3, f3); 
xlabel('t');
ylabel('f3(t)');
title('f3(t) = g2(t) + g4(t)');
beauty_plot;

%% p4
figure(4);
k4 = -10:1:20; 
f4 = zeros(size(k4)); 
f4(k4>=-2) = 1; 
f4(k4>=5) = f4(k4>=5) - 1; 
stem(k4, f4); 
xlabel('k');
ylabel('f4(k)');
title('f(k) = ε(k+2) - ε(k-5)');
beauty_plot

%% p5
figure(5);
k5 = -5:1:20;
f5 = 7 * (0.6).^k5 .* cos(0.9*pi*k5); 
stem(k5, f5); 
xlabel('k');
ylabel('f5(k)');
title('f5(k) = 7*(0.6)^k*cos(0.9πk)');
beauty_plot;

%% p6
figure(6);
t6 = -20:0.01:20;
f6 = sin(t6) ./ t6;
plot(t6, f6);
xlabel('t');
ylabel('f6(t)');
title('f6(t) = sin(t)/t');
beauty_plot;



