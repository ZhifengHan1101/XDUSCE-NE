[y, fs] = audioread('��.wav');
sound(y, fs); % ����Դ�ļ�

nbits = audioinfo("��.wav").BitsPerSample; 

ts = 1/fs;
t = (0:ts:(length(y)-1)*ts);

figure(1);
subplot(2,1,1);
plot(t, y);
title('ԭʼ�źŲ���');
xlabel('ʱ��t');
grid on;
subplot(2,1,2);
num = length(y);
f = (-num/2:num/2-1)/num*fs;
plot(f, abs(fftshift(fft(y))));
title('ԭʼ�ź�Ƶ��');
xlabel('Ƶ�� Hz');
grid on;

% Ƶ�ʼ��� fs/2
fs1 = round(fs/2);
y1 = resample(y, 1, 2);
audiowrite('��_2.wav', y1, fs1);

t1 = (0:1/fs1:(length(y1)-1)/fs1);
figure(2);
subplot(2,1,1);
plot(t1, y1);
title('22kHz�����źŲ���');
xlabel('ʱ��t');
grid on;
subplot(2,1,2);
num1 = length(y1);
f1 = (-num1/2:num1/2-1)/num1*fs1;
plot(f1, abs(fftshift(fft(y1))));
title('22kHz�����ź�Ƶ��');
xlabel('Ƶ�� Hz');
grid on;

% Ƶ���ټ��� fs/4
fs2 = round(fs/4);
y2 = resample(y, 1, 4);
audiowrite('��_4.wav', y2, fs2);

t2 = (0:1/fs2:(length(y2)-1)/fs2);
figure(3);
subplot(2,1,1);
plot(t2, y2);
title('11kHz�����źŲ���');
xlabel('ʱ��t');
grid on;
subplot(2,1,2);
num2 = length(y2);
f2 = (-num2/2:num2/2-1)/num2*fs2;
plot(f2, abs(fftshift(fft(y2))));
title('11kHz�����ź�Ƶ��');
xlabel('Ƶ�� Hz');
grid on;

% Ƶ���ټ��� fs/8
fs3 = round(fs/8);
y3 = resample(y, 1, 8);
audiowrite('��_8.wav', y3, fs3);

t3 = (0:1/fs3:(length(y3)-1)/fs3);
figure(4);
subplot(2,1,1);
plot(t3, y3);
title('5.5kHz�����źŲ���');
xlabel('ʱ��t');
grid on;
subplot(2,1,2);
num3 = length(y3);
f3 = (-num3/2:num3/2-1)/num3*fs3;
plot(f3, abs(fftshift(fft(y3))));
title('5.5kHz�����ź�Ƶ��');
xlabel('Ƶ�� Hz');
grid on;

% Ƶ���ټ��� fs/16
fs4 = round(fs/16);
y4 = resample(y, 1, 16);
audiowrite('��_16.wav', y4, fs4);

t4 = (0:1/fs4:(length(y4)-1)/fs4);
figure(5);
subplot(2,1,1);
plot(t4, y4);
title('2.75kHz�����źŲ���');
xlabel('ʱ��t');
grid on;
subplot(2,1,2);
num4 = length(y4);
f4 = (-num4/2:num4/2-1)/num4*fs4;
plot(f4, abs(fftshift(fft(y4))));
title('2.75kHz�����ź�Ƶ��');
xlabel('Ƶ�� Hz');
grid on;
