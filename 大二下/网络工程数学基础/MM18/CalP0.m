function [p0] = CalP0(lamda, miu, N)

rou = lamda / miu;
p0 = 1;
tmp = 1;

for i = 1:1:N
    tmp = rou * tmp / i;
    p0 = p0 + tmp;
end

p0 = p0 + tmp * lamda / (N * miu - lamda);

p0 = p0 ^ (-1);

end
