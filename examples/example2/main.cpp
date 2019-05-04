/**
 * @file  main.cpp
 * @brief A few short examples in a row.
 *
 *  Demonstrates how-to use the SQLite++ wrapper
 *
 * Copyright (c) 2012-2019 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>


#ifdef SQLITECPP_ENABLE_ASSERT_HANDLER
namespace SQLite
{
/// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    // Print a message to the standard error output stream, and abort the program.
    std::cerr << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
    std::abort();
}
}
#endif

/// Get example path
static inline std::string getExamplePath()
{
    std::string filePath(__FILE__);
    return filePath.substr( 0, filePath.length() - std::string("main.cpp").length());
}

/// Example Database
static const std::string filename_example_db3 = getExamplePath() + "/example.db3";


/// Object Oriented Basic example
int main ()
{
    // Using SQLITE_VERSION would require #include <sqlite3.h> which we want to avoid: use SQLite::VERSION if possible.
//  std::cout << "SQlite3 version " << SQLITE_VERSION << std::endl;
    std::cout << "SQlite3 version " << SQLite::VERSION << " (" << SQLite::getLibVersion() << ")" << std::endl;
    std::cout << "SQliteC++ version " << SQLITECPP_VERSION << std::endl;

    std::ifstream infile("/home/alan/Downloads/tmp/en-zh/UNv1.0.en-zh.en");
    if ( infile.fail()) {
	std::cout << "fail to open file"<<std::endl;
  	return EXIT_FAILURE;
    }
    std::string line;
    ////////////////////////////////////////////////////////////////////////////
    // Simple batch queries example (5/7) :
    try
    {
        // Open a database file in create/write mode
        SQLite::Database    db("test.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

        db.exec("DROP TABLE IF EXISTS en");
        db.exec("CREATE TABLE en (sentence TEXT)");
        SQLite::Statement query(db, "INSERT INTO en VALUES(?)");

        while (std::getline(infile,line)) {
            query.bindNoCopy(1,line);
            query.exec();
            query.reset();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "SQLite exception: " << e.what() << std::endl;
        return EXIT_FAILURE; // unexpected error : exit the example program
    }

    std::cout << "everything ok, quitting\n";

    return EXIT_SUCCESS;
}
