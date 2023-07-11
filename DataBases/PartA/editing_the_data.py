#Python script to delete duplicate rows from csv files.We use the pandas module.
#Program takes 3 parameters.First is the name of the csv file that we want to clean.Second parameter is the name
#of the new csv file where we export the clean file. And third parameter is the name of the column we use to check for duplictes.

import sys #to access program parameters
import pandas as pd #we use pandas module

file_name = sys.argv[1]
file_name_output = sys.argv[2]
column_id = sys.argv[3]

#for some csv files we had to specify the dtype in the read_csv function so the result would be in the data type would not be changed by pandas

#read the csv file
df = pd.read_csv(file_name, sep=",",encoding='utf-8')

#use column inserted as third program parameter to search for duplicates and drop them.
df.drop_duplicates(column_id, inplace=True)

# Write the results to a new file
df.to_csv(file_name_output,sep=',',encoding='utf-8',index=False)


#-------------------------------------------------------------------------------------------------------------------------------------------------------------


#Python script to delete movies from other tables, that dont exist in the Movies_metadata table.We use the pandas module.
#Apply for every table with the Id column name of table we are checking.

import sys #to access program parameters
import pandas as pd #we use pandas module

input_table = sys.argv[1]
check_on = 'movies_metadata.csv'
output_table = sys.argv[2]

#read the csv files
df1 = pd.read_csv(input_table, sep=",",encoding='utf-8')
df2 = pd.read_csv(check_on, sep=",",encoding='utf-8')

#check if movies from input table exist in movies_metadata table
#we access the column of ids of a table , here for instance df1.MovieId for table Links.
#we changed the code to match the name of the id column of every table we checked.
df1 = df1[df1.movieId.isin(df2.id)]

# Write the results to a new file
df1.to_csv(output_table, index=False)