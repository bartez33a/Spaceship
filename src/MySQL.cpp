#include "../headers/MySQL.h"

MySQL::MySQL(const char* host, const char * user, const char * password, char * database_name, int port) : m_host{ host }, m_username{ user }, m_password{ password }, m_database_name{ database_name },
m_port{ port }, m_driver{ sql::mysql::get_driver_instance() }
{
	try {
		// connection properties object
		sql::ConnectOptionsMap connection_properties;

		connection_properties["hostName"] = m_host;
		connection_properties["userName"] = m_username;
		connection_properties["password"] = m_password;
		connection_properties["schema"] = m_database_name; //database == schema
		connection_properties["port"] = m_port;
		connection_properties["OPT_RECONNECT"] = true;

		//connect to database
		m_connection = std::move(std::unique_ptr<sql::Connection>(m_driver->connect(connection_properties)));
		//create statement
		m_statement = std::move(std::unique_ptr<sql::Statement>(m_connection->createStatement()));
		//try block is ok 
		m_IsDatabaseConnected = true;
	}
	catch (std::exception &ex)
	{
		//clear console
		system("cls");
		//show console
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		std::cout << "Unable to connect to database!\n";
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		char c;
		while (true)
		{
			std::cout << "Do you want to play offline? Your score wont be saved!\n";
			std::cout << "You can try again or play without database t\\y\\n: ";
			std::cin >> c;

			if (c == 'y' || c == 'n' || c == 't')
			{
				if (c == 'y')
				{
					m_IsDatabaseConnected = false;
				}
				else if (c == 'n')
				{
					m_IsDatabaseConnected == false;
					exit(-1);
				}
				else if (c == 't')
				{
					m_connectToDatabase(host, user, password, database_name, port);
				}
				break;
			}
			std::cout << "\n";
		}
		//hide console
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		getchar();

	}
	catch (const char *ex)
	{
		//clear console
		system("cls");
		//show console
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		std::cout << "Unable to connect to database!\n";
		std::cout << "EXCEPTION: " << ex << '\n';
		char c;
		while (true)
		{
			std::cout << "Do you want to play offline? Your score wont be saved!\n";
			std::cout << "You can try again or play without database t\\y\\n: ";
			std::cin >> c;

			if (c == 'y' || c == 'n' || c == 't')
			{
				if (c == 'y')
				{
					m_IsDatabaseConnected = false;
				}
				else if (c == 'n')
				{
					m_IsDatabaseConnected == false;
					exit(-1);
				}
				else if (c == 't')
				{
					m_connectToDatabase(host, user, password, database_name, port);
				}
				break;
			}
			std::cout << "\n";
		}
		//hide console
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		getchar();
	}
}

MySQL::~MySQL()
{
	//close connection with sql server
	if (m_IsDatabaseConnected)
	{ 
		m_connection->close();
	}
	
}

bool MySQL::checkIfDatabaseExists(const char * databaseName)
{
	try {
		std::unique_ptr<sql::ResultSet> available_databases(m_statement->executeQuery("SHOW DATABASES"));
		bool is_specified_database_available = false;
		while (available_databases->next())
		{
			if (available_databases->getString(1) == databaseName)
			{
				is_specified_database_available = true;
			}
			std::cout << available_databases->getString(1) << '\n';
		}

		return is_specified_database_available;
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();

	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();

	}
}

void MySQL::selectDatabase(const char * databaseName)
{
	try {
		m_connection->setSchema(databaseName);
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();

	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();

	}
}

void MySQL::createDatabase(const char * databaseName)
{
	try 
	{		
		std::stringstream str;
		str << "CREATE DATABASE " << databaseName;
		std::string command;
		command = str.str();

		m_statement->execute(command);
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();

	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();

	}
}

bool MySQL::checkIfTableExists(const char * databaseName, const char * tableName)
{
	try {
		//check if in spaceship is table best_score
		bool is_specifiedf_table_available = false;
		//first use specified database 
		m_connection->setSchema(databaseName);
		//then ask for tables
		std::unique_ptr<sql::ResultSet> available_tables(m_statement->executeQuery("SHOW TABLES"));

		while (available_tables->next())
		{
			if (available_tables->getString(1) == tableName)
			{
				is_specifiedf_table_available = true;
			}
			
		}
		return is_specifiedf_table_available;
	} //try
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}
}

// creates table with specified tableName in database databaseName with 4 columns:
// ID - INT, primary key, autoincrement
// Name - Text
// Score - INT
// Date - DATE
void MySQL::createTable(const char * databaseName, const char * tableName)
{
	try {
		//first use specified database 
		m_connection->setSchema(databaseName);
		//CREATE TABLE _tableName_ (Columns...);
		//Columns -> (Col1, Col2, ...)
		// Col -> (ColName ColumnType optionalArgs, Col2, ...)
		std::stringstream ss1, ss2, ss3;
		ss1 << "CREATE TABLE " << tableName << 
			" (ID int(11) NOT NULL, Name text COLLATE utf8_polish_ci NOT NULL DEFAULT 'Empty', Score int(11) NOT NULL, Date date NOT NULL) ENGINE = InnoDB DEFAULT CHARSET = utf8 COLLATE = utf8_polish_ci";
		
		ss2 << "ALTER TABLE " << tableName << " ADD PRIMARY KEY(ID);";

		ss3 << "ALTER TABLE " << tableName << " MODIFY ID int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

		m_statement->execute(ss1.str());
		m_statement->execute(ss2.str());
		m_statement->execute(ss3.str());

	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}
}

void MySQL::addRowIntoTable(const char * databaseName, const char * tableName, m_Row row)
{
	try {
		//first use specified database 
		m_connection->setSchema(databaseName);
		std::stringstream command;
		command << "INSERT INTO " << tableName << " (Name, Score, Date) VALUES ('" << row.name << "', " <<
		row.score <<  ", '" <<  row.date <<  "')";
		m_statement->execute(command.str());
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}
}

void MySQL::printAllColumns(const char * databaseName, const char * tableName)
{
	try {
		//first use specified database 
		m_connection->setSchema(databaseName);
		//then select table
		std::stringstream command;
		command << "select * from " << tableName;
		std::unique_ptr< sql::ResultSet > rs(m_statement->executeQuery(command.str()));

		struct column {
			std::string name;
			std::string dataType;
		};

		std::vector<column> columns;

		auto print_all_columns = [&]() {
			// /* IMPORTANT! */
			//cannot use unique_ptr because of virtual destructor in class sql::ResultSetMetaData
			//std::unique_ptr <sql::ResultSetMetaData> rsmdd(rs2->getMetaData());
			sql::ResultSetMetaData * meta = rs->getMetaData();
			int columnsCtr = meta->getColumnCount();

			for (int i = 1; i <= columnsCtr; i++)
			{
				std::cout << meta->getColumnName(i) << "\t";
				std::cout << meta->getColumnTypeName(i) << '\n';
				column col = { meta->getColumnName(i), meta->getColumnTypeName(i) };
				columns.push_back(col);
			}
		};
		print_all_columns();
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}
}

void MySQL::printAllRecords(const char * databaseName, const char * tableName)
{
	try {
		//first use specified database 
		m_connection->setSchema(databaseName);
		//then select table
		std::stringstream command;
		command << "select * from " << tableName;
		std::unique_ptr< sql::ResultSet > rs(m_statement->executeQuery(command.str()));

		struct column {
			std::string name;
			std::string dataType;
		};

		std::vector<column> columns;

		//cannot use unique_ptr because of virtual destructor in class sql::ResultSetMetaData
		//std::unique_ptr <sql::ResultSetMetaData> rsmdd(rs2->getMetaData());
		sql::ResultSetMetaData * meta = rs->getMetaData();
		int columnsCtr = meta->getColumnCount();

		for (int i = 1; i <= columnsCtr; i++)
		{
			column col = { meta->getColumnName(i), meta->getColumnTypeName(i) };
			columns.push_back(col);
		}


		unsigned int rowCtr = rs->rowsCount();

		// print each row
		for (int row = 1; row <= rowCtr; row++)
		{
			//for each column.
			for (int col = 0; col < columns.size(); col++)
			{
				//column name
				std::cout << columns[col].name << ": ";
				rs->absolute(row);

				//column value
				if (columns[col].dataType == "INT")
				{
					std::cout << rs->getInt(columns[col].name.c_str()) << "\t";
				}
				else if (columns[col].dataType == "TEXT")
				{
					std::cout << rs->getString(columns[col].name.c_str()) << "\t";
				}
				else if (columns[col].dataType == "DATETIME")
				{
					std::cout << rs->getString(columns[col].name.c_str()) << "\t";
				}
			}
			std::cout << '\n';
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}
}

void MySQL::writeScore(std::string name, int score)
{
	try
	{
		//get time since NULL
		std::time_t result = std::time(nullptr);
		//table to store date
		char time[50];
		// struct for timer
		tm tm2;
		localtime_s(&tm2, &result);

		// yyyy-mm-dd hh:mm:ss
		std::stringstream ss;
		ss << tm2.tm_year + 1900 << "-" <<
			std::setw(2) << std::setfill('0') << tm2.tm_mon + 1 << "-" <<
			std::setw(2) << std::setfill('0') << tm2.tm_mday << " " <<
			std::setw(2) << std::setfill('0') << tm2.tm_hour << ":" <<
			std::setw(2) << std::setfill('0') << tm2.tm_min << ":" <<
			std::setw(2) << std::setfill('0') << tm2.tm_sec;

		m_Row row{ name, score, ss.str() };
		addRowIntoTable("spaceship", "best_score", row);

		//temporary
		std::cout << ss.str() << '\n';
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
	}

}

//this function returns best scores
//you can limit number of records, default -> without limit
std::vector<int> MySQL::getBestScores(const char * databaseName, const char *tableName, int limit)
{
	try 
	{
		std::vector<int> bestScores;
		//first use specified database 
		m_connection->setSchema(databaseName);
		//then select table
		std::stringstream command;
		if (limit != 0)
		{
			command << "select Score from " << tableName << " ORDER BY Score DESC LIMIT " << limit;
		}
		else
		{
			command << "select Score from " << tableName << " ORDER BY Score DESC";
		}
		std::unique_ptr< sql::ResultSet > rs(m_statement->executeQuery(command.str()));


		while (rs->next())
		{
			bestScores.push_back(rs->getInt("Score"));
			if (bestScores.size() >= 10)
				break;
		}

		return bestScores;
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
		exit(EXIT_FAILURE);
	}
}

//this function reads top ten best scores and names and returns it as vecotr of MySQL::m_Row objects
std::vector<MySQL::m_Row> MySQL::getTopTen(const char * databaseName, const char *tableName)
{
	try
	{
		std::vector<m_Row> topTen;

		m_Row data;

		//first use specified database 
		m_connection->setSchema(databaseName);
		//then select table
		std::stringstream command;
		//order by score and if scores are equal, order by date ascending (first score (by date) is first)
		command << "select Name, Score  from " << tableName << " ORDER BY Score DESC, Date ASC LIMIT 10";
		std::unique_ptr< sql::ResultSet > rs(m_statement->executeQuery(command.str()));

		while (rs->next())
		{
			data.name = rs->getString("Name");
			data.score = rs->getInt("Score");
			topTen.emplace_back(data);

			//read only ten best scores
			if (topTen.size() >= 10)
			{
				break;
			}
		}

		return topTen;
	}
	catch (std::exception &ex)
	{
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		getchar();
		exit(EXIT_FAILURE);
	}
	catch (const char *ex)
	{
		std::cout << "EXCEPTION: " << ex << '\n';
		getchar();
		exit(EXIT_FAILURE);
	}
}

bool MySQL::isDatabaseConnected() const
{
	return m_IsDatabaseConnected;
}

void MySQL::m_connectToDatabase(const char* host, const char * user, const char * password, char * database_name, int port)
{
	static int tryAgain = 0;
	tryAgain++;

	if (tryAgain >= 4)
	{
		system("cls");
		std::cout << "I'm sorry.\nToo many tries to connect to database.\nCheck connection with MySQL server.\n";
		getchar(); getchar();
		exit(-1);
	}

	try {
		// connection properties object
		sql::ConnectOptionsMap connection_properties;

		connection_properties["hostName"] = m_host;
		connection_properties["userName"] = m_username;
		connection_properties["password"] = m_password;
		connection_properties["schema"] = m_database_name; //database == schema
		connection_properties["port"] = m_port;
		connection_properties["OPT_RECONNECT"] = true;

		//connect to database
		m_connection = std::move(std::unique_ptr<sql::Connection>(m_driver->connect(connection_properties)));
		//create statement
		m_statement = std::move(std::unique_ptr<sql::Statement>(m_connection->createStatement()));

		//try block is ok 
		m_IsDatabaseConnected = true;
	}
	catch (std::exception &ex)
	{
		//clear console
		system("cls");
		//show console
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		std::cout << "Unable to connect to database!\n";
		std::cout << "STD EXCEPTION: " << ex.what() << '\n';
		char c;
		while (true)
		{
			std::cout << "Do you want to play offline? Your score wont be saved!\n";
			std::cout << "You can try again or play without database t\\y\\n: ";
			std::cin >> c;

			if (c == 'y' || c == 'n' || c == 't')
			{
				if (c == 'y')
				{
					m_IsDatabaseConnected = false;
				}
				else if (c == 'n')
				{
					m_IsDatabaseConnected == false;
					exit(-1);
				}
				else if (c == 't')
				{
					m_connectToDatabase(host, user, password, database_name, port);
				}
				break;
			}
			std::cout << "\n";
		}
		//hide console
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		getchar();

	}
	catch (const char *ex)
	{
		//clear console
		system("cls");
		//show console
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		std::cout << "Unable to connect to database!\n";
		std::cout << "EXCEPTION: " << ex << '\n';
		char c;
		while (true)
		{
			std::cout << "Do you want to play offline? Your score wont be saved!\n";
			std::cout << "You can try again or play without database t\\y\\n: ";
			std::cin >> c;

			if (c == 'y' || c == 'n' || c == 't')
			{
				if (c == 'y')
				{
					m_IsDatabaseConnected = false;
				}
				else if (c == 'n')
				{
					m_IsDatabaseConnected == false;
					exit(-1);
				}
				else if (c == 't')
				{
					m_connectToDatabase(host, user, password, database_name, port);
				}
				break;
			}
			std::cout << "\n";
		}
		//hide console
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		getchar();
	}
}
