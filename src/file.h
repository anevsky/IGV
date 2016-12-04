/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>
#include <string>

class File {
public:
	typedef std::string t_FileName;
	typedef std::string t_FileData;
	typedef bool t_SourceFileAvailable;
	
private:	
	t_FileName m_FileName;
	t_FileData m_FileData;
	t_SourceFileAvailable m_SourceFileAvailable;
public:	
	File() : m_FileName(), m_FileData(), m_SourceFileAvailable(false) {}
	File(const t_FileName& input_FileName) { Read(input_FileName); };
	
	bool Read(const t_FileName& input_FileName);
	bool Save(const t_FileData& input_DataToSave, t_FileName input_FileName);
	
	const bool GetData(std::string& output_Data) const;
	std::ostream& GetData(std::ostream& output_Stream) const;
	t_SourceFileAvailable IsSourceFileAvailable(void);
};

#endif
