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


df1 = pd.read_sql("SELECT genre_name , COUNT(id) AS number_of_movies FROM Genres GROUP BY genre_name;",con = conn)
df1.plot.bar('genre_name','number_of_movies',figsize=(17,15))
plt.title("Number of Movies per Genre")
plt.xlabel("")

plt.savefig('Query_2_bar_plot.jpg',dpi=200)
    
# Clean up
conn.commit()
cursor.close()
conn.close()