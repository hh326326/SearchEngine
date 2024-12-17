
#include "cache/lru_cache.h"

int main() {
  hh::LruCache lc(5);
  lc.AddRecord("a", "1");
  lc.LruTest();
  lc.AddRecord("b", "2");
  lc.LruTest();
  lc.AddRecord("b", "2");
  lc.LruTest();
  lc.AddRecord("c", "3");
  lc.LruTest();
  lc.AddRecord("d", "4");
  lc.LruTest();
  lc.AddRecord("e", "5");
  lc.LruTest();
  lc.AddRecord("c", "3");
  lc.LruTest();
  lc.AddRecord("d", "4");
  lc.LruTest();
  lc.AddRecord("f", "6");
  lc.LruTest();
  lc.AddRecord("f", "6");
  lc.LruTest();
  lc.AddRecord("g", "7");
  lc.LruTest();

  return 0;
}