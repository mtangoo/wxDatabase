wxDatabase is built on the excellent work of Joseph Blough called DatabaseLayer.
This excellent library have been abandoned since 2009. I was finding a suitable database
library to fit my needs and tried different solutions. I found them so missing few things
that I though may be I make mine. Before that I said I would check with databaselayer.

I found that this work should not be let to die. So I picked up the work. Since I felt
JB's way of naming somehow not matching general wx naming, I refactored the code. So much
have been prefixed with "wx" as well as removing the "layer". File name have changed also.

Since I have ambitious plans to see it integrated in wxWidgets (like SQL classes in QT) I will
maintain it. Here are few things I have done which are (unfortunately) not pleasing to many:
- I have dropped support for all other databases leaving MySQL, SQLite3, Postgres and ODBC.
  I don't have enough time to maintain all of those plus you can go ODBC way!
- I have renamed files and classes to match with wx nomenclature
- I have added support for static linking (LIST SUPPORTED Database here)

Finally I would like to thank Joseph for his excellent work. I'm grateful to my company also
for allowing me to do this work of maintanance.

Mungu awabariki!
Stefano
