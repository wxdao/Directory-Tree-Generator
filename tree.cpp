#include "tree.h"
#include "dirent.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

using namespace std;

void _getdtree(string path,DirTree &dtree)
{
  DIR *pdir=opendir(path.c_str());
  dirent *ent;
  struct stat sb;
  while ((ent=readdir(pdir))!=NULL)
    {
      if(strncmp(ent->d_name, ".", 1) == 0)
        continue;
      if(stat((path+"/"+string(ent->d_name)).c_str(),&sb)>=0 && S_ISDIR(sb.st_mode))
        {
          DirNode dn;
          dn.name=ent->d_name;
          dn.type=Dir;
          DirTree subdir;
          _getdtree(path + "/" + ent->d_name,subdir);
          dn.dir=subdir;
          dtree.push_back(dn);
        }
      else
        {
          DirNode dn;
          dn.name=ent->d_name;
          dn.type=File;
          dtree.push_back(dn);
        }
    }
}

DirTree getDirTree(const char *path)
{
  DirTree dt1;
  _getdtree(path,dt1);
  return dt1;
}

bool whole =false;
bool findDirbyNameA(DirTree &dtree, string name)
{
  whole =false;
  for(DirNode i : dtree)
    {
      if(whole==true)
        break;
      if(i.name==name)
        {
          whole=true;
          break;
        }
      if(i.type==Dir)
        {
          findDirbyNameA(i.dir,name);
        }
    }
  return whole;
}
bool findDirbyNameB(DirTree &dtree, string name, string do_not)
{
  whole =false;
  for(DirNode i : dtree)
    {
      if(whole==true)
        break;
      if((do_not + "/" + i.name)==name)
        {
          whole=true;
          break;
        }
      if(i.type==Dir)
        {
          findDirbyNameB(i.dir,name,do_not + "/" + i.name);
        }
    }
  return whole;
}

bool findDirbyFuncA(DirTree &dtree, CheckFunc func)
{
  whole =false;
  for(DirNode i : dtree)
    {
      if(whole==true)
        break;
      if(func(i.name))
        {
          whole=true;
          break;
        }
      if(i.type==Dir)
        {
          findDirbyFuncA(i.dir,func);
        }
    }
  return whole;
}

bool findDirbyFuncB(DirTree &dtree, CheckFunc func, string do_not)
{
  whole =false;
  for(DirNode i : dtree)
    {
      if(whole==true)
        break;
      if(func(do_not + "/" + i.name))
        {
          whole=true;
          break;
        }
      if(i.type==Dir)
        {
          findDirbyFuncB(i.dir,func,do_not + "/" + i.name);
        }
    }
  return whole;
}

void paintDirTree(DirTree &dt, FILE *f, int with_file, string do_not)
{
  for(DirNode i : dt)
    {
      if(i.type==File)
        {
          if(with_file)
            {
              fprintf(f,"%s[F]%s\n",do_not.c_str(),i.name.c_str());
            }
        }
      else
        {
          fprintf(f,"%s[D]%s\n",do_not.c_str(),i.name.c_str());
          paintDirTree(i.dir,f,with_file,do_not+"----");
        }
    }
}
