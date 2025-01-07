% 主程序
image_path = 'origin_figure.jpeg';
output_path = 'modified_figure.jpeg';
text = 'you have found it';

% 隐藏文本信息到图像
hide_text_in_image(image_path, text, output_path);

% 从隐藏文本的图像中解码文本信息
hidden_text = extract_text_from_image(output_path);
disp(hidden_text);
