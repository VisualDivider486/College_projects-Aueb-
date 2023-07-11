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


df1 = pd.read_sql("SELECT extract(year from release_date) AS Release_year , genre_name , COUNT(Movies_Metadata.id) AS number_of_movies FROM Movies_Metadata,Genres WHERE Movies_Metadata.id = Genres.id AND extract(year from release_date) IS NOT NULL GROUP BY Release_year,genre_name ORDER BY Release_year DESC;",con = conn)
image = df1.plot.scatter('genre_name','release_year',c = 'number_of_movies',cmap='inferno',rot=0,figsize=(35,20))
plt.title("Number of Movies per Year & Genre")
    
plt.savefig('Query_3_scatter_plot.jpg',dpi=200)
    
# Clean up
conn.commit()
cursor.close()
conn.close()