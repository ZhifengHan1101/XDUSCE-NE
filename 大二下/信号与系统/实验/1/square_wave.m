function y = square_wave(n)

t = linspace(0, 4*pi, 1001);
y = zeros(size(t));

for k = 1:n
    y = y + sin((2*k-1)*t) ./ (2*k-1);
end

end