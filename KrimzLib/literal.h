#pragma once


/* MEMORY */
unsigned long long int operator"" _kb(unsigned long long int obj) {
	return obj * 1024;
}
unsigned long long int operator"" _mb(unsigned long long int obj) {
	return obj * 1048576;
}
unsigned long long int operator"" _gb(unsigned long long int obj) {
	return obj * 1073741824;
}
unsigned long long int operator"" _tb(unsigned long long int obj) {
	return obj * 1099511627776;
}
long double operator"" _kb(long double obj) {
	return obj * 1024;
}
long double operator"" _mb(long double obj) {
	return obj * 1048576;
}
long double operator"" _gb(long double obj) {
	return obj * 1073741824;
}
long double operator"" _tb(long double obj) {
	return obj * 1099511627776;
}
