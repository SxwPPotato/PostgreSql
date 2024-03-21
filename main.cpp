#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#pragma execution_character_set( "utf-8" )

class Tablework {

public:
	Tablework(){

	}

	void createtable(pqxx::connection& conn) {

		pqxx::transaction tx(conn);

		static const char* query = "CREATE TABLE if not exists client(     \
		Client_id serial PRIMARY key,                                      \
		Name text NOT NULL,                                                \
		Surname text NOT NULL,                                             \
		Email text NOT NULL,                                               \
		Telephone BIGINT                                                  \
	    );";

		tx.exec(query);

		tx.commit();
	}

	void newclient(pqxx::connection& conn, std::string name, std::string surname, std::string email) {
		pqxx::work tx{ conn };
		tx.exec("INSERT INTO client(Name, Surname, Email) "			
		"VALUES('" + tx.esc(name) + "', '" + tx.esc(surname) + "' , '" + tx.esc(email) + "'  )");
		tx.commit();
	}

	void telephone(pqxx::connection& conn, std::string name, long long telephone) {
		pqxx::work tx{ conn };
		tx.exec("Update client SET Telephone = " + tx.esc(std::to_string(telephone)) + " "
			"WHERE Name = '" + tx.esc(name) + "'");
		tx.commit();
	}

	void Updateclient(pqxx::connection& conn, std::string email, std::string newname, std::string newsurname, std::string newemail) {
		pqxx::work tx{ conn };
		tx.exec("Update client SET Name = '" + tx.esc(newname) + "' ,Surname = '" + tx.esc(newsurname) + "', Email = '" + tx.esc(newemail) + "' "
			"WHERE Email = '" + tx.esc(email) + "'");
		tx.commit();
	}

	void deletetelephone(pqxx::connection& conn, std::string name) {
		pqxx::work tx{ conn };
		tx.exec(" Update client SET Telephone  = " + tx.esc(std::to_string(0)) + " "
			"WHERE Name = '" + tx.esc(name) + "'");
		tx.commit();
	}

	void deleteclient(pqxx::connection& conn, std::string name) {
		pqxx::work tx{ conn };
		tx.exec(" DELETE From client "
			"WHERE Name = '" + tx.esc(name) + "'");
		tx.commit();
	}
	
	void selectclient(pqxx::connection& conn, std::string name) {
		pqxx::work tx{ conn };

		auto collection = tx.query<std::string, std::string, std::string, long long>("Select Name, Surname, Email, Telephone From client "
			"WHERE Name = '" + tx.esc(name) + "' OR Surname = '" + tx.esc(name) + "' OR Email = '" + tx.esc(name) + "' ");

		
		for (auto elem : collection) 
		{

			std::cout << "Name: " << std::get<0>(elem) << "  ";
			std::cout << "Surname: " << std::get<1>(elem) << "  ";
			std::cout << "Email: " << std::get<2>(elem) << "  ";
			std::cout << "Telephone: " << std::get<3>(elem) << std::endl;
		}

		tx.commit();
	}

	void selectclient(pqxx::connection& conn, long long tel) {
		pqxx::work tx{ conn };

		auto collection = tx.query<std::string, std::string, std::string, long long>("Select Name, Surname, Email, Telephone From client "
			"WHERE Telephone = " + tx.esc(std::to_string(tel)) + " ");


		for (auto elem : collection)
		{

			std::cout << "Name: " << std::get<0>(elem) << "  ";
			std::cout << "Surname: " << std::get<1>(elem) << "  ";
			std::cout << "Email: " << std::get<2>(elem) << "  ";
			std::cout << "Telephone: " << std::get<3>(elem) << std::endl;
		}

		tx.commit();
	}

};



int main(){

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	
	try {
		pqxx::connection conn(
			"host=localhost "
			"port=5432 "
			"dbname=client "
			"user=postgres "
			"password=0406Dbnfkz2005 ");


		Tablework table;
		table.createtable(conn);

		std::cout << "Table created" << std::endl;

		table.newclient(conn, "Vitaliy", "Jorikov", "vitaliy@gmail.com");
		
		std::cout << "New client created" << std::endl;

		table.telephone(conn, "Vitaliy", 79387286482);

		std::cout << "Telephone created" << std::endl;

		table.Updateclient(conn, "vitaliy@gmail.com", "Michael", "Peren", "Michael@gmail.com");
		
		std::cout << "Update client" << std::endl;

		table.selectclient(conn, "Michael");

		std::cout << "Client select" << std::endl;

		table.deletetelephone(conn, "Michael");

		std::cout << "Telephone deleted" << std::endl;

		table.deleteclient(conn, "Michael");

		std::cout << "Client deleted" << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;

} 