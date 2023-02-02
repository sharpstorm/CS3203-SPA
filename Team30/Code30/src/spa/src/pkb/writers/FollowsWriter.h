#pragma once

#include "../storage/FollowsStorage.h"

class FollowsWriter {
public:
	FollowsWriter(FollowsStorage& followsStore);

	void addFollows(int arg1, int arg2);
private:
	FollowsStorage& followsStore;

};