function bits = text_to_bits(text)
    bits = [];
    for i = 1:length(text)
        bits = [bits, dec2bin(text(i), 8) - '0'];
    end
end