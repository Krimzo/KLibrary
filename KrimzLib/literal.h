#pragma once


/* MEMORY */
long double operator"" _kb(long double obj)
{
	return obj * 1024;
}
long double operator"" _mb(long double obj)
{
	return obj * 1048576;
}
long double operator"" _gb(long double obj)
{
	return obj * 1073741824;
}
long double operator"" _tb(long double obj)
{
	return obj * 1099511627776;
}
