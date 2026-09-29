select title, duration d
from tracks t 
order by d DESC
limit 1;

select title t, duration
from tracks t 
where duration > (3.5*60);

select title t
from compil c 
where c.release_year between 2018 and 2020;

select name
from artists a 
WHERE name !~ '\s';

select title t
from tracks t 
where title like'%My%';

SELECT 
    g.name AS genre,
    COUNT(ag.artist_id) AS artists_count
FROM genre g
LEFT join artistgenre ag ON ag.genre_id = g.id
GROUP BY g.id, g.name
ORDER BY artists_count DESC;

SELECT COUNT(*) AS tracks_count
FROM tracks t
JOIN album a ON a.id = t.album_id
WHERE a.release_year BETWEEN 2019 AND 2020;

SELECT 
    a.id,
    a.title AS album,
    AVG(t.duration) AS avg_duration
FROM album a
JOIN tracks t ON t.album_id = a.id
GROUP BY a.id, a.title
ORDER BY avg_duration DESC;

SELECT a.id, a.name
FROM artists a
WHERE NOT EXISTS (
    SELECT 1
    FROM artistalbum aa
    JOIN album al ON al.id = aa.album_id
    WHERE aa.artist_id = a.id
      AND al.release_year = 2020
)
ORDER BY a.name;

SELECT DISTINCT c.id, c.title, c.release_year
FROM compil c
JOIN compiltrack ct ON ct.compil_id = c.id
JOIN tracks t              ON t.id = ct.track_id
JOIN artistalbum aa ON aa.album_id = t.album_id
JOIN artists a            ON a.id = aa.artist_id
WHERE a.name = 'Gorillaz'
ORDER BY c.release_year;