function text = bits_to_text(bits)
    text = '';
    for i = 1:8:length(bits)
        byte = bits(i:i+7);
        text = [text, char(bin2dec(char(byte + '0')))];
    end
end