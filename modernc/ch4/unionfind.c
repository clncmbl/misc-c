
#include <stdio.h>
#include <stdint.h>


size_t Find(const size_t* const tbl, const size_t node) {
  if (tbl[node] == SIZE_MAX) {
    return node;
  }
  return Find(tbl, tbl[node]);
}

void FindReplace(
    size_t* const tbl,
    const size_t node,
    const size_t new_parent) {

  const size_t old_parent = tbl[node];
  tbl[node] = new_parent;
  if (old_parent == SIZE_MAX) return;

  FindReplace(tbl, old_parent, new_parent);
}

size_t FindCompress(size_t* const tbl, const size_t node) {
  const size_t root = Find(tbl, node);
  size_t current = node;
  size_t next;
  while (current != root) {
    next = tbl[current];
    tbl[current] = root;
    current = next;
  }
  return root;
}

void Union(
    size_t* const tbl,
    const size_t node,
    const size_t node_partree) {
  size_t root = FindCompress(tbl, node_partree);
  FindReplace(tbl, node, root);
}

int main(int argc, char* argv[argc+1]) {

  const size_t nodecount = 5;
  size_t table[nodecount];

  for (size_t i = 0; i < nodecount; ++i) {
    table[i] = SIZE_MAX;
  }

  table[2] = 4;
  size_t root = Find(table, 2);
  printf("%zu\n", root);

  FindCompress(table, 1);
  FindReplace(table, 2, 3);

  Union(table, 1, 0);
  Union(table, 2, 1);

  for (size_t i = 0; i < nodecount; ++i) {
    printf("%zu:\t%zu\n", i, table[i]);
  }
}

