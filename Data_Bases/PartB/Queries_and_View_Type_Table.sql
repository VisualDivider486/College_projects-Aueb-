SELECT extract(year from release_date) AS Release_year, COUNT(id) AS Number_of_movies
FROM Movies_metadata
WHERE extract(year from release_date) IS NOT NULL
GROUP BY Release_year
ORDER BY Release_year DESC;

SELECT genre_name , COUNT(id) AS number_of_movies
FROM Genres
GROUP BY genre_name;

SELECT extract(year from release_date) AS Release_year , genre_name , COUNT(Movies_Metadata.id) AS number_of_movies
FROM Movies_Metadata,Genres
WHERE Movies_Metadata.id = Genres.id AND extract(year from release_date) IS NOT NULL
GROUP BY Release_year,genre_name
ORDER BY Release_year DESC;

SELECT genre_name , AVG(rating) AS average_rating
FROM Genres , Ratings
WHERE Genres.id = Ratings.MovieId
GROUP BY genre_name;

SELECT userId , COUNT(userId) AS number_of_ratings
FROM Ratings
GROUP BY userId
ORDER BY number_of_ratings DESC;

SELECT userId , AVG(rating) AS average_rating
FROM Ratings
GROUP BY userId
ORDER BY average_rating DESC;

CREATE VIEW View_Type_Table AS SELECT userId , COUNT(userId) AS number_of_ratings , AVG(rating) AS average_rating
FROM Ratings
GROUP BY userId
ORDER BY number_of_ratings DESC;