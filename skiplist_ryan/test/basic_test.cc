/* ************************************************************************
> File Name:     basic_test.cc
> Author:        ryan
> Mail:          xieg678@gmail.com
> Created Time:  Sun Jul 30 19:30:41 2023
> Description:
 ************************************************************************/
#include <iostream>

#include "skiplist.hpp"
#define FILE_PATH "../store/dumpFile"

int main() {
  SkipList<std::string, std::string> skipList(6);
  skipList.InsertElement("1", "a");
  skipList.InsertElement("3", "b");
  skipList.InsertElement("7", "c");
  skipList.InsertElement("8", "ryan");
  skipList.InsertElement("9", "xie");
  skipList.InsertElement("19", "guohong");
  skipList.InsertElement("19", "guohong");
  skipList.InsertElement("19", "guohong");
  skipList.InsertElement("19", "ghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");

  std::cout << "skipList size:" << skipList.size() << std::endl;

  skipList.DumpFile();

  // skipList.load_file();

  skipList.SearchElement("9");
  skipList.SearchElement("18");

  skipList.DisplayList();

  skipList.DeleteElement("3");
  skipList.DeleteElement("7");

  std::cout << "skipList size:" << skipList.size() << std::endl;

  skipList.DisplayList();
}
