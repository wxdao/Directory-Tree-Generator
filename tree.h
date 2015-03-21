#ifndef TREE_H
#define TREE_H

#include <vector>
#include <string>
#include <functional>

enum NodeType{File,Dir};

#define DIRTREE std::vector<DirNode>

struct DirNode
{
  std::string name;
  NodeType type;
  DIRTREE dir;
};

typedef std::vector<DirNode> DirTree;
typedef std::function<bool(std::string)> CheckFunc;

extern "C" DirTree getDirTree(const char *path);
extern "C" bool    findDirbyNameA(DirTree &dtree, std::string name);
extern "C" bool    findDirbyNameB(DirTree &dtree, std::string name, std::string do_not="");
extern "C" bool    findDirbyFuncA(DirTree &dtree, CheckFunc func);
extern "C" bool    findDirbyFuncB(DirTree &dtree, CheckFunc func, std::string do_not="");
extern "C" void    paintDirTree(DirTree &dt,FILE *f,int with_file=0,std::string do_not="");

#endif // TREE_H
