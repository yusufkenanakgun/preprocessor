struct ArrayTable {
char name[10];
int dim; // 1: 1-dimensional, 2: 2-dimensional
char size1[10]; // size of dimension 1
char size2[10]; // size of dimension 2
};
struct ArrayTable AT[20]; // max. 20 arrays


struct ParseTable {
char oper[10]; // @read, @copy, etc.
char lhs[10]; // array on the left hand side
char rhs1[10]; // first array on the right hand side
char rhs2[10]; // second array on the right hand side
};
struct ParseTable PT;

int P_dot;
int P_sum;
int P_aver;
int aCount=0;
