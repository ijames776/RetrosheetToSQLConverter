#pragma once
#include <mysql.h>
#include <vector>
#include <string>

class FDBConnect
{
public:
	FDBConnect(const char* Host, const char* UserName, const char* Password, const char* DatabaseName, int Port);
	~FDBConnect();

	std::vector<std::vector<std::string>> GetQueryResults(std::string Query);
	bool bWasUpdateSuccessful(std::string Query);

private:
	MYSQL* conn{}; // declare a pointer connection object
	MYSQL_ROW row{}; // creates an index style object to hold rows of data
	MYSQL_RES* res{}; // creates a pointer to an object that holds a query result

	const char* Host{};
	const char* UserName{};
	const char* Password{};
	const char* DatabaseName{};
	int Port{};

	void OutputMessage(std::string message);

};