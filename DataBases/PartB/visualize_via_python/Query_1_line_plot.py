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


df1 = pd.read_sql("SELECT extract(year from release_date) AS Release_year, COUNT(id) AS Number_of_Movies FROM Movies_metadata WHERE extract(year from release_date) is not null GROUP BY Release_year ORDER BY Release_year DESC;",con = conn)
plt.plot(df1['release_year'],df1['number_of_movies'])
plt.title("Number of Movies per Year")
plt.xlabel("Release year")
plt.ylabel("Number of Movies")
    
plt.savefig('Query_1_line_plot.jpg',dpi=150)
    
# Clean up
conn.commit()
cursor.close()
conn.close()