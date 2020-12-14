#include "FDBConnect.h"
#include <iostream>

FDBConnect::FDBConnect(const char* Host, const char* UserName, const char* Password, const char* DatabaseName, int Port)
{
	this->Host = Host;
	this->UserName = UserName;
	this->Password = Password;
	this->DatabaseName = DatabaseName;
	this->Port = Port;
}

FDBConnect::~FDBConnect()
{
}

void FDBConnect::OutputMessage(std::string message)
{
	std::cout << message;
}

std::vector<std::vector<std::string>> FDBConnect::GetQueryResults(std::string Query)
{
	std::vector<std::vector<std::string>> results{};
	if (conn == nullptr)
	{
		conn = mysql_init(0);
		conn = mysql_real_connect(conn, Host, UserName, Password, DatabaseName, Port, NULL, 0);
	}
	int qstate = mysql_query(conn, Query.c_str()); // runs the query. If the data is sent successfully qstate will remain 0, if it comes back as a non-zero value the query failed

	if (!qstate) // if qstate has a non-0 value
	{
		res = mysql_store_result(conn); // store the result of the query in res
		while (row = mysql_fetch_row(res)) // goes through the results row by row setting the value of the row object to be the next row in the res object
		{
			int NumFields = mysql_num_fields(res);
			std::vector<std::string> RowData;

			for (auto i = 0; i < NumFields; i++)
			{
				if (row[i] != NULL)
				{
					RowData.push_back(row[i]);
				}
				else
				{
					RowData.push_back("NULL");
				}
			}
			results.push_back(RowData);
		}
	}
	//else { OutputMessage("Connection failed."); }
	return results;
}

bool FDBConnect::bWasUpdateSuccessful(std::string Query)
{
	// Connect to GameData DB
	if (conn == nullptr)
	{
		conn = mysql_init(0);
		conn = mysql_real_connect(conn, Host, UserName, Password, DatabaseName, Port, NULL, 0);
	}
	int qstate = 0; // sets the state of the query

	qstate = mysql_query(conn, Query.c_str()); // runs the query. If the data is sent successfully qstate will remain 0, if it comes back as a non-zero value the query failed

	if (!qstate)
	{
		//OutputMessage("List saved successfully.");
		return true;
	}
	else
	{
		//OutputMessage("List failed to upload.");
		return false;
	}
}
