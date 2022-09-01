#pragma once

#include <iostream>
#include <mysql\jdbc.h>
#include <vector>
#include <memory>
#include <exception>
#include <sstream>

class MySQL
{
private:
	//forward declaration
	struct m_Row;
public:
	MySQL( const char* host, const char* user, const char* password, char* database_name);
	~MySQL();

	//functon for checking if database (schema) of name databaseName exists on SQL server
	bool checkIfDatabaseExists(const char *databaseName);
	//function for selecting database (schema)
	void selectDatabase(const char *databaseName);
	//functon for creating database of name databaseName on SQL server
	void createDatabase(const char *databaseName);
	//functon for checking if table of name tableName exists in database databaseName
	bool checkIfTableExists(const char * databaseName, const char *tableName);
	//functon for creating table of name tableName in database databaseName
	void createTable(const char * databaseName, const char *tableName);
	// add row to table tableName in database databaseName
	void addRowIntoTable(const char * databaseName, const char *tableName, m_Row row);
	//function for priting all columns of table tableName in database databaseName
	void printAllColumns(const char * databaseName, const char *tableName);
	//function for printing all records of table tableName in database databaseName
	void printAllRecords(const char * databaseName, const char *tableName);

private:
	//url
	const char * m_url;
	//host
	const char * m_host;
	// username
	const char * m_username;
	// password
	const char * m_password;
	// database
	char * m_database_name;
	//driver object
	sql::Driver* m_driver;
	//connection object
	std::unique_ptr< sql::Connection> m_connection;
	//statement object for executing sql statements
	std::unique_ptr< sql::Statement > m_statement;

	struct m_Row {
		std::string name;
		int score;
		std::string date;
	};

};

