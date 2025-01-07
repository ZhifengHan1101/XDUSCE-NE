%% （1）
t1 = linspace(-pi, pi, 1000);
N11 = 50;
f_approx1 = zeros(size(t1));
F_approx1 = zeros(size(t1));

for n = 1:N11
    f_approx1 = f_approx1 + sin(n * t1) / n;
    F_approx1 = F_approx1 + f_approx1;
end

figure(1);
plot(t1, f_approx1);
beauty_plot;
title('逼近法（50次）'), xlabel('t'), ylabel('y');


N12 = 100;
f_approx2 = zeros(size(t1));
F_approx2 = zeros(size(t1));

for n = 1:N12
    f_approx2 = f_approx2 + sin(n * t1) / n;
    F_approx2 = F_approx2 + f_approx2;
end

figure(2);
plot(t1, f_approx2);
beauty_plot;
title('逼近法（100次）'), xlabel('t'), ylabel('y');


N13 = 200;
f_approx3 = zeros(size(t1));
F_approx3 = zeros(size(t1));

for n = 1:N13
    f_approx3 = f_approx3 + sin(n * t1) / n;
    F_approx3 = F_approx3 + f_approx3;
end

figure(3);
plot(t1, f_approx3);
beauty_plot;
title('逼近法（200次）'), xlabel('t'), ylabel('y');


N14 = 1000;
f_approx4 = zeros(size(t1));
F_approx4 = zeros(size(t1));

for n = 1:N14
    f_approx4 = f_approx4 + sin(n * t1) / n;
    F_approx4 = F_approx4 + f_approx4;
end

figure(4);
plot(t1, f_approx4);
beauty_plot;
title('逼近法（1000次）'), xlabel('t'), ylabel('y');


N15 = 10000;
f_approx5 = zeros(size(t1));
F_approx5 = zeros(size(t1));

for n = 1:N15
    f_approx5 = f_approx5 + sin(n * t1) / n;
    F_approx5 = F_approx5 + f_approx5;
end

figure(5);
plot(t1, f_approx5);
beauty_plot;
title('逼近法（10000次）'), xlabel('t'), ylabel('y');


%% （2）
F_approx1 = F_approx1 / N11;
F_approx2 = F_approx2 / N12;
F_approx3 = F_approx3 / N13;
F_approx4 = F_approx4 / N14;
F_approx5 = F_approx5 / N15;

figure(6);
plot(t1, F_approx1);
beauty_plot;
title('逼近法（50次）'), xlabel('t'), ylabel('y');

figure(7);
plot(t1, F_approx2);
beauty_plot;
title('逼近法（100次）'), xlabel('t'), ylabel('y');

figure(8);
plot(t1, F_approx3);
beauty_plot;
title('逼近法（200次）'), xlabel('t'), ylabel('y');

figure(9);
plot(t1, F_approx4);
beauty_plot;
title('逼近法（1000次）'), xlabel('t'), ylabel('y');

figure(10);
plot(t1, F_approx5);
beauty_plot;
title('逼近法（10000次）'), xlabel('t'), ylabel('y');
