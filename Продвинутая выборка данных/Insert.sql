insert into artists (name)
values ('Queen'), ('Дэвид Боуи'), ('Gorillaz'), ('Майлз Дэвис');

insert into genre (name)
values ('Rock'), ('Pop'), ('Jazz'), ('Электроника');

insert into album (title, release_year)
values ('A night at the Opera', '1975'),
		('The Rise and Fall of Ziggy Stardust', '1972'),
		('Demon Days', '2005'),
		('Kind of Blue', '1959'),
		('Folklore','2020');

insert into tracks (title, duration, album_id)
values ('Bohemian Rhapsody','354','1'),
('Love of Me Life','219','1'),
('Starman','254','2'),
('Ziggy Stardust','193','2'),
('Feel Good Inc','222','3'),
('Dare','244','3'),
('Cardigan','244','9');

insert into compil (title, release_year)
values ('Лененды рока','2020'),
('Best of 2000s','2010'),
('Электронная классика','2015'),
('Хиты на все времена','2023');

insert into artistgenre (artist_id, genre_id)
values ('1','1'),
('2','1'),
('2','2'),
('3','1'),
('3','2'),
('3','4'),
('4','3');

insert into artistalbum (album_id, artist_id)
values ('1','1'),
('2','2'),
('3','3'),
('4','4');

insert into compiltrack (compil_id, track_id)
values ('1','1'),
('1','3'),
('1','5'),
('2','5'),
('4','1');


