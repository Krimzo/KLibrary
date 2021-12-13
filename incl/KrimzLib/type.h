#pragma once


/* PRIMITIVES */
namespace kl {
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
}

/* BINARY */
namespace kl {
	typedef unsigned char byte;
	typedef std::vector<byte> bytes;
	typedef std::string bits;
}

/* WINDOW */
namespace kl {
	typedef unsigned long long key;
}

/* OPENGL */
namespace kl {
	typedef unsigned int id;
	typedef unsigned int index;
}
