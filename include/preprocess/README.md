NULL_PP
============

**A c/c++ preprocessing library(一个c/c++预处理库)**

## 一、基本工具 ##

### 1.NULL_PP_CAT ###

	NULL_PP_CAT(x, y) ==> x ## y (x,y先进行扩展)

	NULL_PP_CAT(1,2) ==> 12
	NULL_PP_CAT(NULL_PP_, AND(0,1)) ==> 0
	NULL_PP_CAT(NULL_PP_, OR(0,1)) ==> 1


### 2.NULL_PP_STR ###

	NULL_PP_STR(x) ==> #x (x会先展开)

	NULL_PP_STR(1) ==> "1"
	NULL_PP_STR(NULL_PP_BOO(7)) ==> "1"
	NULL_PP_STR(NULL_PP_CAT(NULL_PP_,BOO(0))) ==> "0"


## 二、布尔运算 ##

### 1.NULL_PP_BOOL ###

	NULL_PP_BOOL(x) ==> if x != 0 then 1 else 0 fi
	(0 <= x < 256)

	NULL_PP_BOOL(0) ==> 0
	NULL_PP_BOOL(1) ==> 1
		.
		.
		.
	NULL_PP_BOOL(255) ==> 1


### 2.NULL_PP_AND ###

	NULL_PP_AND(x, y) ==> NULL_PP_AND(NULL_PP_BOOL(x), NULL_PP_BOOL(y))
	(0 <= x,y < 256)

	NULL_PP_AND(0,0) ==> 0
	NULL_PP_AND(1,0) ==> 0
	NULL_PP_AND(0,1) ==> 0
	NULL_PP_AND(1,1) ==> 1


### 3.NULL_PP_OR ###

	NULL_PP_OR(x, y) ==> NULL_PP_OR(NULL_PP_BOOL(x), NULL_PP_BOOL(y))
	(0 <= x,y < 256)

	NULL_PP_OR(0,0) ==> 0
	NULL_PP_OR(1,0) ==> 1
	NULL_PP_OR(0,1) ==> 1
	NULL_PP_OR(1,1) ==> 1


### 4.NULL_PP_NOT ###

	NULL_PP_NOT(x) ==> NULL_PP_NOT(NULL_PP_BOOL(x))
	(0 <= x < 256)

	NULL_PP_NOT(0) ==> 1
	NULL_PP_NOT(1) ==> 0


### 5.NULL_PP_NOR ###

	NULL_PP_NOR(x,y) ==> NULL_PP_NOR(NULL_PP_BOOL(x), NULL_PP_BOOL(y))
	NULL_PP_NOR(x,y) <==> NULL_PP_NOT(NULL_PP_AND(x, y))
	(0 <= x,y < 256)

	NULL_PP_NOR(0,0) ==> 1
	NULL_PP_NOR(1,0) ==> 0
	NULL_PP_NOR(0,1) ==> 0
	NULL_PP_NOR(1,1) ==> 0


### 6.NULL_PP_XOR(x,y) ###

	NULL_PP_XOR(x,y) ==> NULL_PP_XOR(NULL_PP_BOOL(x), NULL_PP_BOOL(y))
	(0 <= x,y < 256)

	NULL_PP_XOR(0,0) ==> 0
	NULL_PP_XOR(1,0) ==> 1
	NULL_PP_XOR(0,1) ==> 1
	NULL_PP_XOR(1,1) ==> 0


## 三、比较运算 ##

### 1.NULL_PP_NOT\_EQUAL ###

	NULL_PP_NOT_EQUAL(x, y) ==> x != y ? 1 : 0;
	(0 <= x,y < 256)

	NULL_PP_NOT_EQUAL(0, 0) ==> 0
	NULL_PP_NOT_EQUAL(1, 0) ==> 1
	NULL_PP_NOT_EQUAL(7, 8) ==> 1

### 2.NULL_PP_EQUAL ###

	NULL_PP_EQUAL(x, y) ==> x == y ? 1 : 0;
	(0 <= x,y < 256)

	NULL_PP_EQUAL(0, 0) ==> 1
	NULL_PP_EQUAL(1, 0) ==> 0
	NULL_PP_EQUAL(7, 8) ==> 0


## 四、算数运算 ##

### 1.NULL_PP_INC ###

	NULL_PP_INC(x) ==> x + 1 (0 <= x < 256)

	NULL_PP_INC(0) ==> 1
	NULL_PP_INC(1) ==> 2
		.
		.
		.
	NULL_PP_INC(255) ==> 256
	NULL_PP_INC(256) ==> 256


### 2.NULL_PP_DEC ###

	NULL_PP_DEC(x) ==> x + 1 (0 < x <= 256)

	NULL_PP_DEC(0) ==> 0
	NULL_PP_DEC(1) ==> 0
	NULL_PP_DEC(2) ==> 1
		.
		.
		.
	NULL_PP_DEC(256) ==> 255


(未完)

