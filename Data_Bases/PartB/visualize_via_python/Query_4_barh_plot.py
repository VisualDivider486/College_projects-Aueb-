import psycopg2
import pandas as pd
import matplotlib.pyplot as plt

# Update connection string information
host = "project-vaseis-2021.postgres.database.azure.com"
dbname = "postgres"
user = "vasilis@project-vaseis-2021"
password = "project123@"
sslmode = "require"

# Construct connection string
conn_string = "host={0} user={1} dbname={2} password={3} sslmode={4}".format(host, user, dbname, password, sslmode)
conn = psycopg2.connect(conn_string)
cursor = conn.cursor()


df1 = pd.read_sql("SELECT genre_name , AVG(rating) AS average_rating FROM Genres , Ratings WHERE Genres.id = Ratings.MovieId GROUP BY genre_name;",con = conn)
df1.plot.barh('genre_name','average_rating',figsize=(10,7))

plt.savefig('Query_4_barh_plot.jpg',dpi=120)
    
# Clean up
conn.commit()
cursor.close()
conn.close()