select product_id, product_name, sale_price
from product
where sale_price > (select avg (sale_price) from product);