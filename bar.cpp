#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <fstream>
#include <iostream>
#include <algorithm>

// Function that prints the progress bar
void printProgressBar(int curr, int total)
{
    const int barWidth = 70;
    float progress = float(curr) / total;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}



int main()
{

    // To calculate total lines
    std::ifstream countFile("dir.txt");
    int totalLines = std::count(std::istreambuf_iterator<char>(countFile), 
                     std::istreambuf_iterator<char>(), '\n');
    countFile.close();

    sqlite3 *db;
    char *error = 0;

    // Open the DB
    int dbOpen = sqlite3_open("test.db", &db);

    if (dbOpen)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }

    // Create a table
    std::string sql = "CREATE TABLE IF NOT EXISTS Lines(Line TEXT);";
    sqlite3_exec(db, sql.c_str(), NULL, NULL, &error);

    // Open the text file
    std::ifstream inputFile("dir.txt");
    std::string line;

    if (inputFile.is_open())
    {
        // counter for number of lines processed
        int count = 0;

        while (getline(inputFile, line))
        {
            // begin a new transaction every 10000 lines
            if (count % 10000 == 0)
            {
                sql = "BEGIN;";
                sqlite3_exec(db, sql.c_str(), NULL, NULL, &error);
            }

            // Replace any single quotes in the line with double single quotes to avoid SQL error
            for (size_t pos = 0; (pos = line.find("'", pos)) != std::string::npos; pos += 2) {
                line.insert(pos, 1, '\'');
            }

            // Prepare the SQL statement
            sql = "INSERT INTO Lines(Line) VALUES('" + line + "');";

            // Execute the SQL statement
            sqlite3_exec(db, sql.c_str(), NULL, NULL, &error);

            // commit the transaction every 10000 lines
            if (count % 10000 == 9999)
            {
                sql = "COMMIT;";
                sqlite3_exec(db, sql.c_str(), NULL, NULL, &error);
            }
            printProgressBar(count, totalLines);

            count++;
        }

        // commit any remaining lines
        sql = "COMMIT;";
        sqlite3_exec(db, sql.c_str(), NULL, NULL, &error);

        inputFile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }

    // Close DB
    sqlite3_close(db);
    printProgressBar(totalLines, totalLines);
    return 0;
}