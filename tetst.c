/*#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

int main(int argc, char const *argv[])
{
    char palabra[20] = "test";
    uint32_t hash = jenkins_one_at_a_time_hash(palabra, strlen(palabra));
    hash = hash % 800;
    printf("%d", hash);
    return 0;
}*/
