create database myDB;
create table processed_data (
  id int auto_increment primary key,
  compressed_image longblob,
  compressed_text longblob
);

insert into processed_data (compressed_image, compressed_text)
value (
  LOAD_FILE('/path/to/image.jpg'), 
  'text data'
);

select compressed_image, compressed_text from processed_data;