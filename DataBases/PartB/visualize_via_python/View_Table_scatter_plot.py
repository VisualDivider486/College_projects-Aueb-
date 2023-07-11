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


df1 = pd.read_sql("SELECT number_of_ratings,average_rating FROM View_Type_Table;",con = conn)
colour_array = np.arange((df1.size)/2)
image = df1.plot.scatter('number_of_ratings','average_rating',c = colour_array,cmap='magma',colorbar=False)
plt.title("View Table Scatter Plot")
plt.xlabel("Number of Ratings")
plt.ylabel("Average rating")
    
plt.savefig('View_Type_Table_scatter_plot.jpg',dpi=150)
    
# Clean up
conn.commit()
cursor.close()
conn.close()