#pragma once

#include <stdio.h>

#include <openssl/rand.h>
#include <time.h>

char *genRandString(int num, int flag);
