/**
 * @file    :file.h
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-13
 */

#ifndef __FILE_H__
#define __FILE_H__

#include <fstream>
#include <ios>
#include <string>

namespace hh {
using std::ifstream;
using std::ios_base;
using std::string;

ifstream Open(const char *path, ios_base::openmode = ios_base::in);

} // namespace hh

#endif // FILE_H