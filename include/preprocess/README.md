MyCPP
============

**A strong c/c++ preprocessing library(一个强大的c/c++预处理库)**

## 一、基本工具 ##

### 1.MY\_CAT ###

	MY_CAT(x, y) ==> x ## y (x,y先进行扩展)

	MY_CAT(1,2) ==> 12
	MY_CAT(MY_, AND(0,1)) ==> 0
	MY_CAT(MY_, OR(0,1)) ==> 1


### 2.MY\_STR ###

	MY_STR(x) ==> #x (x会先展开)

	MY_STR(1) ==> "1"
	MY_STR(MY_BOO(7)) ==> "1"
	MY_STR(MY_CAT(MY_,BOO(0))) ==> "0"


## 二、布尔运算 ##

### 1.MY\_BOOL ###

	MY_BOOL(x) ==> if x != 0 then 1 else 0 fi
	(0 <= x < 256)

	MY_BOOL(0) ==> 0
	MY_BOOL(1) ==> 1
		.
		.
		.
	MY_BOOL(255) ==> 1


### 2.MY\_AND ###

	MY_AND(x, y) ==> MY_AND(MY_BOOL(x), MY_BOOL(y))
	(0 <= x,y < 256)

	MY_AND(0,0) ==> 0
	MY_AND(1,0) ==> 0
	MY_AND(0,1) ==> 0
	MY_AND(1,1) ==> 1


### 3.MY\_OR ###

	MY_OR(x, y) ==> MY_OR(MY_BOOL(x), MY_BOOL(y))
	(0 <= x,y < 256)

	MY_OR(0,0) ==> 0
	MY_OR(1,0) ==> 1
	MY_OR(0,1) ==> 1
	MY_OR(1,1) ==> 1


### 4.MY\_NOT ###

	MY_NOT(x) ==> MY_NOT(MY_BOOL(x))
	(0 <= x < 256)

	MY_NOT(0) ==> 1
	MY_NOT(1) ==> 0


### 5.MY\_NOR ###

	MY_NOR(x,y) ==> MY_NOR(MY_BOOL(x), MY_BOOL(y))
	MY_NOR(x,y) <==> MY_NOT(MY_AND(x, y))
	(0 <= x,y < 256)

	MY_NOR(0,0) ==> 1
	MY_NOR(1,0) ==> 0
	MY_NOR(0,1) ==> 0
	MY_NOR(1,1) ==> 0


### 6.MY\_XOR(x,y) ###

	MY_XOR(x,y) ==> MY_XOR(MY_BOOL(x), MY_BOOL(y))
	(0 <= x,y < 256)

	MY_XOR(0,0) ==> 0
	MY_XOR(1,0) ==> 1
	MY_XOR(0,1) ==> 1
	MY_XOR(1,1) ==> 0


## 三、比较运算 ##

### 1.MY\_NOT\_EQUAL ###

	MY_NOT_EQUAL(x, y) ==> x != y ? 1 : 0;
	(0 <= x,y < 256)

	MY_NOT_EQUAL(0, 0) ==> 0
	MY_NOT_EQUAL(1, 0) ==> 1
	MY_NOT_EQUAL(7, 8) ==> 1

### 2.MY\_EQUAL ###

	MY_EQUAL(x, y) ==> x == y ? 1 : 0;
	(0 <= x,y < 256)

	MY_EQUAL(0, 0) ==> 1
	MY_EQUAL(1, 0) ==> 0
	MY_EQUAL(7, 8) ==> 0


## 四、算数运算 ##

### 1.MY\_INC ###

	MY_INC(x) ==> x + 1 (0 <= x < 256)

	MY_INC(0) ==> 1
	MY_INC(1) ==> 2
		.
		.
		.
	MY_INC(255) ==> 256
	MY_INC(256) ==> 256


### 2.MY\_DEC ###

	MY_DEC(x) ==> x + 1 (0 < x <= 256)

	MY_DEC(0) ==> 0
	MY_DEC(1) ==> 0
	MY_DEC(2) ==> 1
		.
		.
		.
	MY_DEC(256) ==> 255


(未完)

