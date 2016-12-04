/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include "file.h"

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool File::Read(const t_FileName& input_FileName) 
{
	
	m_SourceFileAvailable = false;
	
	m_FileName = input_FileName;
	
	std::ifstream in;
	in.open(m_FileName.c_str());
	if (in) {
		std::string s;
		size_t linesNumber(0);
		while(!in.eof()) {
			getline(in, s);
			++linesNumber;
			if (linesNumber != 1)
				m_FileData = m_FileData + "\n" + s;
			else m_FileData += s;
		}
		m_SourceFileAvailable = true;
	}	
	else {
		m_SourceFileAvailable = false;
		std::cerr << "Error: unable to open input file \""
		<< input_FileName << "\"!" << std::endl;
	}
	in.close();
	
	return m_SourceFileAvailable;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool File::Save(
  const t_FileData& input_DataToSave, 
  t_FileName input_FileName
) 
{
	bool success(false);
	
	std::ofstream out;
	out.open(input_FileName.c_str());
	if (out) {
		out << input_DataToSave;
		success = true;
	}	
	else {
		success = false;
		std::cerr << "Error: unable to save data to file \""
		<< input_FileName << "\"!" << std::endl;
	}
	out.close();
	
	return success;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const bool File::GetData(std::string& output_Data) const 
{
	
	bool output_Success(false);
	
	if (m_SourceFileAvailable) {
		output_Data = m_FileData;
		output_Success = true;
	}
	
	return output_Success;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& File::GetData(std::ostream& output_Stream) const 
{
	
	output_Stream << m_FileData;
	
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
File::t_SourceFileAvailable File::IsSourceFileAvailable(void) 
{
	return m_SourceFileAvailable;
}
