import psycopg2
import numpy as np
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


df1 = pd.read_sql("SELECT userId , AVG(rating) AS average_rating FROM Ratings GROUP BY userId ORDER BY average_rating DESC;",con = conn)
image = df1.plot.scatter(x = 'userid',y = 'average_rating',colorbar=False)
plt.xticks([])
plt.xlabel("Users")
plt.savefig('Query_6_scatter_plot.jpg',dpi=150)
    
# Clean up
conn.commit()
cursor.close()
conn.close()