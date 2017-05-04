typedef struct strtest0_s strtest0;

struct strtest0_s {
double a;
int ia;
double b;
int ib;
double c;
int ic;
double d;
int id;
};

strtest0 str_ret0()
{
	strtest0 c;
	c.a=1.0;
	c.b=2.0;
	c.c=3.0;
	c.d=4.0;
	return(c);
}
