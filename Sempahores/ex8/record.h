typedef struct{
    int number;
    char name[50];
    char address[100];
}record;

typedef struct{
    record entry[100];
    int index;
}data;