#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <sstream>

std::vector<std::string> splitStringBySpace(const std::string& input) {
    std::istringstream buffer(input);
    std::vector<std::string> results;
    std::string temp;

    while (std::getline(buffer, temp, ' ')) {
        results.push_back(temp);
    }

    return results;
}



static int getRowidCallback(void *data, int argc, char **argv, char **azColName) {
    std::vector<std::pair<int, std::string>>* rows = static_cast<std::vector<std::pair<int, std::string>>*>(data);
    rows->push_back(std::make_pair(std::stoi(argv[0]), argv[1] ? argv[1] : "NULL"));
    return 0;
}

static int printDataCallback(void *data, int argc, char **argv, char **azColName) {
    std::string lineValue = static_cast<char*>(data);


    std::vector<std::string> results = splitStringBySpace(lineValue);
    if (!results.empty()) { // 条件检查以确保vector不为空
    std::string lastElement = results.back();
    // std::cout << "The last element is: " << lastElement << std::endl;
        for(int i = 0; i<argc; i++) {
            std::vector<std::string> filePATHresult=splitStringBySpace((argv[i] ? argv[i] : "NULL"));
            if (!filePATHresult.empty()) {
                
                    std::cout <<filePATHresult.back()<< "\\" << lastElement << "\n";
                
            }

        }
        // std::cout << "\n";
    }
    return 0;
}

int main(int argc, char* argv[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // 从命令行参数获取搜索字符串
    std::string searchTerm = "demo";  // 默认值
    if (argc > 1) {
        searchTerm = argv[1];  // 从命令行参数获取搜索字符串
    }

    // 打开数据库
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        return(0);
    } 

    // 获得含有搜索字符串的行的rowid
    std::string sql = "SELECT rowid, Line FROM Lines WHERE Line LIKE '%" + searchTerm + "%';";
    std::vector<std::pair<int, std::string>> rows;
    rc = sqlite3_exec(db, sql.c_str(), getRowidCallback, &rows, &zErrMsg);
    
    if(rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << "\n";
        sqlite3_free(zErrMsg);
    } 

    // 根据每个rowid进行查询
    for(const auto& row : rows) {
        int rowid = row.first;
        std::string lineValue = row.second;
        std::string sql = "SELECT * FROM Lines WHERE Line LIKE ' Directory of%' AND rowid < " + std::to_string(rowid) + " ORDER BY rowid DESC LIMIT 1;";
        rc = sqlite3_exec(db, sql.c_str(), printDataCallback, (void*)lineValue.c_str(), &zErrMsg);
        
        if(rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << "\n";
            sqlite3_free(zErrMsg);
        }
    }

    // 关闭数据库
    sqlite3_close(db);

    return 0;
}