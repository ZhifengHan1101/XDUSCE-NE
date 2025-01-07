function hide_text_in_image(image_path, text, output_path)
    img = imread(image_path);
    img = rgb2gray(img);
    img = double(img);

    [u, s, vh] = svd(img);
    bits = text_to_bits(text);

    index = 50;
    u(:, index) = u(:, index) + bits(1:size(u, 1))';

    hidden_img = u * s * vh';
    hidden_img = uint8(hidden_img);
    imwrite(hidden_img, output_path);
end