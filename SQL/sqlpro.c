#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_COL_NAME 50
#define MAX_COL_TYPE 50
#define MAX_QUERY 500

typedef struct column
{
    char colName[MAX_COL_NAME];
    char colType[MAX_COL_TYPE];
    int PK;
    int colLength;
    int colScale;
    int AutoIncrement;
    struct column* nextColumn;
}column;
typedef struct {
    char tableName[MAX_COL_NAME];
    column* ls_columns;
}SqlTable;
int parseCreate(const char *query,SqlTable *table);
void printtab(SqlTable *table);
column* createSQLColumn(char* columnName,char* columnType, int colLength, int colScale, int PK, int AutoIncrement);
int compare(char *token, char *query);
int main(){
    SqlTable table;
    int res;
    char query[MAX_QUERY];
    printf("Enter the create table command:\n");
    fgets(query,MAX_QUERY,stdin);
    query[strcspn(query, "\n")] = 0;
    res=parseCreate(query, &table);
    if (res) {
        printtab(&table);
    } else {
        printf("Error parsing CREATE TABLE query.\n");
    }
}
int parseCreate(const char *query , SqlTable* table ){
    //printf("Hello World");
    char queryCopy[MAX_QUERY];
    char *token;
    char colName[MAX_COL_NAME];
    char colType[MAX_COL_TYPE];
    int colLength ,colScale , PK, AutoIncrement ;
    colLength =colScale = PK=AutoIncrement =0;
    memset(table , 0, sizeof(SqlTable));
    
    table->ls_columns =NULL;
    strcpy(queryCopy, query);
    printf("%s",queryCopy);
    
    token =strtok(queryCopy," ,(),;");
    //use a usd function for casesensititvity check becasue of to eable usage in macOS/linux/Windows
    printf("%s",token);

    if(!compare(token,"CREATE"))
        return -1;
    token=strtok(NULL, " (),;");
    if(!compare(token,"TABLE"))
        return -1;
    token = strtok(NULL, " (),;");
    strncpy(table->tableName, token, MAX_COL_NAME - 1);
       // printf("Hello World");

    printf("%s",table->tableName);
    token = strtok(NULL, " ();");
    column* curCol = NULL;
    column* prevCol = NULL;
    while (token!=NULL) {  
        strncpy(colName, token, MAX_COL_NAME - 1);
        token = strtok(NULL, " (),;");
        
        if (token)
            return -1;//(token==NUll)
        strncpy(colType, token, MAX_COL_TYPE - 1);
        token = strtok(NULL, " (),;");
        colLength = PK = AutoIncrement =0;
        if (compare(colType, "VARCHAR")) {
            token = strtok(NULL, " (),;");
            if (!(token) && token[0] == '(') {
                colLength = atoi(&token[1]);
                strtok(NULL, " (),;");
            }
        } else if (compare(colType, "DECIMAL")){
         token = strtok(NULL, " (),;");
        if (!(token) && token[0] == '(') {
        colLength = atoi(&token[1]);  
        strtok(NULL, " (),;");  
        token = strtok(NULL, " (),;");  
        if (token != NULL) {
            colScale = atoi(token);
        }
    }
        } else if (compare(colType, "DATE")) {
            colLength = 0;
        }

        if (!(token) && compare(token, "PRIMARY")) {
            strtok(NULL, " (),;");
            PK = 1;
            token = strtok(NULL, " (),;");
        }
        
        if (!(token)&& compare(token, "AUTO_INCREMENT")) {
            AutoIncrement = 1;
            token = strtok(NULL, " (),;");
        }

        curCol = createSQLColumn(colName, colType, colLength,colScale, PK, AutoIncrement);
        if (table->ls_columns == NULL) {
            table->ls_columns = curCol;
        } else {
            prevCol->nextColumn = curCol;
        }
        prevCol = curCol;
    }

    return 0;

    //NULL tells strtok() to continue from where it left off, using the same string that was passed in the first call.
} 
column* createSQLColumn(char* columnName,char* columnType, int colLength, int colScale, int PK, int AutoIncrement) {
    column* ll_of_col = (column*)malloc(sizeof(column));
    /*if (ll_of_col == NULL) {
        perror("Failed to allocate memory for new column");
        exit(1);
    }*/
    strncpy(ll_of_col->colName, columnName, MAX_COL_NAME- 1);
    strncpy(ll_of_col->colType, columnType, MAX_COL_TYPE - 1);
    ll_of_col->colLength = colLength;
    ll_of_col->colScale=colScale;
    ll_of_col->PK = PK;
    ll_of_col->AutoIncrement = AutoIncrement;
    ll_of_col->nextColumn = NULL;
    return ll_of_col;
}
int compare( char *token, char *queryCopy) {//here is query copy is another string thats it , our interpretation 
    while (*token && *queryCopy) //until both not equal to '\0'
     {
        if ((char)*token != ( char)*queryCopy) {//        if (tolower((char)*token) != tolower(( char)*queryCopy)) {
            return 1;  
        }
        token++;
        queryCopy++;
    }
    return (*token == *queryCopy) ? 0 : 1;
}
void printtab(SqlTable * table){
    printf("Table: %s\n", table->tableName);
     if (table->ls_columns == NULL) {
        printf("No columns found.\n");
        return;
    }
    printf("Columns:\n");
    printf("%-20s | %-10s | %-8s | %-8s | %-5s | %-15s | %-15s\n",  "Column Name", "Type", "Length", "Scale",
     "PK", "Auto Increment", "Default Value");
    column * cur = table->ls_columns;
    while(!(cur->nextColumn)){
        char* autoIncrementStr = (cur->AutoIncrement == 0) ? "NULL" : "1";
        char* scaleStr = (!(cur->colScale)) ? "NULL" : (char[5]){cur->colScale + '0'};
        char* pkStr = (!(cur->PK )) ? "NULL" : "1"; 
        char* defaultValueStr = (!(cur->colLength )&& !(cur->colScale)) ? "NULL" : "NULL"; 
        printf("%-20s | %-10s | %-8d | %-8s | %-5s | %-15s | %-15s\n",cur->colName, cur->colType,cur->colLength, 
        scaleStr, pkStr, autoIncrementStr,defaultValueStr);
        cur = cur->nextColumn;
    }
}




