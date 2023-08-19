Connect mysql with c in CLion IDE
=================================


1) create your mysql database and a table with columns

2) get the code from chatGPT (sample code here)

#include "C:/Program Files/MySQL/MySQL Server 8.0/include/mysql.h"  // add your path for mysql.h
#include <stdio.h>
#include <stdlib.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // change this parameters
    const char *server = "localhost"; 
    const char *user = "root";
    const char *password = "1108";"enter your sqlpassword"
    const char *database = "sms";"enter your data base name"

    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return 1;
    }

    // Add your sql quary
    const char *insert_query = "INSERT INTO student VALUES (105,'mihin',20,'045718')";

    if (mysql_query(conn, insert_query)) {
        fprintf(stderr, "INSERT query failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    printf("Data inserted successfully!\n");

    mysql_close(conn);

    return 0;
}


3) change the CMakeLists.txt { don't paste this. add only include_directories() and target_link_libraries() }

cmake_minimum_required(VERSION 3.24)
project(untitled C)

set(CMAKE_C_STANDARD 17)

# Add the MySQL include directory
include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include")

add_executable(untitled main.c)

# Add the libmysql.lib path, replace your project name
target_link_libraries(project_name "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")


4) go to "C:/Program Files/MySQL/MySQL Server 8.0/lib" file

5) find libmysql.dll file and copy it

6) past libmysql.dll file inside the cmake-build-debug folder(libmysql.dll and project_name.exe should be in the same directory)