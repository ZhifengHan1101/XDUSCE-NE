function hidden_text = extract_text_from_image(image_path)
    img = imread(image_path);
    img = rgb2gray(img);
    img = double(img);

    [u, s, vh] = svd(img);

    index = 50;
    hidden_bits = round(u(:, index));
    hidden_text = bits_to_text(hidden_bits');
end