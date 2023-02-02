#pragma once

#include "../../common/Types.h"

class PkbWriter {
public:
	PkbWriter(PKB& pkb);

	void addFollows(int arg1, int arg2);
	void addParent(int arg1, int arg2);
};