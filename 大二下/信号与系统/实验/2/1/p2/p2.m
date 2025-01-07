numerator = [2 1];
denominator = [1 4 3];
transfer_function = tf(numerator, denominator);

%% (1)
[impulse_response, t1] = impulse(transfer_function);

figure(1);
plot(t1, impulse_response);
beauty_plot;
title('单位冲击相应'), xlabel('t'), ylabel('y');

%% (2)
[step_response, t2] = step(transfer_function);
figure(2);
plot(t2, step_response);
beauty_plot;
title('单位阶跃响应'), xlabel('t'), ylabel('y');

%% (3)
t3 = 0:0.01:4;
input_signal = 4 * sin(2 * pi * t3);

output_signal = lsim(transfer_function, input_signal, t3);

figure(3);
subplot(2,1,1);
plot(t3, input_signal);
beauty_plot;
title('4sin(2*pi*t)'), xlabel('t'), ylabel('y');

subplot(2,1,2);
plot(t3, output_signal);
beauty_plot;
title('响应函数'), xlabel('t'), ylabel('y');
