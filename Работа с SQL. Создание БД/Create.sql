create table if not exists genre (

id SERIAL primary key,
name VARCHAR(60) not null

);

create table if not exists artists (

id SERIAL primary key,
name VARCHAR(60) not null

);

create table if not exists album (

id SERIAL primary key,
title TEXT not null,
release_year INTEGER not null

);

create table if not exists tracks (

id SERIAL primary key,
title TEXT not null,
duration INTEGER not null,
album_id INTEGER not null references album(id)

);

create table if not exists compil (

id SERIAL primary key,
title TEXT not null,
release_year INTEGER

);

create table if not exists artistGenre (

artist_id INTEGER references artists(id),
genre_id INTEGER references genre(id),
constraint AG primary key (artist_id, genre_id)

);

create table if not exists artistAlbum (

album_id INTEGER references album(id),
artist_id INTEGER references artists(id),
constraint AA primary key (album_id, artist_id)

);

create table if not exists compilTrack (

compil_id INTEGER references compil(id),
track_id INTEGER references tracks(id),
constraint CT primary key (compil_id, track_id)

);