/*
    Creating the tables using the python script from the previous projects and altering some types that didnt suit our data.
*/

CREATE TABLE Ratings(
   userId int,
   movieId int,
   rating numeric,
   timestamp varchar(20)
);

CREATE TABLE Ratings_Small(
   userId int,
   movieId int,
   rating numeric,
   timestamp varchar(20)
);

CREATE TABLE Movies_Metadata(
   adult varchar(10),
   belongs_to_collection varchar(190),
   budget int,
   genres varchar(270),
   homepage varchar(250),
   id int,
   imdb_id varchar(10),
   original_language varchar(10),
   original_title varchar(110),
   overview varchar(1000),
   popularity varchar(10),
   poster_path varchar(40),
   production_companies varchar(1260),
   production_countries varchar(1040),
   release_date date,
   revenue bigint,
   runtime varchar(10),
   spoken_languages varchar(770),
   status varchar(20),
   tagline varchar(300),
   title varchar(110),
   video varchar(10),
   vote_average varchar(10),
   vote_count int
);


CREATE TABLE Credits(
   cast_1 text, /*changed the name because postgres didnt allow us to use the name 'cast'*/
   crew text,
   id int
);

CREATE TABLE Keywords(
   id int,
   keywords text
);

CREATE TABLE Links(
   movieId int,
   imdbId varchar(10),
   tmdbId varchar(10)
);

/*
    Fixing genres column from Movies_Metadata table , so we can use the values as type jsonb.
*/

UPDATE Movies_metadata
SET genres = regexp_replace(genres,'''','"','g');

/*
    Making new table with the genres of every movie.
*/

CREATE TABLE Genres AS 
SELECT id , jsonb_array_elements(genres::jsonb)->>'id' AS genre_id , jsonb_array_elements(genres::jsonb)->>'name' AS genre_name 
FROM Movies_metadata;

/*
    Drop column genres from Movies_Metadata , because we have the data in another table now
*/

ALTER TABLE Movies_Metadata
DROP COLUMN genres;

/*
    Putting all ratings in one table for better access. After that we would have dropped the table Ratings_Small but we let it so the examiner 
    of the project can see it was created correctly etc.
*/

INSERT INTO Ratings
SELECT * FROM Ratings_Small;

/*
    Adding Primary Keys to the tables.
*/

ALTER TABLE Movies_Metadata
ADD PRIMARY KEY(id);

ALTER TABLE Links
ADD PRIMARY KEY(MovieId);

ALTER TABLE Credits
ADD PRIMARY KEY(id);

ALTER TABLE Ratings
ADD PRIMARY KEY(UserId,MovieId,timestamp);

ALTER TABLE Genres
ADD PRIMARY KEY(id,genre_id);

ALTER TABLE Keywords
ADD PRIMARY KEY(id);

/*
    Adding Foreign Keys to the tables.
*/

ALTER TABLE Credits
ADD FOREIGN KEY(id) REFERENCES Movies_Metadata(id);

ALTER TABLE Links
ADD FOREIGN KEY(movieid) REFERENCES Movies_Metadata(id);

ALTER TABLE Keywords
ADD FOREIGN KEY(id) REFERENCES Movies_Metadata(id);

ALTER TABLE Ratings
ADD FOREIGN KEY(movieid) REFERENCES Movies_Metadata(id);

ALTER TABLE Genres
ADD FOREIGN KEY(id) REFERENCES Movies_Metadata(id);








