function [L, T] = CalLT(lamda, miu, N)

p0 = CalP0(lamda, miu, N);
rou = lamda / miu;
former = 0;
tmp = 1;

for i = 1:1:N
    tmp = tmp * rou / i;
    former = former + i * tmp;
end

largenumber = 1000000;
latter = 0;
e = 10^(-6);
for i = N+1:1:largenumber
    if(tmp * i * rou / N < e)
        break;
    end
    latter = latter + tmp * i * rou / N;
    tmp = tmp * rou / N;
end

L = p0 * (former + latter);

T = L / lamda;

end