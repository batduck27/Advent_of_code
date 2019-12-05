#pragma once

static const int DIM = 256;
static const int SUFFIX[] = {17, 31, 73, 47, 23};

std::vector<int> getASCIICodes(std::string);

void KnothRound(const std::vector<int>&, int, int&, size_t&);

std::string KnothHash(std::string);
