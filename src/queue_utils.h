#ifndef QUEUE_UTIL
#define QUEUE_UTIL

typedef struct TNode
{
  int id;
  char buffer[200];
  struct TNode* next;
}TNode;
int insert(int id, const char * chunck, struct TNode **ptr_init);
int removeList(struct TNode **ptr_init);
#endif
